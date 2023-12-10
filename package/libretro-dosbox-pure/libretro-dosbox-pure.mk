################################################################################
#
# DOSBOX_PURE
#
################################################################################

# Commit of 2023/12/29
LIBRETRO_DOSBOX_PURE_VERSION = 87bf6365158325b76ff238c1ad8daf16a859bbe8
LIBRETRO_DOSBOX_PURE_SITE = $(call github,schellingb,dosbox-pure,$(LIBRETRO_DOSBOX_PURE_VERSION))
LIBRETRO_DOSBOX_PURE_LICENSE = GPL-2.0
LIBRETRO_DOSBOX_PURE_LICENSE_FILES = LICENSE

define LIBRETRO_DOSBOX_PURE_BUILD_CMDS
	$(SED) "s|-O2|-O3|g" $(@D)/Makefile
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" -C $(@D)/ -f Makefile platform="unix"
endef

define LIBRETRO_DOSBOX_PURE_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/dosbox_pure_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/dosbox_pure_libretro.so
endef

$(eval $(generic-package))
