#!/bin/sh

##
# Probe with OLS (pin 0->SDA, 1->SCL)
##

DRIVER=ols:conn=/dev/ttyACM1

sigrok-cli --driver $DRIVER --config samplerate=1m --probes 0,1 --time 1s -P i2c:sda=0:scl=1
