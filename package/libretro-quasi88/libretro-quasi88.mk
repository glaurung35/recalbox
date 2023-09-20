################################################################################
#
# QUASI88
#
################################################################################

# Commit of 2023/01/03
LIBRETRO_QUASI88_VERSION = 7980f2484997055a9665837bb4c325d562acb04e
LIBRETRO_QUASI88_SITE = $(call github,libretro,quasi88-libretro,$(LIBRETRO_QUASI88_VERSION))
LIBRETRO_QUASI88_LICENSE = BSD-3-Clause
LIBRETRO_QUASI88_LICENSE_FILES = LICENSE

define LIBRETRO_QUASI88_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D) -f Makefile platform="$(RETROARCH_LIBRETRO_PLATFORM)"
endef

define LIBRETRO_QUASI88_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/quasi88_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/quasi88_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/quasi88
endef

$(eval $(generic-package))
