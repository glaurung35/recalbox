################################################################################
#
# jammasd
#
################################################################################

JAMMASD_VERSION = 1
JAMMASD_LICENSE = GPL2

define JAMMASD_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0644 -D $(BR2_EXTERNAL_RECALBOX_PATH)/jammasd/99-jammasd.rules $(TARGET_DIR)/etc/udev/rules.d/99-jammasd.rules
	$(INSTALL) -m 0755 -D $(BR2_EXTERNAL_RECALBOX_PATH)/jammasd/jammsd $(TARGET_DIR)/usr/bin/jammasd
endef

$(eval $(generic-package))
