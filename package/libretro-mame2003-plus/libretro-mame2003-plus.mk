################################################################################
#
# MAME2003_PLUS
#
################################################################################

# Commit of 2023/08/05 - don't forget to regenerate arcade-dats flats file
LIBRETRO_MAME2003_PLUS_VERSION = 4406607dcab8d9ee6f332a06de94d37d97cd2d34
LIBRETRO_MAME2003_PLUS_SITE = $(call github,libretro,mame2003-plus-libretro,$(LIBRETRO_MAME2003_PLUS_VERSION))
LIBRETRO_MAME2003_PLUS_LICENSE = MAME
LIBRETRO_MAME2003_PLUS_NON_COMMERCIAL = y

LIBRETRO_MAME2003_PLUS_DEPENDENCIES = arcade-dats

define LIBRETRO_MAME2003_PLUS_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile
	mkdir -p $(@D)/obj/mame/cpu/ccpu
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CC="$(TARGET_CC)" -C $(@D)/ -f Makefile platform="$(RETROARCH_LIBRETRO_BOARD)"
endef

define LIBRETRO_MAME2003_PLUS_INSTALL_TARGET_CMDS
	$(call InstallArcadeFiles,libretro,mame2003-plus,$(LIBRETRO_MAME2003_PLUS_VERSION))
	$(INSTALL) -D $(@D)/mame2003_plus_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/mame2003_plus_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2003-plus/samples
	cp -R $(@D)/metadata/* $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2003-plus
	rm $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2003-plus/mame2003-plus.xml
endef

$(eval $(generic-package))
