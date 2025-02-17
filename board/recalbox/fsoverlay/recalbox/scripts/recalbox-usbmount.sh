#!/bin/bash

# the aim of this script
# is to make things happend in this way :
# udev
# mounting /recalbox/share
# mounting other devices
#
# because:
# 1) there are 2 drivers for ntfs ; not the 2 are possible at the same time
# 2) for ntfs, if the device is not correctly removed, we've to try to fix (ntfsfix) or fallback in case of error, and the device must not be mounted
# 3) for some others actions, such as /dev/mmcblk0p3 fs growing, it must not be mounted

if [ "$ACTION" = add ] && mount | grep -q -E "^$DEVNAME\b"; then
    /usr/bin/recallog -s usbmount -t WARNING "$DEVNAME already mounted, skipping..."
    exit 1
fi

# if share is already mounted, just use the basic usbmount
if test -e /var/run/recalbox.share.mounted
then
    /usr/bin/recallog -s usbmount -t INFO "$DEVNAME processed"
    /usr/share/usbmount/usbmount "$1"
    exit $?
fi

# if not, delay the automounting by saving the context for later
# it will be played by the S11share script after the mounting of /recalbox/share
if mkdir -p /var/run/usbmount.delay
then
  set |
	  grep -E '^DEVNAME=|^ID_FS_USAGE=|^ID_FS_UUID=|^ID_FS_TYPE=|^ID_FS_LABEL=|^ACTION=' |
	  sed -e s+'^'+'export '+ > /var/run/usbmount.delay/$(basename "$DEVNAME")
  /usr/bin/recallog -s usbmount -t INFO "$DEVNAME saved for future mount"
fi

