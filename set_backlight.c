// vim: sw=4 ts=4 et filetype=c
#include <stdio.h>
#include <stdlib.h>

#include "iface_lcd.h"
#include "iface_uart.h"

int main(int argc, char *argv[])
{
    int d;

    // Read brightness from command line.  If not usable, bomb out.
    if ((argc == 2) && (sscanf(argv[1], "%d", &d) > 0))
    {
        if ((d < 0) || (d > 255))
        {
            fprintf(stderr, "invalid brightness value\n");
            return 2;
        }
    }
    else
    {
        fprintf(stderr, "usage: %s <0..255>\n", argv[0]);
        return 1;
    }

    // Initialize the display
    (void)uartInit();

    // Set the brightness
    lcdSetBrightness(d);
    fprintf(stdout, "brightness set to %d / 255\n", d);

    // Done
    uartClose();

    return 0;
}
