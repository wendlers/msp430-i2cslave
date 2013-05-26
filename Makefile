##
# Simple toplevel Makefile to build MSP430 I2C slave example 
#
# Stefan Wendler, sw@kaltpost.de 
##

all: build 

build: build-master build-slave 

build-master:
	make -C ./master/src

build-slave:
	make -C ./slave/src

deploy: build
	mkdir -p ./deploy
	cp ./master/src/i2c* ./deploy/.
	cp ./slave/src/firmware ./deploy/.

clean: clean-master clean-slave
	test -d ./deploy && rm -fr ./deploy

clean-master:
	make -C ./master/src clean

clean-slave:
	make -C ./slave/src clean
