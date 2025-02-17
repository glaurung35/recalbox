################################################################################
#
# SWANSTATION
#
################################################################################

# Commit of 2023/10/01
LIBRETRO_SWANSTATION_VERSION = afb6bc73c7ce522190b19ec3e3c8a14eb264bdeb
LIBRETRO_SWANSTATION_SITE = $(call github,libretro,swanstation,$(LIBRETRO_SWANSTATION_VERSION))
LIBRETRO_SWANSTATION_LICENSE = GPL-3.0
LIBRETRO_SWANSTATION_LICENSE_FILES = LICENSE

ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_RPI1)$(BR2_PACKAGE_RECALBOX_TARGET_RPIZERO2LEGACY),y)
LIBRETRO_SWANSTATION_CONF_OPTS += -DEGL_INCLUDE_DIR="$(STAGING_DIR)/usr/include"
LIBRETRO_SWANSTATION_SIDE_LIBS = -lGLESv2
LIBRETRO_SWANSTATION_CONF_OPTS += -DUSE_FBDEV=ON
else
LIBRETRO_SWANSTATION_SIDE_LIBS =
LIBRETRO_SWANSTATION_CONF_OPTS += -DUSE_DRMKMS=ON
endif

ifeq ($(BR2_PACKAGE_HAS_LIBGLES),y)
LIBRETRO_SWANSTATION_DEPENDENCIES += libgles
endif

ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_X86_64),y)
LIBRETRO_SWANSTATION_CONF_OPTS += -DUSE_X11=ON
else
LIBRETRO_SWANSTATION_CONF_OPTS += -DUSE_X11=OFF
endif

ifeq ($(BR2_PACKAGE_RECALBOX_TARGET_ODROIDGO2),y)
LIBRETRO_SWANSTATION_SIDE_LIBS += -lmali
endif

LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_BUILD_TYPE=Release
LIBRETRO_SWANSTATION_CONF_OPTS += -DBUILD_SHARED_LIBS=FALSE
LIBRETRO_SWANSTATION_CONF_OPTS += -DANDROID=OFF -DENABLE_DISCORD_PRESENCE=OFF -DBUILD_LIBRETRO_CORE=ON
LIBRETRO_SWANSTATION_CONF_OPTS += -DBUILD_QT_FRONTEND=OFF -DBUILD_NOGUI_FRONTEND=OFF
LIBRETRO_SWANSTATION_CONF_OPTS += -DUSE_EVDEV=ON
LIBRETRO_SWANSTATION_CONF_OPTS += -DUSE_WAYLAND=OFF
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_C_FLAGS="-DEGL_NO_X11"

ifeq ($(BR2_PACKAGE_HAS_LIBEGL),y)
LIBRETRO_SWANSTATION_CONF_OPTS += -DUSE_EGL=ON
else
LIBRETRO_SWANSTATION_CONF_OPTS += -DUSE_EGL=OFF
endif

LIBRETRO_SWANSTATION_CONF_ENV += LDFLAGS=-lpthread

LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_C_ARCHIVE_CREATE="<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>"
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_C_ARCHIVE_FINISH=true
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_CXX_ARCHIVE_CREATE="<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>"
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_CXX_ARCHIVE_FINISH=true
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_AR="$(TARGET_CC)-ar"
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_C_COMPILER="$(TARGET_CC)"
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_CXX_COMPILER="$(TARGET_CXX)"
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_LINKER="$(TARGET_LD)"
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_C_FLAGS="$(COMPILER_COMMONS_CFLAGS_SO)"
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_CXX_FLAGS="$(COMPILER_COMMONS_CXXFLAGS_SO)"
LIBRETRO_SWANSTATION_CONF_OPTS += -DCMAKE_LINKER_EXE_FLAGS="$(COMPILER_COMMONS_LDFLAGS_SO)"

define LIBRETRO_SWANSTATION_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/swanstation_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/swanstation_libretro.so
endef

$(eval $(cmake-package))
