################################################################################
#
# MAME2000 / IMAME
#
################################################################################

# Commit of 2023/04/25
LIBRETRO_MAME2000_VERSION = 720b8ad4cbd76abd57b9aeced9ba541dc8476f7f
LIBRETRO_MAME2000_SITE = $(call github,libretro,mame2000-libretro,$(LIBRETRO_MAME2000_VERSION))
LIBRETRO_MAME2000_LICENSE = MAME
LIBRETRO_MAME2000_NON_COMMERCIAL = y

LIBRETRO_MAME2000_DEPENDENCIES = arcade-dats

define LIBRETRO_MAME2000_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile
	mkdir -p $(@D)/obj_libretro_libretro/cpu
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CC="$(TARGET_CC)" -C $(@D) -f Makefile ARM=1
endef

define LIBRETRO_MAME2000_INSTALL_TARGET_CMDS
	$(call InstallArcadeFiles,libretro,mame2000,$(LIBRETRO_MAME2000_VERSION))
	$(INSTALL) -D $(@D)/mame2000_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/mame2000_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2000/samples
	cp -R $(@D)/metadata/* $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2000
	rm '$(TARGET_DIR)/recalbox/share_upgrade/bios/mame2000/MAME 0.37b5 XML.dat'
endef

$(eval $(generic-package))
