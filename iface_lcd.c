#include <stdio.h>
#include "overseer.h"
#include "iface_lcd.h"
#include "iface_uart.h"

void lcdWrite(char *txbuf)
{
    int i;
    char newbuf[LCD_WIDTH + 1];
    char space_out = FALSE;

    // pad out the display with spaces
    for (i = 0; i < LCD_WIDTH; i++)
    {
        if (0 == (char)*(txbuf + i))
        {
            printf("lcdWrite: found null\n");
            space_out = TRUE;
        }

        if (TRUE == space_out)
        {
            newbuf[i] = ' ';
        }
        else
        {
            newbuf[i] = (char)*(txbuf + i);
        }
    }

    newbuf[LCD_WIDTH] = 0;

    uartTx(newbuf, LCD_WIDTH);
}

void lcdClear()
{
    uartTx("\xFE\x01", 2);
}
