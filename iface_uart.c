/*
 * note:
 * remove the serial console arguments from /boot/cmdline_backup.txt:
 *   console=ttyAMA0,115200 kgdboc=ttyAMA0,115200
 * also comment out the getty using the serial port in /etc/inittab
 *
 * UART0_TXD is pin 8
 * UART0_RXD is pin 10
 */

// uart code based on:
// http://www.raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "overseer.h"
#include "iface_uart.h"

static int uart0_filestream = -1;

char uartInit()
{
    if (uart0_filestream != -1)
    {
        // already open?!
        fprintf(stderr, "uartInit: uart already open\n");
        return FALSE;
    }
    uart0_filestream = open(SERIAL_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1)
    {
        fprintf(stderr, "uartInit: unable to open %s\n", SERIAL_DEVICE);
        return FALSE;
    }
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    return TRUE;
}

void uartClose()
{
    if (uart0_filestream != -1)
    {
        close(uart0_filestream);
    }

    uart0_filestream = -1;

    return;
}

char uartTx(char *txbuf, size_t len)
{
    if (uart0_filestream == -1)
    {
        fprintf(stderr, "uartTx: uart not initialized (call uartInit first)\n");
        return FALSE;
    }

    int tx_len = write(uart0_filestream, txbuf, len);

    if (tx_len < 0)
    {
        fprintf(stderr, "uartTx: uart tx error\n");
    }

    fprintf(stdout, "uartTx: len=%d tx_len=%d txbuf=\"%s\"\n", len, tx_len, txbuf);
    return TRUE;
}

size_t uartRx(char *rxbuf, size_t maxlen)
{
    if (uart0_filestream == -1)
    {
        fprintf(stderr, "uartRx: uart not initialized (call uartInit first)\n");
        return 0;
    }

    int rx_len = read(uart0_filestream, rxbuf, maxlen);

    if (rx_len < 0)
    {
        fprintf(stderr, "uartRx: uart rx error\n");
        return 0;
    }
    else
    {
        // zero or more bytes.  null-terminate and send back
        rxbuf[rx_len] = '\0';
        return rx_len;
    }
}
