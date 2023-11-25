################################################################################
#
# pikeyd165
#
################################################################################

PIKEYD165_SOURCE =
PIKEYD165_VERSION = 1
PIKEYD165_LICENSE = MIT

define PIKEYD165_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 644 $(PIKEYD165_PKGDIR)/pikeyd165.conf $(TARGET_DIR)/etc/pikeyd165.conf
	$(INSTALL) -D -m 644 $(PIKEYD165_PKGDIR)/99-pi2jamma.rules $(TARGET_DIR)/etc/udev/rules.d/
	$(INSTALL) -D -m 0744 $(PIKEYD165_PKGDIR)/S18pikeyd165 $(TARGET_DIR)/etc/init.d/
	$(INSTALL) -D -m 0744 $(PIKEYD165_PKGDIR)/pi2jamma $(TARGET_DIR)/usr/bin/
	$(INSTALL) -D -m 0744 $(PIKEYD165_PKGDIR)/pikeyd165 $(TARGET_DIR)/usr/bin/
endef

$(eval $(generic-package))
