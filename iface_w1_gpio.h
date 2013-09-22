#ifndef __INCLUDE_IFACE_W1_GPIO_H_
#define __INCLUDE_IFACE_W1_GPIO_H_

#include "overseer.h"

// standard paths in sysfs
#define DEVICE_PATH             "/sys/bus/w1/drivers/w1_master_driver/w1_bus_master1"
#define DEVICE_READ_PATH        DEVICE_PATH "/%s/w1_slave"

// per the datasheet
// http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Temp/DS18B20.pdf
#define TEMPERATURE_MULTIPLER   0.0625

// prototypes
float convertTemperature(char msb, char lsb);
char readSensor(float *reading, char *deviceId);
size_t getSensorList(sensor_t *sensor_array);

#endif
