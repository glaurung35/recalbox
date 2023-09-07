################################################################################
#
# FLYCAST
#
################################################################################

# Commit of 31/07/2023
LIBRETRO_FLYCAST_VERSION = 92a10ba0f0a5ca0d1ec9498962002f4cdcc8e7f6
LIBRETRO_FLYCAST_SITE = http://github.com/flyinghead/flycast.git
LIBRETRO_FLYCAST_LICENSE = GPL-2.0
LIBRETRO_FLYCAST_SITE_METHOD = git
LIBRETRO_FLYCAST_GIT_SUBMODULES = y

LIBRETRO_FLYCAST_DEPENDENCIES = arcade-dats
LIBRETRO_FLYCAST_CONF_OPTS = -DLIBRETRO=ON -DBUILD_SHARED_LIBS=OFF

ifeq ($(BR2_PACKAGE_RECALBOX_HAS_VULKAN),y)
# vulkan support is enabled by default
LIBRETRO_FLYCAST_DEPENDENCIES += vulkan-headers
endif

ifeq ($(BR2_PACKAGE_RECALBOX_HAS_LIBGLES),y)
LIBRETRO_FLYCAST_DEPENDENCIES += libgles
ifeq ($(BR2_PACKAGE_RPI_USERLAND),y)
LIBRETRO_FLYCAST_CONF_OPTS += \
	-DUSE_GLES=OFF \
	-DUSE_GLES2=ON \
	-DUSE_VIDEOCORE=ON
else
LIBRETRO_FLYCAST_CONF_OPTS += -DUSE_GLES=ON
endif
endif

ifeq ($(BR2_PACKAGE_RECALBOX_HAS_LIBMALI),y)
LIBRETRO_FLYCAST_SIDE_LIBS = -lmali
endif

LIBRETRO_FLYCAST_CONF_OPTS += -DCMAKE_CXX_FLAGS="$(COMPILER_COMMONS_CXXFLAGS_EXE) $(LIBRETRO_FLYCAST_SIDE_LIBS)"

define LIBRETRO_FLYCAST_INSTALL_TARGET_CMDS
	$(call InstallArcadeFiles,libretro,naomi,$(LIBRETRO_FLYCAST_VERSION))
	$(call InstallArcadeFiles,libretro,naomi2,$(LIBRETRO_FLYCAST_VERSION))
	$(call InstallArcadeFiles,libretro,naomigd,$(LIBRETRO_FLYCAST_VERSION))
	$(call InstallArcadeFiles,libretro,atomiswave,$(LIBRETRO_FLYCAST_VERSION))
	$(INSTALL) -D $(@D)/flycast_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/flycast_libretro.so
endef

$(eval $(cmake-package))
