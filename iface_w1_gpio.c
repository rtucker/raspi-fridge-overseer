#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "iface_w1_gpio.h"
#include "overseer.h"

#define DEVICE_PATH         "/sys/bus/w1/drivers/w1_master_driver/w1_bus_master1"
#define DEVICE_READ_PATH    DEVICE_PATH "/%s/w1_slave"

// convertTemperature: converts a hex value from the 1-wire sensor into
// degrees C
float convertTemperature(char msb, char lsb)
{
    signed int rawval = 0;

    if ((msb & (1 << 7)) > 0)
    {
        // We have a negative temperature value coming in.  Extend
        // the sign bit in rawval...
        rawval -= 0x10000;
    }

    // if msb is 0xDE and lsb is 0xAD, then this is:
    //  rawval += 0xDE00;
    //  rawval += 0x00AD;
    rawval += msb << 8;
    rawval += lsb;

    // Multiply by magic number
    return (float)rawval * TEMPERATURE_MULTIPLER;
}

// readSensor: given a device id, produces the value from that sensor
char readSensor(float *reading, char *deviceId)
{
    FILE *fp;
    char fName [NAME_MAX];
    unsigned int hexout[10];
    int result = TRUE;

    // open file
    snprintf(fName, NAME_MAX, DEVICE_READ_PATH, deviceId);
    fp = fopen(fName, "r");

    // verify that we're open
    if (fp == NULL)
    {
        return FALSE;
    }

    // The input looks something like this:
    // 84 01 4b 46 7f ff 0c 10 72 : crc=72 YES
    // 84 01 4b 46 7f ff 0c 10 72 t=24250
    // As long as crc is YES, we can get what we need from the first line
    if (fscanf(fp, "%x %x %x %x %x %x %x %x %x : crc=%x YES", &hexout[0],
            &hexout[1], &hexout[2], &hexout[3], &hexout[4], &hexout[5],
            &hexout[6], &hexout[7], &hexout[8], &hexout[9]) != 10)
    {
        result = FALSE;
    }
    else
    {
        // convert to temperature
        *reading = convertTemperature(hexout[1], hexout[0]);
    }

    return result;
}

// getSensorList: populates a list of all sensors we believe exist, along
// with their present readings.
size_t getSensorList(sensor_t *sensor_array)
{
    size_t index = 0;

    DIR *dirp;
    struct dirent *dp;
    float read_value;

    dirp = opendir(DEVICE_PATH);
    while ((dp = readdir(dirp)) != NULL)
    {
        if ((dp->d_type == DT_DIR) && !(strncmp(dp->d_name, ".", 1) == 0) && readSensor(&read_value, dp->d_name))
        {
            sensor_array[index].filename = dp->d_name;
            sensor_array[index].reading = read_value;
            index++;
        }

        if (index > SENSOR_ARRAY_LENGTH) break;
    }
    (void)closedir(dirp);

    return index;
}
