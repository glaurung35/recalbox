################################################################################
#
# GENESIS-PLUS-GX-WIDE
#
################################################################################

# Commit of 2023/09/16
LIBRETRO_GENESISPLUSGXWIDE_VERSION = f73cf1b0769127cee137a559f1c132b24c147923
LIBRETRO_GENESISPLUSGXWIDE_SITE = $(call github,libretro,Genesis-Plus-GX-Wide,$(LIBRETRO_GENESISPLUSGXWIDE_VERSION))
LIBRETRO_GENESISPLUSGXWIDE_LICENSE = COPYRIGHT
LIBRETRO_GENESISPLUSGXWIDE_LICENSE_FILES = LICENSE.txt
LIBRETRO_GENESISPLUSGXWIDE_NON_COMMERCIAL = y

define LIBRETRO_GENESISPLUSGXWIDE_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile.libretro
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D) -f Makefile.libretro platform="$(RETROARCH_LIBRETRO_BOARD)"
endef

define LIBRETRO_GENESISPLUSGXWIDE_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/genesis_plus_gx_wide_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/genesisplusgxwide_libretro.so
endef

$(eval $(generic-package))
