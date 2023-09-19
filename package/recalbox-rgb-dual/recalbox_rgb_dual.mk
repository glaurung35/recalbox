################################################################################
#
# RECALBOX_RGB_DUAL
#
################################################################################

RECALBOX_RGB_DUAL_VERSION = custom
RECALBOX_RGB_DUAL_SITE = $(TOPDIR)/../projects/recalbox-rgb-dual
RECALBOX_RGB_DUAL_SITE_METHOD = local
RECALBOX_RGB_DUAL_LICENSE = GPL
RECALBOX_RGB_DUAL_DEPENDENCIES = linux

define RECALBOX_RGB_DUAL_BUILD_CMDS
	$(HOST_DIR)/bin/linux-dtc $(@D)/recalboxrgbdual-overlay.dts -o $(@D)/recalboxrgbdual.dtbo
	$(HOST_DIR)/bin/linux-dtc $(@D)/recalboxrgbdual-pi5-overlay.dts -o $(@D)/recalboxrgbdual-pi5.dtbo
	$(HOST_DIR)/bin/linux-dtc $(@D)/recalboxrgbdual-thirdparty-overlay.dts -o $(@D)/recalboxrgbdual-thirdparty.dtbo
	$(HOST_DIR)/bin/linux-dtc $(@D)/headphones-overlay.dts -o $(@D)/headphones.dtbo
endef

define RECALBOX_RGB_DUAL_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0644 $(@D)/hat_map.dtb $(BINARIES_DIR)/boot-data/overlays/hat_map.dtb
	$(INSTALL) -D -m 0644 $(@D)/recalboxrgbdual.dtbo $(BINARIES_DIR)/boot-data/overlays/recalboxrgbdual.dtbo
	$(INSTALL) -D -m 0644 $(@D)/recalboxrgbdual-pi5.dtbo $(BINARIES_DIR)/boot-data/overlays/recalboxrgbdual-pi5.dtbo
	$(INSTALL) -D -m 0644 $(@D)/recalboxrgbdual-thirdparty.dtbo $(BINARIES_DIR)/boot-data/overlays/recalboxrgbdual-thirdparty.dtbo
	$(INSTALL) -D -m 0644 $(@D)/headphones.dtbo $(BINARIES_DIR)/boot-data/overlays/headphones.dtbo
endef

$(eval $(kernel-module))
$(eval $(generic-package))
