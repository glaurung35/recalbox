################################################################################
#
# rtl88x2bu
#
################################################################################

RTL88X2BU_VERSION = 12cfcd8cd8ec7115158df3d223510435541ddc32
RTL88X2BU_SITE = $(call github,RinCat,RTL88x2BU-Linux-Driver,$(RTL88X2BU_VERSION))
RTL88X2BU_LICENSE = GPL-2.0
RTL88X2BU_LICENSE_FILES = LICENSE

RTL88X2BU_MODULE_MAKE_OPTS = \
	KSRC=$(LINUX_DIR) \
	KVER=$(LINUX_VERSION_PROBED) \
	INSTALL_MOD_PATH=$(TARGET_DIR) \
	-C $(@D)

$(eval $(kernel-module))
$(eval $(generic-package))
