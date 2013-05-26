/*
 * This file is part of the MSP430 USCI I2C slave example.
 *
 * Copyright (C) 2012 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * This is the master part of the MSP430 I2C slave example. It toggles
 * the LED on P1.0 as fast as possible. The program could be exited by
 * pressing CTRL+C. 
 */

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>

/* I2C bus on which the MSP430 is connected */
#define I2C_DEV				"/dev/i2c-0"	

/* I2C 7-bit slave address */
#define I2C_SLAVE_ADDR		0x48

/* Commands */
#define CMD_SETLED  (unsigned char)0x00

/* Parameters */
#define PAR_LEDOFF  (unsigned char)0x00
#define PAR_LEDON   (unsigned char)0x01

/* indicates that main loop should be terminated */
int interrupted = 0;

/* signal handler */
static void signal_handler(int signal) {
	interrupted = 1;
}

int main(void) {

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    unsigned char buf1[2];
    unsigned char buf2[2];

    int fd;

    if (signal(SIGINT, signal_handler) == SIG_ERR) {
		printf("unable to register signal handler\n");
        return -1;
    }

    if ((fd = open(I2C_DEV, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
		return -1;
    }

	printf("Press CTRL-C to end program.\n");

	/* prepare message in advance */
    messages[0].addr  = I2C_SLAVE_ADDR;
    messages[0].flags = 0;
    messages[0].len   = sizeof(buf1);
    messages[0].buf   = buf1;

    buf1[0] = CMD_SETLED;
	buf1[1] = PAR_LEDON;

    messages[1].addr  = I2C_SLAVE_ADDR;
    messages[1].flags = 0;
    messages[1].len   = sizeof(buf2);
    messages[1].buf   = buf2;

    buf2[0] = CMD_SETLED;
	buf2[1] = PAR_LEDOFF;

    packets.msgs  	= messages;
    packets.nmsgs 	= 2;

	while(interrupted == 0) {
	    ioctl(fd, I2C_RDWR, &packets);
    }

    close(fd);

    return 0;
}
