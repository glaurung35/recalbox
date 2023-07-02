################################################################################
#
# Arcade dats processor
#
################################################################################

ARCADE_DATS_VERSION = 255
ARCADE_DATS_SOURCE =
ARCADE_DATS_SITE =
ARCADE_DATS_LICENSE = MAME

ARCADE_DATS_PRECOMPILED_DIR = $(ARCADE_DATS_DIR)/precompiled

# Parameters:
# $(1) emulator name (libretro, advancemame, ...)
# $(2) core name (mame2003, advancemame, ...)
# $(3) package version
# Example: $(call InstallArcadeFiles,libretro,mame2000,$(LIBRETRO_MAME2000_VERSION))
define InstallArcadeFiles
	mkdir -p $(TARGET_DIR)/recalbox/system/arcade/dats/$(1)
	mkdir -p $(TARGET_DIR)/recalbox/system/arcade/flats
	cp '$(ARCADE_DATS_PRECOMPILED_DIR)/$(2)-$(3).dat' $(TARGET_DIR)/recalbox/system/arcade/dats/$(1)/$(2).dat
	cp '$(ARCADE_DATS_PRECOMPILED_DIR)/$(2)-$(3).lst' $(TARGET_DIR)/recalbox/system/arcade/flats/$(2).lst
	cp '$(ARCADE_DATS_PRECOMPILED_DIR)/$(2)-$(3).fdt' $(TARGET_DIR)/recalbox/system/arcade/flats/$(2).fdt
endef

define ARCADE_DATS_EXTRACT_CMDS
endef

define ARCADE_DATS_BUILD_CMDS
	cp -r $(ARCADE_DATS_PKGDIR)/precompiled $(ARCADE_DATS_DIR)/
endef

define ARCADE_DATS_INSTALL_TARGET_CMDS
endef

$(eval $(generic-package))
