################################################################################
#
# hid-nintendo
#
################################################################################

HID_NX_VERSION = 974d6c407296c47390d99f008933846c86f52bb9
HID_NX_SITE = $(call github,emilyst,hid-nx-dkms,$(HID_NX_VERSION))
HID_NX_LICENSE = GPL-2.0
HID_NX_LICENSE_FILES = LICENSE

$(eval $(kernel-module))
$(eval $(generic-package))
