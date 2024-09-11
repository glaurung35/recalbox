################################################################################
#
# ikemen
#
################################################################################

IKEMEN_VERSION = 02a078d6bf189ff56bbcbbbc8dbfd81753692c56
IKEMEN_SITE = https://github.com/ikemen-engine/Ikemen-GO.git
IKEMEN_LICENSE = MIT
IKEMEN_LICENSE_FILES = License.txt
IKEMEN_SITE_METHOD = git
IKEMEN_GIT_SUBMODULES = YES

IKEMEN_DEPENDENCIES = libgtk3 mesa3d openal libglfw

define IKEMEN_BUILD_CMDS
	$(HOST_GO_TARGET_ENV) $(MAKE) -C $(@D) -f Makefile Ikemen_GO_Linux
endef

define IKEMEN_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/usr/bin
	$(INSTALL) -D $(@D)/bin/Ikemen_GO_Linux $(TARGET_DIR)/usr/bin/ikemen
endef

$(eval $(golang-package))
