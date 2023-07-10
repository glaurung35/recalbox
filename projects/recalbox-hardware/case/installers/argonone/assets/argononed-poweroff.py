#!/usr/bin/python
import sys
import os
from smbus import SMBus
import RPi.GPIO as GPIO
rev = GPIO.RPI_REVISION

if rev == 2 or rev == 3:
    bus = SMBus(1)
else:
    bus = SMBus(0)

if len(sys.argv)>1:
    bus.write_byte(0x1a,0)
    if sys.argv[1] == "poweroff" or sys.argv[1] == "halt":
        try:
            bus.write_byte(0x1a,0xFF)
        except:
            rev=0
