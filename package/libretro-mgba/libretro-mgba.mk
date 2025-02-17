################################################################################
#
# MGBA
#
################################################################################

# Commit of 2023/05/28
LIBRETRO_MGBA_VERSION = 314bf7b676f5b820f396209eb0c7d6fbe8103486
LIBRETRO_MGBA_SITE = $(call github,libretro,mgba,$(LIBRETRO_MGBA_VERSION))
LIBRETRO_MGBA_LICENSE = MPL-2.0
LIBRETRO_MGBA_LICENSE_FILES = LICENSE

ifeq ($(BR2_ARM_CPU_HAS_NEON),y)
LIBRETRO_MGBA_NEON += "HAVE_NEON=1"
else
LIBRETRO_MGBA_NEON += "HAVE_NEON=0"
endif

ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_RPI3)$(BR2_PACKAGE_RECALBOX_TARGET_RPIZERO2LEGACY)$(BR2_PACKAGE_RECALBOX_TARGET_RPIZERO2),y)
LIBRETRO_MGBA_PLATFORM=rpi3
else
LIBRETRO_MGBA_PLATFORM=$(RETROARCH_LIBRETRO_PLATFORM)
endif

define LIBRETRO_MGBA_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile.libretro
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/ -f Makefile.libretro platform="$(LIBRETRO_MGBA_PLATFORM)" $(LIBRETRO_MGBA_NEON)
endef

define LIBRETRO_MGBA_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/mgba_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/mgba_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/gb
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/gbc
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/sgb
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/gba
endef

$(eval $(generic-package))
