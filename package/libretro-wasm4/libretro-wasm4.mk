################################################################################
#
# WASM-4
#
################################################################################

# Commit version of 2023/03/27
LIBRETRO_WASM4_VERSION = e3002f75ef82866c056f792f92a8e2f8dedbeb2d
LIBRETRO_WASM4_SITE = https://github.com/aduros/wasm4.git
LIBRETRO_WASM4_SITE_METHOD = git
LIBRETRO_WASM4_GIT_SUBMODULES = YES
LIBRETRO_WASM4_LICENSE = ISC
LIBRETRO_WASM4_LICENSE_FILES = LICENSE

LIBRETRO_WASM4_SUBDIR=runtimes/native

LIBRETRO_WASM4_CONF_OPTS += -DCMAKE_BUILD_TYPE=Release
LIBRETRO_WASM4_CONF_OPTS += -DLIBRETRO=1
LIBRETRO_WASM4_CONF_OPTS += -DBUILD_LIBRETRO_CORE=ON

define LIBRETRO_WASM4_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/runtimes/native/wasm4_libretro.so \
		$(TARGET_DIR)/usr/lib/libretro/wasm4_libretro.so
	mkdir -p $(TARGET_DIR)/recalbox/share_upgrade/roms/wasm4
	cp -R $(@D)/site/static/carts/*.wasm $(TARGET_DIR)/recalbox/share_upgrade/roms/wasm4
endef

$(eval $(cmake-package))
