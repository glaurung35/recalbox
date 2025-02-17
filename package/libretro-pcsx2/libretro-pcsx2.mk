################################################################################
#
# libretro-pcsx2
#
################################################################################

# Version: 2021-11-02
LIBRETRO_PCSX2_VERSION = 408e3b253847b25536ad77e4ab0c3aff6b8b896f
LIBRETRO_PCSX2_SITE = https://github.com/libretro/pcsx2.git
LIBRETRO_PCSX2_SITE_METHOD = git
LIBRETRO_PCSX2_GIT_SUBMODULES = YES
LIBRETRO_PCSX2_LICENSE = GPLv2
LIBRETRO_PCSX2_DEPENDENCIES = retroarch libaio xz host-xxd

LIBRETRO_PCSX2_CONF_OPTS += \
	-DCMAKE_BUILD_TYPE=Release \
	-DSDL2_API=ON \
	-DDISABLE_PCSX2_WRAPPER=ON \
	-DPACKAGE_MODE=OFF \
	-DBUILD_SHARED_LIBS=OFF \
	-DENABLE_TESTS=OFF \
	-DENABLE_QT=OFF \
	-DLIBRETRO=ON \
	-DDISABLE_ADVANCE_SIMD=ON \
	-DEXTRA_PLUGINS=TRUE

#LIBRETRO_PCSX2_CONF_OPTS += -DUSE_LTO=ON

define LIBRETRO_PCSX2_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/pcsx2/pcsx2_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/pcsx2_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/bios/ps2
endef

$(eval $(cmake-package))
