#!/bin/bash
mount -o remount, rw /
cp asound.conf /etc/
cp 10-picade.rules /etc/udev/rules.d/
