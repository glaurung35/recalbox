################################################################################
#
# BSNES
#
################################################################################

# Commit of 2023/09/29
LIBRETRO_BSNES_VERSION = 39c6b38c2f96881ddbfd98ad24cc9560e65b12c1
LIBRETRO_BSNES_SITE = $(call github,libretro,bsnes-libretro,$(LIBRETRO_BSNES_VERSION))
LIBRETRO_BSNES_LICENSE = COPYRIGHT
LIBRETRO_BSNES_LICENSE_FILES = LICENSE
LIBRETRO_BSNES_NON_COMMERCIAL = y

define LIBRETRO_BSNES_BUILD_CMDS
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/ -f Makefile platform="$(RETROARCH_LIBRETRO_PLATFORM)"
endef

define LIBRETRO_BSNES_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/bsnes_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/bsnes_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/sgb/
endef

$(eval $(generic-package))
