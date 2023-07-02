################################################################################
#
# SNES9X2010 / SNES9X_NEXT
#
################################################################################

LIBRETRO_SNES9X2010_VERSION = 187e2b58fc09dfeb9fdb5a95bc26786219a111cf
LIBRETRO_SNES9X2010_SITE = $(call github,libretro,snes9x2010,$(LIBRETRO_SNES9X2010_VERSION))
LIBRETRO_SNES9X2010_LICENSE = COPYRIGHT
LIBRETRO_SNES9X2010_LICENSE_FILES = LICENSE.txt
LIBRETRO_SNES9X2010_NON_COMMERCIAL = y

ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_RPI4_64),y)
LIBRETRO_SNES9X2010_BOARD = unix
else
LIBRETRO_SNES9X2010_BOARD = $(RETROARCH_LIBRETRO_BOARD)
endif

define LIBRETRO_SNES9X2010_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile.libretro
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/ -f Makefile.libretro platform="$(LIBRETRO_SNES9X2010_BOARD)"
endef

define LIBRETRO_SNES9X2010_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/snes9x2010_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/snes9x2010_libretro.so
endef

$(eval $(generic-package))
