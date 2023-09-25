################################################################################
#
# dpi-pizero-gpicase
#
################################################################################

DPI_PIZERO_GPICASE_VERSION = ac63baf1f394913aa39c0ce216d85c58ff6a143c
DPI_PIZERO_GPICASE_SITE = $(call github,ian57,dpi-pizero-gpicase,$(DPI_PIZERO_GPICASE_VERSION))
DPI_PIZERO_GPICASE_LICENSE = GPL-2.0
DPI_PIZERO_GPICASE_LICENSE_FILES = LICENSE

DPI_PIZERO_GPICASE_DEPENDENCIES += linux

define DPI_PIZERO_GPICASE_BUILD_CMDS
	$(HOST_DIR)/bin/linux-dtc $(@D)/dpi-pizero-gpicase.dts -o $(@D)/dpi-pizero-gpicase.dtbo
endef

define DPI_PIZERO_GPICASE_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0644 $(@D)/dpi-pizero-gpicase.dtbo $(BINARIES_DIR)/boot-data/overlays/dpi-pizero-gpicase.dtbo
endef

$(eval $(generic-package))
