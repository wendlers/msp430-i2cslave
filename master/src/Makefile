##
# Make the srf05web binary
##

CC=mips-openwrt-linux-gcc
CFLAGS=-Wall -Werror 
LDFLAGS=-lpthread -ldl

all: i2cmaster 

i2cmaster: main.c
	$(CC) $(CFLAGS) main.c $(LDFLAGS) -o i2cmaster 

clean:
	rm i2cmaster
