# vim: noet

CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
RM=rm -f

OVERSEER_SOURCES=iface_lcd.c iface_uart.c iface_w1_gpio.c overseer.c
OVERSEER_OBJECTS=$(OVERSEER_SOURCES:.c=.o)
SET_BACKLIGHT_SOURCES=iface_lcd.c iface_uart.c set_backlight.c
SET_BACKLIGHT_OBJECTS=$(SET_BACKLIGHT_SOURCES:.c=.o)

all: overseer set_backlight
    
overseer: $(OVERSEER_OBJECTS) 
	$(CC) $(LDFLAGS) $(OVERSEER_OBJECTS) -o $@

set_backlight: $(SET_BACKLIGHT_OBJECTS) 
	$(CC) $(LDFLAGS) $(SET_BACKLIGHT_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: clean_test clean_overseer clean_set_backlight

clean_overseer:
	$(RM) $(OVERSEER_OBJECTS) overseer

clean_set_backlight:
	$(RM) $(SET_BACKLIGHT_OBJECTS) set_backlight

test: test_w1_gpio clean_test
clean_test: clean_test_w1_gpio

test_w1_gpio: iface_w1_gpio.o test_iface_w1_gpio.o
	$(CC) $(LDFLAGS) iface_w1_gpio.o test_iface_w1_gpio.o -o test_iface_w1_gpio
	./test_iface_w1_gpio

clean_test_w1_gpio:
	$(RM) test_iface_w1_gpio test_iface_w1_gpio.o
