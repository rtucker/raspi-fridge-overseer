# vim: noet

CC=clang
CFLAGS=-c -Wall -MMD -MP
LDFLAGS=
RM=rm -f

OVERSEER_SOURCES=iface_lcd.c iface_uart.c iface_w1_gpio.c overseer.c
OVERSEER_OBJECTS=$(OVERSEER_SOURCES:.c=.o)
OVERSEER_DEPS=$(OVERSEER_OBJECTS:.o=.d)
OVERSEER_BIN=overseer

SET_BACKLIGHT_SOURCES=iface_lcd.c iface_uart.c set_backlight.c
SET_BACKLIGHT_OBJECTS=$(SET_BACKLIGHT_SOURCES:.c=.o)
SET_BACKLIGHT_DEPS=$(SET_BACKLIGHT_OBJECTS:.o=.d)
SET_BACKLIGHT_BIN=set_backlight

all: overseer set_backlight

$(OVERSEER_BIN): $(OVERSEER_OBJECTS)
	$(CC) $(LDFLAGS) $(OVERSEER_OBJECTS) -o $@

$(SET_BACKLIGHT_BIN): $(SET_BACKLIGHT_OBJECTS)
	$(CC) $(LDFLAGS) $(SET_BACKLIGHT_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: clean_test clean_overseer clean_set_backlight

clean_overseer:
	$(RM) $(OVERSEER_OBJECTS) $(OVERSEER_DEPS) $(OVERSEER_BIN)

clean_set_backlight:
	$(RM) $(SET_BACKLIGHT_OBJECTS) $(SET_BACKLIGHT_DEPS) $(SET_BACKLIGHT_BIN)

test: test_w1_gpio clean_test
clean_test: clean_test_w1_gpio

test_w1_gpio: iface_w1_gpio.o test_iface_w1_gpio.o
	$(CC) $(LDFLAGS) iface_w1_gpio.o test_iface_w1_gpio.o -o test_iface_w1_gpio
	./test_iface_w1_gpio

clean_test_w1_gpio:
	$(RM) test_iface_w1_gpio test_iface_w1_gpio.o

analyze:
	$(CC) --analyze $(OVERSEER_SOURCES) $(SET_BACKLIGHT_SOURCES)

# Handle dependency files
-include $(OVERSEER_DEPS)
-include $(SET_BACKLIGHT_DEPS)

