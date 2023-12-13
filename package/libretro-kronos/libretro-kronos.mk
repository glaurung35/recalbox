################################################################################
#
# KRONOS
#
################################################################################

LIBRETRO_KRONOS_VERSION = 607dc9cf4de0f23edf209dab40327d0e74f059b0
LIBRETRO_KRONOS_SITE = $(call github,FCare,Kronos,$(LIBRETRO_KRONOS_VERSION))
LIBRETRO_KRONOS_LICENSE = GPL-2.0
LIBRETRO_KRONOS_SUBDIR = yabause


#ifeq ($(BR2_PACKAGE_HAS_LIBGLES),y)
#  	LIBRETRO_KRONOS_GL = GLESv2
#else
#  	LIBRETRO_KRONOS_GL = GL
#endif
#
#LIBRETRO_KRONOS_SUPP_OPT += -DYAB_PORTS=sdl
#
#define LIBRETRO_KRONOS_BUILD_CMDS
#	$(SED) "s|-O2|-O3|g" $(@D)/yabause/src/libretro/Makefile
#	$(SED) "s|-lGL|-l$(LIBRETRO_KRONOS_GL)|g" $(@D)/yabause/src/libretro/Makefile
#	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_SO)" \
#		CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_SO)" \
#		LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_SO)" \
#		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" AR="$(TARGET_AR)" LD="$(TARGET_LD)" RANLIB="$(TARGET_RANLIB)" -C $(@D)/yabause/src/libretro -f Makefile platform="unix-$(RETROARCH_LIBRETRO_PLATFORM)" $(LIBRETRO_KRONOS_SUPP_OPT)
#endef
#
#define LIBRETRO_KRONOS_INSTALL_TARGET_CMDS
#	$(INSTALL) -D $(@D)/yabause/src/libretro/kronos_libretro.so \
#		$(TARGET_DIR)/usr/lib/libretro/kronos_libretro.so
#	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/saturn
#endef
LIBRETRO_KRONOS_CONF_OPTS += -DKRONOS_LIBRETRO_CORE=1
LIBRETRO_KRONOS_CONF_OPTS += -DYAB_PORTS=linux -DYAB_FORCE_GLES31=1

$(eval $(cmake-package))
