# vim: noet

CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=iface_lcd.c iface_uart.c iface_w1_gpio.c overseer.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=overseer
RM=rm -f

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: clean_test
	$(RM) $(OBJECTS) $(EXECUTABLE)

test: test_w1_gpio clean_test
clean_test: clean_test_w1_gpio

test_w1_gpio: iface_w1_gpio.o test_iface_w1_gpio.o
	$(CC) $(LDFLAGS) iface_w1_gpio.o test_iface_w1_gpio.o -o test_iface_w1_gpio
	./test_iface_w1_gpio

clean_test_w1_gpio:
	$(RM) test_iface_w1_gpio test_iface_w1_gpio.o
