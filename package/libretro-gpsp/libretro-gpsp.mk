################################################################################
#
# GPSP
#
################################################################################

# Commit of 2023/10/06
LIBRETRO_GPSP_VERSION = eaf8b94702c5ded2fe3a3fc6a7f9718652fe8595
LIBRETRO_GPSP_SITE = $(call github,libretro,gpsp,$(LIBRETRO_GPSP_VERSION))
LIBRETRO_GPSP_LICENSE = GPL-2.0
LIBRETRO_GPSP_LICENSE_FILES = COPYING

ifeq ($(BR2_ARM_CPU_ARMV6),y)
LIBRETRO_GPSP_PLATFORM = rpi1
else ifeq ($(BR2_aarch64),y)
LIBRETRO_GPSP_PLATFORM = arm64
else
LIBRETRO_GPSP_PLATFORM = $(RETROARCH_LIBRETRO_BOARD)
endif

define LIBRETRO_GPSP_BUILD_CMDS
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO) -freorder-blocks-and-partition" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO) -freorder-blocks-and-partition" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D) platform="$(LIBRETRO_GPSP_PLATFORM)" OPTIMIZE="$(COMPILER_COMMONS_LDFLAGS_SO)"
endef

define LIBRETRO_GPSP_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/gpsp_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/gpsp_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/gba
	$(INSTALL) -D $(@D)/bios/open_gba_bios.bin \
		$(TARGET_DIR)/recalbox/share_upgrade/bios/gba
endef

$(eval $(generic-package))
