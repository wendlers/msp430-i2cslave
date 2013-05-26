##
# Make the srf05web binary
##

CC=mips-openwrt-linux-gcc
CFLAGS=-Wall -Werror 
LDFLAGS=-lpthread -ldl

all: i2cblink i2cspeed

i2cblink: blink.c
	$(CC) $(CFLAGS) blink.c $(LDFLAGS) -o i2cblink

i2cspeed: speed.c
	$(CC) $(CFLAGS) speed.c $(LDFLAGS) -o i2cspeed

clean:
	rm i2cblink
	rm i2cspeed
