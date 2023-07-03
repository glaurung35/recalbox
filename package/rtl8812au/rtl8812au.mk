################################################################################
#
# rtl8812au
#
################################################################################

# commit of 3/07/2023 (v5.6.4.2)
RTL8812AU_VERSION = 9dc4a4c9a2637df74692542aa01ba63f6fad6f5a
RTL8812AU_SITE = $(call github,aircrack-ng,rtl8812au,$(RTL8812AU_VERSION))
RTL8812AU_LICENSE = GPL-2.0
RTL8812AU_LICENSE_FILES = LICENSE

RTL8812AU_MODULE_MAKE_OPTS = \
	KSRC="$(LINUX_DIR)" \
	KVER="$(LINUX_VERSION_PROBED)" \
	INSTALL_MOD_PATH="$(TARGET_DIR)" \
	-C $(@D)

$(eval $(kernel-module))
$(eval $(generic-package))
