// vim: sw=4 ts=4 et filetype=c
#include <stdio.h>
#include <stdlib.h>
#include "overseer.h"
#include "iface_w1_gpio.h"

int main()
{
    size_t actualsize;
    sensor_t *sensor_array = malloc(SENSOR_ARRAY_LENGTH * sizeof(*sensor_array));
    float result;
    int i = 0;

    actualsize = getSensorList(sensor_array);

    for (i = 0; i < (int)actualsize; i++)
    {
        printf("Sensor %d (%s) = %g degrees C\n", i, sensor_array[i].filename, sensor_array[i].reading);
    }
    return 0;
}
