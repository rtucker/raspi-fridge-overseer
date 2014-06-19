// vim: sw=4 ts=4 et filetype=c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "overseer.h"
#include "iface_lcd.h"
#include "iface_uart.h"
#include "iface_w1_gpio.h"

int main()
{
    sensor_t *sensor_array;
    size_t actualsize;
    char outbuf[DISPLAY_WIDTH+1];
    int i = 0;
    time_t t;
    struct tm *tmp;

    // Pull in sensor data
    sensor_array = malloc(SENSOR_ARRAY_LENGTH * sizeof(sensor_t));

    if (sensor_array == NULL)
    {
        // woah nelly
        printf("# malloc of sensor_array failed!\n");
        return 1;
    }
    actualsize = getSensorList(sensor_array);

    // Initialize the display
    (void)uartInit();
    lcdClear();

    // Output the current time to the display
    t = time(NULL);
    tmp = localtime(&t);
    if (tmp == NULL)
    {
        snprintf(outbuf, DISPLAY_WIDTH+1, "Time: NULL");
    }
    else if (strftime(outbuf, DISPLAY_WIDTH+1, "%a %b %d %H:%M", tmp) == 0)
    {
        snprintf(outbuf, DISPLAY_WIDTH+1, "Time: INVALID");
    }
    lcdWrite(outbuf);

    // Produce a hunk of output for munin
    for (i = 0; i < (int)actualsize; ++i)
    {
        printf("sensor_%s.value %g\n",
               sensor_array[i].filename, sensor_array[i].reading);
    }

    // Output two temperatures to the display
    if ((int)actualsize > 1)
    {
        snprintf(outbuf, DISPLAY_WIDTH+1, "0=%3.1f 1=%3.1f",
                 sensor_array[0].reading, sensor_array[1].reading);
    }
    else if ((int)actualsize > 0)
    {
        snprintf(outbuf, DISPLAY_WIDTH+1, "Temp: %3.1f", sensor_array[0].reading);
    }
    else
    {
        snprintf(outbuf, DISPLAY_WIDTH+1, "NO DATA");
    }
    lcdWrite(outbuf);

    // Clean up
    free(sensor_array);
    uartClose();

    return 0;
}
