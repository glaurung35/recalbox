################################################################################
#
# raspi2png
#
################################################################################

RASPI2PNG_VERSION = b3c5599c2e2652a3f585dc89075ff65dcd36a0dc
RASPI2PNG_SITE = $(call github,AndrewFromMelbourne,raspi2png,$(RASPI2PNG_VERSION))
RASPI2PNG_LICENSE = MIT
RASPI2PNG_LICENSE_FILES = LICENSE

RASPI2PNG_LDFLAGS = -L$(STAGING_DIR)/usr/lib -lbcm_host -lpng -lm -lvchostif
RASPI2PNG_INCLUDES = -I$(STAGING_DIR)/usr/include/ -I$(STAGING_DIR)/usr/include/interface/vcos/pthreads -I$(STAGING_DIR)/usr/include/interface/vmcs_host/linux

# rpi0, 1, 2 and 3
ifeq ($(BR2_PACKAGE_RPI_USERLAND),y)
RASPI2PNG_DEPENDENCIES += rpi-userland
endif

define RASPI2PNG_BUILD_CMDS
	CFLAGS="$(TARGET_CFLAGS)" CXXFLAGS="$(TARGET_CXXFLAGS)" \
		$(MAKE) CXX="$(TARGET_CXX)" CC="$(TARGET_CC)" LDFLAGS="$(RASPI2PNG_LDFLAGS)" INCLUDES="$(RASPI2PNG_INCLUDES)" -C $(@D)
endef

define RASPI2PNG_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/raspi2png \
		$(TARGET_DIR)/usr/bin/raspi2png.real
endef

$(eval $(generic-package))
