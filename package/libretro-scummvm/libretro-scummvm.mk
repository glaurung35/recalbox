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
LIBRETRO_SCUMMVM_DEPENDENCIES = sdl2 sdl2_net zlib jpeg-turbo libmpeg2 libogg libvorbis flac libmad libpng libtheora faad2 fluidsynth freetype

ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_RPI4_64),y)
LIBRETRO_SCUMMVM_PLATFORM=rpi4_64
LIBRETRO_SCUMMVM_OPTS += FORCE_OPENGLES2=1
else ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_RPI5_64),y)
LIBRETRO_SCUMMVM_PLATFORM=rpi5_64
LIBRETRO_SCUMMVM_OPTS += FORCE_OPENGLES2=1
else ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_RPI3),y)
LIBRETRO_SCUMMVM_PLATFORM=rpi3
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
		$(MAKE) all CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/backends/platform/libretro -f Makefile platform="$(LIBRETRO_SCUMMVM_PLATFORM)" $(LIBRETRO_SCUMMVM_OPTS)
endef

define LIBRETRO_SCUMMVM_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/backends/platform/libretro/scummvm_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/scummvm_libretro.so
endef

define LIBRETRO_SCUMMVM_INSTALL_DATAFILES
	mkdir -p $(TARGET_DIR)/recalbox/share_init/bios/scummvm
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/scummvm
	unzip -j -o $(@D)/backends/platform/libretro/scummvm.zip \
		-d $(TARGET_DIR)/recalbox/share_init/bios/scummvm \
		scummvm/extra/*
	unzip -j -o $(@D)/backends/platform/libretro/scummvm.zip \
		-d $(TARGET_DIR)/recalbox/share_upgrade/bios/scummvm \
		scummvm/extra/*
endef

define LIBRETRO_SCUMMVM_INSTALL_THEMES
	mkdir -p $(TARGET_DIR)/usr/share/libretro-scummvm/theme
	unzip -j -o $(@D)/backends/platform/libretro/scummvm.zip \
		-d $(TARGET_DIR)/usr/share/libretro-scummvm/theme \
		scummvm/theme/*

endef

LIBRETRO_SCUMMVM_POST_INSTALL_TARGET_HOOKS += LIBRETRO_SCUMMVM_INSTALL_DATAFILES
LIBRETRO_SCUMMVM_POST_INSTALL_TARGET_HOOKS += LIBRETRO_SCUMMVM_INSTALL_THEMES

$(eval $(generic-package))
