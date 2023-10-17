################################################################################
#
# XPAD_NOONE
#
################################################################################

XPAD_NOONE_VERSION = 392b5725efb6e8e78671c092d617fafbdc966153
XPAD_NOONE_SITE = $(call github,ublue-os,xpad-noone,$(XPAD_NOONE_VERSION))
XPAD_NOONE_LICENSE = GPL-2.0

$(eval $(kernel-module))
$(eval $(generic-package))
