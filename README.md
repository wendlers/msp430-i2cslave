MSP430G2553 I2C Slave Example
=============================
23.05.2012 Stefan Wendler
sw@kaltpost.de

This example shows how to use the MSP430G2553 as an I2C slave. The MSP430 takes some simple commands.
One for switching the build in LED (P1.0) on/off, one for reading the state of the build in button
(P1.3). Also a master example is provided. This example I run on a [modified TP-Link MR3020] (http://wiki.openwrt.org/toh/tp-link/tl-mr3020#adding.i2c.bus)
router which runs OpenWrt. At the end of this document there are also some examples on how to use a
Bus Pirate as master. 

For a quick start to setup the I2C slave, the TI USCI I2C slave
code from [slaa383] (http://www.ti.com/general/docs/litabsmultiplefilelist.tsp?literatureNumber=slaa383) 
was used and slightly modified to work on the MSP430G2553 and the msp-gcc. For details on leagal issues
regarding the TI_USCI_I2C_slave code see "ti-usci-i2c-slave-legal.txt".
 

Project Directory Layout
------------------------

The top-level directory structure of the project looks something like this:

* `Makefile` 		toplevel Make file
* `README.md`		this README
* `setenv.sh`		source to set cross-compile environment
* `master`			sample master code (e.g. to run on the TP-Link MR3020)	
* `slave`			sample slave firmware to tun on the MSP430G2553	


Prerequisites
-------------

To compile this project the following is needed: 

* OpenWrt checked out and compiled for your target (see [this instructions] (http://wiki.openwrt.org/doc/howto/buildroot.exigence) )
  - alternatively a corss-toolchain for your target
* I2C bus accessible through "/dev/i2c-x" on the master, e.g. [I2C on TP-Link MR3020] (http://wiki.openwrt.org/toh/tp-link/tl-mr3020#adding.i2c.bus)
* msp-gcc installed
* MSP430 Launchpad with MSP430G2553
  - connect P1.7 to SDA of master
  - connect P1.6 to SCL of master
  - Add 100k pull-ups btw. P1.7/P1.6 and VCC of Launchpad
  - Connect GND of master and Launchpad


Compilation
------------

**Check `setenv.sh`**

Edit `setenv.sh`, make sure `OPENWRT_DIR` points to the location of your OpenWrt base directory (or
any any other base directory of your cross-toolchain).

**Set Cross-Compile Environment**

To set the cross-compile environment use the following:

	source setenv.sh

**Compile**

To compile both, the master and the slave code:

	make

To copy the binaries to the `delpoy` subdirectory:

	make deploy

To onyl build the master:

	make build-master

To only build the slave:
	
	make build-slave

To flash the slave firmware (usign mspdebug):

	make -C slave/src flash


Installing on the Targets
-------------------------

Copy the `i2cmaster` found in `deploy` to your target (e.g. MR3020).

Flash the `firmware` to your target (MSP430):

	mspdebug rf2500 "prog deploy/firmware"


Usage
-----

On the master target run `i2cmaster`. Pressing the build in butto of the Launchpad (P1.3) 
changes the frequency at which thei red LED (P1.0) blinks. End the program with CTRL-C.

Alternatively you could connec the Launchpad to a Bus Pirate and operate the slave whith
the following commands:

Enter I2C mode at 100Mhz

	m4 3

Set LED HIGH (P1.0):

	[0x90 0x00 0x01

Set LED LOW  (P1.0)

	[0x90 0x00 0x00


Get BUTTON state (P1.3), 0 if pressed, 1 otherwise

	[0x90 0x01 [0x91 r
 
