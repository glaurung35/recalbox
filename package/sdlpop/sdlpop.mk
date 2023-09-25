################################################################################
#
# SDLPoP
#
################################################################################

# Commit of 2023/10/23
SDLPOP_VERSION = bb0c6f3da464106b5fc931925fcf338f6c4939b9
SDLPOP_SITE = $(call github,NagyD,SDLPoP,$(SDLPOP_VERSION))
SDLPOP_SUBDIR = src
SDLPOP_LICENSE = GPL3.0
SDLPOP_LICENSE_FILES = COPYING
SDLPOP_DEPENDENCIES = sdl2 sdl2_image

define SDLPOP_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/recalbox/share_initsystem/configs/sdlpop
	mkdir -p "$(TARGET_DIR)/recalbox/share_init/roms/ports/Prince of Persia"
	$(INSTALL) -m 0755 $(@D)/prince -D $(TARGET_DIR)/usr/bin
	cp -r $(@D)/SDLPoP.ini $(TARGET_DIR)/recalbox/share_init/system/configs/sdlpop/sdlpop.ini
	cp -r $(@D)/data "$(TARGET_DIR)/recalbox/share_init/roms/ports/Prince of Persia"
endef

$(eval $(cmake-package))
