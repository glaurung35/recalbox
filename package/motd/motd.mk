################################################################################
#
# MOTD
#
################################################################################

MOTD_VERSION = custom
MOTD_SITE_METHOD = local
MOTD_LICENSE = GPL-2.0

define MOTD_COPY_CMDS
	$(INSTALL) -D -m 0644 $(@D)/install/modprobe.conf $(TARGET_DIR)/etc/modprobe.d/xone-blacklist.conf
endef

MOTD_INSTALL_TARGET_CMDS += $(XONE_COPY_CMDS)

$(eval $(generic-package))
