################################################################################
#
# RECALBOX_RGB_JAMMA
#
################################################################################

RECALBOX_RGB_JAMMA_VERSION = custom
RECALBOX_RGB_JAMMA_SITE = $(TOPDIR)/../projects/recalbox-rgb-jamma
RECALBOX_RGB_JAMMA_SITE_METHOD = local
RECALBOX_RGB_JAMMA_LICENSE = GPL
RECALBOX_RGB_JAMMA_DEPENDENCIES = linux recalbox-rgb-dual

define RECALBOX_RGB_JAMMA_BUILD_CMDS
	$(HOST_DIR)/bin/linux-dtc $(@D)/recalboxrgbjamma-overlay.dts -o $(@D)/recalboxrgbjamma.dtbo
	$(HOST_DIR)/bin/linux-dtc $(@D)/recalboxrgbjammapoll-overlay.dts -o $(@D)/recalboxrgbjammapoll.dtbo
endef

define RECALBOX_RGB_JAMMA_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0644 $(@D)/recalboxrgbjamma.dtbo $(BINARIES_DIR)/rpi-firmware/overlays/recalboxrgbjamma.dtbo
	$(INSTALL) -D -m 0644 $(@D)/recalboxrgbjammapoll.dtbo $(BINARIES_DIR)/rpi-firmware/overlays/recalboxrgbjammapoll.dtbo
	$(INSTALL) -D -m 0744 $(RECALBOX_RGB_JAMMA_PKGDIR)/S03rrgbjamma $(TARGET_DIR)/etc/init.d/
endef

$(eval $(kernel-module))
$(eval $(generic-package))
