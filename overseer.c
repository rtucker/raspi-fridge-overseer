// vim: sw=4 ts=4 et filetype=c
#include <stdio.h>
#include <stdlib.h>

#include "overseer.h"
#include "iface_w1_gpio.h"

int main()
{
    sensor_t *sensor_array = malloc(SENSOR_ARRAY_LENGTH * sizeof(*sensor_array));
    size_t actualsize      = getSensorList(sensor_array);

    int i = 0;

    for (i = 0; i < (int)actualsize; i++)
    {
        printf("Sensor %d (%s) = %g degrees C\n",
               i, sensor_array[i].filename, sensor_array[i].reading);
    }
    return 0;
}
