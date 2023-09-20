################################################################################
#
# SAMEBOY
#
################################################################################

# Commit of 2023/04/19
LIBRETRO_SAMEBOY_VERSION = 09138330990da32362246c7034cf4de2ea0a2a2b
LIBRETRO_SAMEBOY_SITE = $(call github,libretro,SameBoy,$(LIBRETRO_SAMEBOY_VERSION))
LIBRETRO_SAMEBOY_LICENSE = MIT
LIBRETRO_SAMEBOY_LICENSE_FILES = LICENSE

define LIBRETRO_SAMEBOY_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/libretro/Makefile
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/libretro -f Makefile platform="unix"
endef

define LIBRETRO_SAMEBOY_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/libretro/sameboy_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/sameboy_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/gb
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/gbc
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/sgb
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/gba
	$(INSTALL) -D $(@D)/BootROMs/prebuilt/dmg_boot.bin $(TARGET_DIR)/recalbox/share_upgrade/bios/gb
	$(INSTALL) -D $(@D)/BootROMs/prebuilt/cgb_boot.bin $(TARGET_DIR)/recalbox/share_upgrade/bios/gbc
	$(INSTALL) -D $(@D)/BootROMs/prebuilt/sgb_boot.bin $(TARGET_DIR)/recalbox/share_upgrade/bios/sgb
	$(INSTALL) -D $(@D)/BootROMs/prebuilt/sgb2_boot.bin $(TARGET_DIR)/recalbox/share_upgrade/bios/sgb
	$(INSTALL) -D $(@D)/BootROMs/prebuilt/agb_boot.bin $(TARGET_DIR)/recalbox/share_upgrade/bios/gba
endef

$(eval $(generic-package))
