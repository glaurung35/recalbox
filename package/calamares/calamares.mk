################################################################################
#
# calamares
#
################################################################################

CALAMARES_VERSION = 80ef430185212b9312bf30ef4eba9890c7a9fcfe
CALAMARES_SITE =  $(call github,calamares,calamares,$(CALAMARES_VERSION))
CALAMARES_LICENSE = CUSTOM
CALAMARES_LICENSE_FILES = README
CALAMARES_DEPENDENCIES = qt5base qt5tools qt5svg yaml-cpp kf5-kcoreaddons parted

CALAMARES_CONF_OPTS = -DINSTALL_POLKIT=OFF -DWITH="partition"

define CALAMARES_INSTALL_CONFIG
	mkdir -p $(TARGET_DIR)/etc/calamares/
	$(INSTALL) -m 0644 -D $(BR2_EXTERNAL_RECALBOX_PATH)/package/calamares/settings.conf $(TARGET_DIR)/etc/calamares/settings.conf
endef
CALAMARES_POST_INSTALL_TARGET_HOOKS += CALAMARES_INSTALL_CONFIG

$(eval $(cmake-package))
