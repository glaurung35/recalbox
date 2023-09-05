################################################################################
#
# SNES9X
#
################################################################################

# Commit of 2023/10/13
LIBRETRO_SNES9X_VERSION = ec4ebfc8f3819a9522fcb8e53eed985090017b1b
LIBRETRO_SNES9X_SITE = $(call github,libretro,snes9x,$(LIBRETRO_SNES9X_VERSION))
LIBRETRO_SNES9X_LICENSE = COPYRIGHT
LIBRETRO_SNES9X_LICENSE_FILES = LICENSE
LIBRETRO_SNES9X_NON_COMMERCIAL = y

define LIBRETRO_SNES9X_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/libretro/Makefile
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO) -std=c++11" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/libretro -f Makefile platform="$(RETROARCH_LIBRETRO_PLATFORM)"
endef

define LIBRETRO_SNES9X_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/libretro/snes9x_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/snes9x_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/satellaview
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/sufami
endef

$(eval $(generic-package))
