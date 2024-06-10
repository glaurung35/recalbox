################################################################################
#
# python-upnpy
#
################################################################################

PYTHON_UPNPY_VERSION = 8b5eaeae26b26cebe12405c5ebd31fdaaa75fa60
PYTHON_UPNPY_SITE = $(call github,mark-pippin,upnpy-bugfix,$(PYTHON_UPNPY_VERSION))
PYTHON_UPNPY_LICENSE = MIT
PYTHON_UPNPY_LICENSE_FILES = LICENSE
PYTHON_UPNPY_SETUP_TYPE = distutils

define PYTHON_UPNPY_ADD_SCRIPT
	cp $(PYTHON_UPNPY_PKGDIR)netplay_port $(TARGET_DIR)/usr/bin/netplay_port
endef

PYTHON_UPNPY_POST_INSTALL_TARGET_HOOKS += PYTHON_UPNPY_ADD_SCRIPT

$(eval $(python-package))
