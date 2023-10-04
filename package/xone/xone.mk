################################################################################
#
# XONE
#
################################################################################

XONE_VERSION = bbf0dcc484c3f5611f4e375da43e0e0ef08f3d18
XONE_SITE = $(call github,medusalix,xone,$(XONE_VERSION))
XONE_LICENSE = GPL-2.0

define XONE_COPY_CMDS
	$(INSTALL) -D -m 0644 $(@D)/install/modprobe.conf $(TARGET_DIR)/etc/modprobe.d/xone-blacklist.conf
	$(INSTALL) -D -m 0644 $(XONE_PKGDIR)/assets/FW_ACC_00U.bin $(TARGET_DIR)/lib/firmware/xow_dongle.bin
endef

XONE_INSTALL_TARGET_CMDS += $(XONE_COPY_CMDS)

$(eval $(kernel-module))
$(eval $(generic-package))
