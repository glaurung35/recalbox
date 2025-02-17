################################################################################
#
# XRICK
#
################################################################################

# Commit of 2023/05/28
LIBRETRO_XRICK_VERSION = c433c1080409fefcfb69d66dc6416967ff5f6735
LIBRETRO_XRICK_SITE = $(call github,libretro,xrick-libretro,$(LIBRETRO_XRICK_VERSION))
LIBRETRO_XRICK_DEPENDENCIES = recalbox-romfs2
LIBRETRO_XRICK_LICENSE = GPL-3.0

define LIBRETRO_XRICK_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile.libretro
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/ -f Makefile.libretro platform="unix"
endef

define LIBRETRO_XRICK_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/xrick_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/xrick_libretro.so
	mkdir -p "$(TARGET_DIR)/recalbox/share_init/roms/ports/Rick Dangerous"
	$(INSTALL) -D $(@D)/data.zip "$(TARGET_DIR)/recalbox/share_init/roms/ports/Rick Dangerous/Rick Dangerous.zip"
endef

$(eval $(generic-package))
