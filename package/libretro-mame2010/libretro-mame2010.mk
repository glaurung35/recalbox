################################################################################
#
# MAME2010
#
################################################################################

# Commit of 2022/06/14 - don't forget to regenerate arcade-dats flats file
LIBRETRO_MAME2010_VERSION = 5f524dd5fca63ec1dcf5cca63885286109937587
LIBRETRO_MAME2010_SITE = $(call github,libretro,mame2010-libretro,$(LIBRETRO_MAME2010_VERSION))
LIBRETRO_MAME2010_LICENSE = MAME
LIBRETRO_MAME2010_NON_COMMERCIAL = y

LIBRETRO_MAME2010_DEPENDENCIES = zlib arcade-dats

ifeq ($(BR2_arm),y)
LIBRETRO_MAME2010_BUILD_OPTS=VRENDER=soft PTR64=0 ARM_ENABLED=1
else ifeq ($(BR2_aarch64),y)
LIBRETRO_MAME2010_BUILD_OPTS=VRENDER=soft PTR64=1 ARM_ENABLED=1
else ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_X86_64),y)
LIBRETRO_MAME2010_BUILD_OPTS=VRENDER=soft PTR64=1 ARM_ENABLED=0
else
LIBRETRO_MAME2010_BUILD_OPTS=VRENDER=soft PTR64=0 ARM_ENABLED=0
endif

define LIBRETRO_MAME2010_BUILD_CMDS
	$(SED) "s|^CONLYFLAGS =|CONLYFLAGS = $(COMPILER_COMMONS_CFLAGS_SO)|g" $(@D)/Makefile
	$(SED) "s|^CPPONLYFLAGS =|CPPONLYFLAGS = $(COMPILER_COMMONS_CXXFLAGS_SO)|g" $(@D)/Makefile
	$(SED) "s|^LDFLAGS =|LDFLAGS = $(COMPILER_COMMONS_LDFLAGS_SO)|g" $(@D)/Makefile
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile
	$(MAKE) CC="$(TARGET_CC)" CCX="$(TARGET_CXX)" -C $(@D)/ -f Makefile platform="$(RETROARCH_LIBRETRO_PLATFORM)" $(LIBRETRO_MAME2010_BUILD_OPTS)
endef

define LIBRETRO_MAME2010_INSTALL_TARGET_CMDS
	$(call InstallArcadeFiles,libretro,mame2010,$(LIBRETRO_MAME2010_VERSION))
	$(INSTALL) -D $(@D)/mame2010_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/mame2010_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2010/samples
	cp -R $(@D)/metadata/* $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2010
	rm $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2010/mame2010.xml
	rm $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2010/README.txt
	rm $(TARGET_DIR)/recalbox/share_upgrade/bios/mame2010/mameini.boilerplate
endef

$(eval $(generic-package))
