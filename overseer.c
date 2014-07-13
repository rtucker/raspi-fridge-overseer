// vim: sw=4 ts=4 et filetype=c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "overseer.h"
#include "iface_lcd.h"
#include "iface_uart.h"
#include "iface_w1_gpio.h"

size_t read_string(char const *filename, char *target, size_t len)
{
    FILE *fp = fopen(filename, "r");

    memset(target, '\0', len);

    if (fp == NULL)
    {
        return 0;
    }

    return fread(target, 1, len, fp);
}

int main()
{
    sensor_t *sensor_array;
    size_t actualsize;
    int i = 0;
    time_t t;
    struct tm *tmp;
    char *outbuf;
    char *forecast_now;
    char *forecast_later;
    char txt[DISPLAY_WIDTH+1];

    FILE *munin;

    size_t forecast_now_len = 0, forecast_later_len = 0, outbuf_len = 0, txt_len = 0;

    // Initialize the display
    (void)uartInit();
    lcdClear();

    while(TRUE)
    {
        // Pull in sensor data
        sensor_array = malloc(SENSOR_ARRAY_LENGTH * sizeof(sensor_t));

        if (sensor_array == NULL)
        {
            // woah nelly
            printf("# malloc of sensor_array failed!\n");
            return 1;
        }
        actualsize = getSensorList(sensor_array);

        // Pull in forecast data
        forecast_now = malloc(MAX_FORECAST_WIDTH * sizeof(char));
        forecast_later = malloc(MAX_FORECAST_WIDTH * sizeof(char));

        if ((forecast_now == NULL) || (forecast_later == NULL)) 
        {
            printf("# malloc for forecast data failed!\n");
            return 1;
        }

        forecast_now_len = read_string(FORECAST_NOW, forecast_now, MAX_FORECAST_WIDTH);
        forecast_later_len = read_string(FORECAST_LATER, forecast_later, MAX_FORECAST_WIDTH);

        if (forecast_now_len == 0)
        {
            forecast_now = "No Cur Wx";
            forecast_now_len = strlen(forecast_now);
        }

        if (forecast_later_len == 0)
        {
            forecast_later = "No F'cast";
            forecast_later_len = strlen(forecast_later);
        }

        // Output the current time to the display
        outbuf = malloc(MAX_OUTBUF * sizeof(char));
        outbuf_len = 0;
        if (outbuf == NULL)
        {
            printf("# malloc for outbuf failed!\n");
            return 1;
        }

        t = time(NULL);
        tmp = localtime(&t);
        if (tmp == NULL)
        {
            snprintf(outbuf, MAX_OUTBUF, "Time: NULL");
        }
        else if (strftime(outbuf, MAX_OUTBUF, "%a %b %d %H:%M", tmp) == 0)
        {
            snprintf(outbuf, MAX_OUTBUF, "Time: INVALID");
        }
        lcdSetPosition(1,0);
        lcdWrite(outbuf);
        free(outbuf);

        // Produce a hunk of output for munin
        munin = fopen(MUNIN_FILE, "w");
        if (munin == NULL)
        {
            printf("# fopen munin failed!\n");
            return 1;
        }

        for (i = 0; i < (int)actualsize; ++i)
        {
            fprintf(munin, "sensor_%s.value %g\n",
                   sensor_array[i].filename, sensor_array[i].reading);
        }

        fclose(munin);

        // Process bottom row of display
        outbuf = malloc(MAX_OUTBUF * sizeof(char));
        outbuf_len = 0;
        if (outbuf == NULL)
        {
            printf("# malloc for outbuf failed!\n");
            return 1;
        }

        // Write forecast data
        outbuf_len += snprintf(outbuf+outbuf_len, MAX_OUTBUF-outbuf_len,
                               "%s  %s  Status:", forecast_now, forecast_later);

        // Write sensor data

        if ((int)actualsize > 1)
        {
            txt_len = snprintf(txt, DISPLAY_WIDTH+1, "F=%3.1f R=%3.1f",
                     sensor_array[0].reading, sensor_array[1].reading);
        }
        else if ((int)actualsize > 0)
        {
            txt_len = snprintf(txt, DISPLAY_WIDTH+1, "Temp: %3.1f", sensor_array[0].reading);
        }
        else
        {
            txt_len = snprintf(txt, DISPLAY_WIDTH+1, "NO DATA");
        }

        if (txt_len < (MAX_OUTBUF-DISPLAY_WIDTH))
        {
            // left-pad
            for (i = 0; i <= (DISPLAY_WIDTH-txt_len)/2; i++)
            {
                outbuf[outbuf_len++] = ' ';
            }

            // copy
            memcpy(outbuf+outbuf_len, txt, txt_len);
            outbuf_len += txt_len;

            // right-pad
            for (i = 0; i < (DISPLAY_WIDTH-txt_len)/2; i++)
            {
                outbuf[outbuf_len++] = ' ';
            }

            // null-terminate
            outbuf[outbuf_len] = '\0';
        }

        // note: outbuf_len is the number of chars written, excluding the null
        lcdWriteWide(outbuf, outbuf_len, 2);
        free(outbuf);

        // Clean up
        free(sensor_array);
        free(forecast_now);
        free(forecast_later);

        // Sleep
        sleep(1);
    }

    uartClose();

    return 0;
}
