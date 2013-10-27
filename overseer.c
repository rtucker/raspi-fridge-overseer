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
    sensor_t *sensor_array = malloc(SENSOR_ARRAY_LENGTH * sizeof(*sensor_array));
    size_t actualsize      = getSensorList(sensor_array);
    char outbuf[16];
    int i = 0;
    time_t t;
    struct tm *tmp;

    // Initialize the display
    (void)uartInit();
    lcdClear();

    // Produce a hunk of output for munin
    for (i = 0; i < (int)actualsize; i++)
    {
        printf("sensor_%s.value %g\n",
               sensor_array[i].filename, sensor_array[i].reading);
    }

    // Output the current time to the display
    t = time(NULL);
    tmp = localtime(&t);
    if (tmp == NULL)
    {
        snprintf(outbuf, 16, "Time: NULL");
    }
    else if (strftime(outbuf, 16, "%a %H:%M", tmp) == 0)
    {
        snprintf(outbuf, 16, "Time: INVALID");
    }
    lcdWrite(outbuf);

    // Output two temperatures to the display
    if((int)actualsize > 1)
    {
        snprintf(outbuf, 16, "0=%3.1f 1=%3.1f",
                 sensor_array[0].reading, sensor_array[1].reading);
    }
    else if((int)actualsize > 0)
    {
        snprintf(outbuf, 16, "Temp: %3.1f", sensor_array[0].reading);
    }
    else
    {
        snprintf(outbuf, 16, "NO DATA");
    }
    lcdWrite(outbuf);

    return 0;
}
