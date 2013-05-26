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
 * This is the master part of the MSP430 I2C slave example. It sends
 * LED ON/OFF commands to the firmware making the build in LED on 
 * the MSP430 Launchpad blink. Pressing the build in button P1.3
 * changes the blink frequency. The program could be exited by
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
#define CMD_GETBTN  (unsigned char)0x01

/* Parameters */
#define PAR_LEDOFF  (unsigned char)0x00
#define PAR_LEDON   (unsigned char)0x01

/* indicates that main loop should be terminated */
int interrupted = 0;

/* signal handler */
static void signal_handler(int signal) {
	interrupted = 1;
}

/* wirte command (CMD_*) with given parameter (PAR_*) to slave */
static int i2c_write(int fd, unsigned char cmd, unsigned char par) {

    unsigned char buf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  = I2C_SLAVE_ADDR;
    messages[0].flags = 0;
    messages[0].len   = sizeof(buf);
    messages[0].buf   = buf;

    buf[0] = cmd;
    buf[1] = par;

    packets.msgs  	= messages;
    packets.nmsgs 	= 1;

    if(ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("data could not be send");
        return -1;
    }

    return 0;
}

/* read command (CMD_*) from slave */
static int i2c_read(int fd, unsigned char cmd, unsigned char *val) {

    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    messages[0].addr  = I2C_SLAVE_ADDR;
    messages[0].flags = 0;
    messages[0].len   = 1;
    messages[0].buf   = &cmd;

    messages[1].addr  = I2C_SLAVE_ADDR;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = 1;
    messages[1].buf   = val;

    packets.msgs      = messages;
    packets.nmsgs     = 2;

    if(ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("data could not be send");
        return -1;
    }

    return 0;
}

int main(void) {

	unsigned char btn = 0;
	unsigned char led = PAR_LEDOFF;

    int fd;
	int tog_cnt = 50;
	int cnt 	= 0;

    if (signal(SIGINT, signal_handler) == SIG_ERR) {
		printf("unable to register signal handler\n");
        return -1;
    }

    if ((fd = open(I2C_DEV, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
		return -1;
    }

	printf("Press button (P1.3) to change LED blink pattern.\n");
	printf("Press CTRL-C to end program.\n");

	while(interrupted == 0) {
		
		(void)i2c_read(fd, CMD_GETBTN, &btn);

		if(btn == 0) {

			cnt = 0;

			if(tog_cnt == 50) {
				tog_cnt = 250;
			}
			else {
				tog_cnt = 50;
			}

			printf("Speed now: %d\n", tog_cnt);

			do {
				(void)i2c_read(fd, CMD_GETBTN, &btn);
			}
			while(btn == 0);
		}
		
		usleep(10000);

		if(cnt++ == tog_cnt) {
			led = ~led & 0x01;
			(void)i2c_write(fd, CMD_SETLED, led);
			cnt = 0;
		}
	}

	(void)i2c_write(fd, CMD_SETLED, PAR_LEDOFF);

    close(fd);

    return 0;
}
