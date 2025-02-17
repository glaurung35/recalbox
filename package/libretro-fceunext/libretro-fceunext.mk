################################################################################
#
# FCEUNEXT
#
################################################################################

# Commit of 2023/01/09
LIBRETRO_FCEUNEXT_VERSION = 204a5a1bba98dadea6ee3eb28cf74fa25089c161
LIBRETRO_FCEUNEXT_SITE = $(call github,libretro,fceu-next,$(LIBRETRO_FCEUNEXT_VERSION))
LIBRETRO_FCEUNEXT_LICENSE = GPL-2.0
LIBRETRO_FCEUNEXT_LICENSE_FILES = fceumm-code/Copying

define LIBRETRO_FCEUNEXT_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/fceumm-code/Makefile.libretro
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/fceumm-code -f Makefile.libretro platform="$(RETROARCH_LIBRETRO_PLATFORM)"
endef

define LIBRETRO_FCEUNEXT_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/fceumm-code/fceumm_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/fceunext_libretro.so
endef

$(eval $(generic-package))
