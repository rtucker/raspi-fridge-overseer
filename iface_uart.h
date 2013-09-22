#ifndef __INCLUDE_IFACE_UART_H_
#define __INCLUDE_IFACE_UART_H_

#include <stddef.h>

#define SERIAL_DEVICE   "/dev/ttyAMA0"

char uartInit();
void uartClose();
char uartTx(char *txbuf, size_t len);
size_t uartRx(char *rxbuf, size_t maxlen);
#endif
