################################################################################
#
# DevilutionX
#
################################################################################

DEVILUTIONX_VERSION = 1.5.3
DEVILUTIONX_SITE = $(call github,diasurgical,devilutionX,$(DEVILUTIONX_VERSION))
DEVILUTIONX_LICENSE = SUL1.0
DEVILUTIONX_LICENSE_FILES = LICENSE
DEVILUTIONX_DEPENDENCIES = sdl2 sdl2_image

DEVILUTIONX_CONF_OPTS += -DDEVILUTIONX_SYSTEM_LIBSODIUM=OFF -DDEVILUTIONX_SYSTEM_SDL_IMAGE=OFF

define DEVILUTIONX_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/recalbox/share_init/system/configs/devilutionx
	mkdir -p "$(TARGET_DIR)/recalbox/share_init/roms/ports/DevilutionX"
	$(INSTALL) -m 0755 $(@D)/devilutionx -D $(TARGET_DIR)/usr/bin
	# assets must be in same directory as binary
	cp -r $(@D)/assets $(TARGET_DIR)/usr/bin #"$(TARGET_DIR)/recalbox/share_init/roms/ports/DevilutionX"
endef

$(eval $(cmake-package))
