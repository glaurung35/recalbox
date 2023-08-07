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
LIBRETRO_FLYCAST_CONF_OPTS += -DCMAKE_CXX_FLAGS="$(COMPILER_COMMONS_CXXFLAGS_EXE)"

ifeq ($(BR2_PACKAGE_RECALBOX_HAS_VULKAN),y)
# vulkan support is enabled by default
LIBRETRO_FLYCAST_DEPENDENCIES += vulkan-headers
endif

ifeq ($(BR2_PACKAGE_RECALBOX_HAS_LIBGLES),y)
LIBRETRO_FLYCAST_DEPENDENCIES += libgles
LIBRETRO_FLYCAST_CONF_OPTS += -DUSE_GLES=ON
endif

define LIBRETRO_FLYCAST_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/flycast_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/flycast_libretro.so
endef

$(eval $(cmake-package))
