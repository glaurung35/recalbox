################################################################################
#
# LIBRETRO-SCUMMVM
#
################################################################################

# Release version 2.9.0 - Commit version of 04/24/2024
LIBRETRO_SCUMMVM_VERSION = 65184d8aa28b403c492d9f2e338473a25c66a99b
LIBRETRO_SCUMMVM_SITE = $(call github,libretro,scummvm,$(LIBRETRO_SCUMMVM_VERSION))
LIBRETRO_SCUMMVM_LICENSE = GPL2
LIBRETRO_SCUMMVM_LICENSE_FILES = COPYING

ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_RPI4_64),y)
LIBRETRO_SCUMMVM_OPTS += FORCE_OPENGLES2=1
else ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_ODROIDXU4),y)
LIBRETRO_SCUMMVM_PLATFORM=armv cortexa9 neon hardfloat
else ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_ODROIDGO2),y)
LIBRETRO_SCUMMVM_PLATFORM=odroidgo2
else ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_RG353X),y)
LIBRETRO_SCUMMVM_PLATFORM=rg353x
else
LIBRETRO_SCUMMVM_PLATFORM = $(RETROARCH_LIBRETRO_PLATFORM)
endif

ifeq ($(BR2_ARCH_IS_64),y)
LIBRETRO_SCUMMVM_OPTS += BUILD_64BIT=1
endif

LIBRETRO_SCUMMVM_OPTS += USE_MT32EMU=1

define LIBRETRO_SCUMMVM_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/backends/platform/libretro/Makefile
	CFLAGS="$(TARGET_CFLAGS) $(LIBRETRO_SCUMMVM_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/backends/platform/libretro -f Makefile platform="$(LIBRETRO_SCUMMVM_PLATFORM)" $(LIBRETRO_SCUMMVM_OPTS)
endef

define LIBRETRO_SCUMMVM_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/backends/platform/libretro/scummvm_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/scummvm_libretro.so
endef

$(eval $(generic-package))
