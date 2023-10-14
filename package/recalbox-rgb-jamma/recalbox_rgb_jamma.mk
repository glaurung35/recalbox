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
	$(HOST_DIR)/bin/linux-dtc $(@D)/recalboxrgbjamma-pi5-overlay.dts -o $(@D)/recalboxrgbjamma-pi5.dtbo
	$(HOST_DIR)/bin/linux-dtc $(@D)/recalboxrgbjamma-v2-overlay.dts -o $(@D)/recalboxrgbjamma-v2.dtbo
	$(HOST_DIR)/bin/linux-dtc $(@D)/recalboxrgbjamma-v3-overlay.dts -o $(@D)/recalboxrgbjamma-v3.dtbo
endef

define RECALBOX_RGB_JAMMA_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0644 $(@D)/recalboxrgbjamma.dtbo $(BINARIES_DIR)/rpi-firmware/overlays/recalboxrgbjamma.dtbo
	$(INSTALL) -D -m 0644 $(@D)/recalboxrgbjamma-pi5.dtbo $(BINARIES_DIR)/rpi-firmware/overlays/recalboxrgbjamma-pi5.dtbo
	$(INSTALL) -D -m 0644 $(@D)/recalboxrgbjamma-v2.dtbo $(BINARIES_DIR)/rpi-firmware/overlays/recalboxrgbjamma-v2.dtbo
	$(INSTALL) -D -m 0644 $(@D)/recalboxrgbjamma-v3.dtbo $(BINARIES_DIR)/rpi-firmware/overlays/recalboxrgbjamma-v3.dtbo
	$(INSTALL) -D -m 0744 $(RECALBOX_RGB_JAMMA_PKGDIR)/S03rrgbjamma $(TARGET_DIR)/etc/init.d/
endef

$(eval $(kernel-module))
$(eval $(generic-package))
