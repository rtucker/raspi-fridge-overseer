#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "overseer.h"
#include "iface_lcd.h"
#include "iface_uart.h"
#include "time.h"

void lcdWrite(char const *txbuf)
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

void lcdWriteWide(char const *txbuf, size_t len, char line)
{
    size_t idx;
    char *newbuf;
    struct timespec tim = {0, 0.4e9};

    if (len < LCD_WIDTH)
    {
        // might as well use the old function if it's this short
        lcdWrite(txbuf);
        return;
    }
    else
    {
        // for a clean scroll-in effect
        len += LCD_WIDTH;
    }

    newbuf = malloc(len * sizeof(char));

    if (newbuf == NULL)
    {
        // oh no
        return;
    }

    memset(newbuf, ' ', len);
    memcpy((newbuf+LCD_WIDTH), txbuf, len-LCD_WIDTH);

    // Send this out...
    for (idx = 0; idx <= len-LCD_WIDTH; ++idx)
    {
        lcdSetPosition(line, 0);
        lcdWrite(newbuf+idx);
        //fprintf(stdout, "Displaying: [%.16s] at %d..%d\n", newbuf+idx, idx, idx+15);
        nanosleep(&tim, NULL);
    }

    free(newbuf);
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

void lcdSetPosition(char line, char column)
{
    // Expects zero-indexed column and one-indexed line.
    // (Like the docs...)

    char uartOut[3];
    char position = 0x80;

    switch (line)
    {
        case 1:
            position += 0;
            break;
        case 2:
            position += 64;
            break;
        case 3:
            position += LCD_WIDTH;
            break;
        case 4:
            position += LCD_WIDTH + 64;
            break;
    }

    position += column;

    uartOut[0] = 0xFE;
    uartOut[1] = position;
    uartOut[2] = 0;

    uartTx(uartOut, 2);
}
