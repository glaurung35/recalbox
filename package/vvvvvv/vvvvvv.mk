################################################################################
#
# VVVVVV
#
################################################################################

#Commit of 05/02/2024
VVVVVV_VERSION = d678bd59ff0c1643009e629b9795e87e83c8bd82
VVVVVV_SITE = https://github.com/TerryCavanagh/VVVVVV.git
VVVVVV_LICENSE = VVVVVV Source Code License v1.0
VVVVVV_SUBDIR = desktop_version
VVVVVV_DEPENDENCIES = sdl2
VVVVVV_GIT_SUBMODULES = YES
VVVVVV_SITE_METHOD = git

define VVVVVV_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/usr/bin/vvvvvv/
	cp -r $(@D)/desktop_version/fonts $(TARGET_DIR)/usr/bin/vvvvvv/
	cp -r $(@D)/desktop_version/lang $(TARGET_DIR)/usr/bin/vvvvvv/
	$(INSTALL) -D -m 0755 $(@D)/desktop_version/VVVVVV $(TARGET_DIR)/usr/bin/vvvvvv/
	ln -snf /recalbox/share/roms/ports/vvvvvv/data.zip $(TARGET_DIR)/usr/bin/vvvvvv/data.zip
endef

$(eval $(cmake-package))
