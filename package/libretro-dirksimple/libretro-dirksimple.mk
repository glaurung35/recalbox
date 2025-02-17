################################################################################
#
# LIBRETRO DIRKSIMPLE
#
################################################################################

# Commit of 2023/08/01
LIBRETRO_DIRKSIMPLE_VERSION = 9c370ef73d11917d7252019f28541eb558a58857
LIBRETRO_DIRKSIMPLE_SITE = $(call github,icculus,dirksimple,$(LIBRETRO_DIRKSIMPLE_VERSION))
LIBRETRO_DIRKSIMPLE_DEPENDENCIES = libmpeg2 libogg libvorbis sdl2 zlib
LIBRETRO_DIRKSIMPLE_LICENSE = ZLIB
LIBRETRO_DIRKSIMPLE_LICENSE_FILES = LICENSE.TXT

LIBRETRO_DIRKSIMPLE_PLATFORM=$(RETROARCH_LIBRETRO_PLATFORM)

LIBRETRO_DIRKSIMPLE_CONF_OPTS += -DLIBRETRO=ON

define LIBRETRO_DIRKSIMPLE_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/dirksimple_libretro.so $(TARGET_DIR)/usr/lib/libretro/dirksimple_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/daphne/DirkSimple
	cp -R $(@D)/data $(TARGET_DIR)/recalbox/share_upgrade/bios/daphne/DirkSimple
endef

$(eval $(cmake-package))
