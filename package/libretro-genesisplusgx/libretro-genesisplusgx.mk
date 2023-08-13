################################################################################
#
# GENESIS-PLUS-GX
#
################################################################################

# Commit of 2023/10/06
LIBRETRO_GENESISPLUSGX_VERSION = 8d644682b6a88d662d1a07e063fa863633a99311
LIBRETRO_GENESISPLUSGX_SITE = $(call github,libretro,Genesis-Plus-GX,$(LIBRETRO_GENESISPLUSGX_VERSION))
LIBRETRO_GENESISPLUSGX_LICENSE = COPYRIGHT
LIBRETRO_GENESISPLUSGX_LICENSE_FILES = LICENSE.txt
LIBRETRO_GENESISPLUSGX_NON_COMMERCIAL = y

define LIBRETRO_GENESISPLUSGX_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile.libretro
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D) -f Makefile.libretro platform="$(RETROARCH_LIBRETRO_BOARD)"
endef

define LIBRETRO_GENESISPLUSGX_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/genesis_plus_gx_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/genesisplusgx_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/gamegear
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/mastersystem
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/megadrive
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/segacd
endef

$(eval $(generic-package))
