#ifndef __INCLUDE_IFACE_W1_GPIO_H_
#define __INCLUDE_IFACE_W1_GPIO_H_

#include "overseer.h"
#include <dirent.h>

// per the datasheet
#define TEMPERATURE_MULTIPLER 0.0625

float convertTemperature(char msb, char lsb);
char readSensor(float *reading, char *deviceId);
size_t getSensorList(sensor_t *sensor_array);

#endif
