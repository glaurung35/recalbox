################################################################################
#
# XPAD_NOONE
#
################################################################################

XPAD_NOONE_VERSION = d02737f7eab1e17a7748fbe550dd982e3808525d
XPAD_NOONE_SITE = $(call github,medusalix,xpad-noone,$(XPAD_NOONE_VERSION))
XPAD_NOONE_LICENSE = GPL-2.0

$(eval $(kernel-module))
$(eval $(generic-package))
