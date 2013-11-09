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

// Quick hits for LCD commands
void lcdClear()
{
    uartTx("\xFE\x01", 2);
}

void lcdMoveLeft() 
{
    uartTx("\xFE\x10", 2);
}

void lcdMoveRight() 
{
    uartTx("\xFE\x14", 2);
}

void lcdScrollLeft() 
{
    uartTx("\xFE\x18", 2);
}

void lcdScrollRight() 
{
    uartTx("\xFE\x1C", 2);
}

void lcdVisualDisplayOn() 
{
    uartTx("\xFE\x0C", 2);
}

void lcdVisualDisplayOff() 
{
    uartTx("\xFE\x08", 2);
}

void lcdUnderlineCursorOn() 
{
    uartTx("\xFE\x0E", 2);
}

void lcdUnderlineCursorOff() 
{
    uartTx("\xFE\x0C", 2);
}

void lcdBoxCursorOn() 
{
    uartTx("\xFE\x0D", 2);
}

void lcdBoxCursorOff() 
{
    uartTx("\xFE\x0C", 2);
}

void lcdSetBrightness(char level)
{
    // Sets a backlight brightness level between 0 and 255
    // 
    // We send 0x7C then a number between 128 and 157, which
    // we'll translate the 8-bit value into.

    char uartOut[3];
    char pwmValue;
    
    pwmValue = (level / 8) + 128;

    if (pwmValue > 157)
    {
        // Maximum value is FULLY ON (157)
        pwmValue = 157;
    }
    if (pwmValue < 128)
    {
        // Minimum value is OFF (128)
        pwmValue = 128;
    }

    uartOut[0] = 0x7C;
    uartOut[1] = pwmValue;
    uartOut[2] = 0;

    uartTx(uartOut, 2);
}
