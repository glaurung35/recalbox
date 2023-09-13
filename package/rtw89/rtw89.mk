################################################################################
#
# rtw89
#
################################################################################

# commit of 2023/09/13
RTW89_VERSION = 12837647e34f07e117d90b25708a4f8baa772f50
RTW89_SITE = $(call github,lwfinger,rtw89,$(RTW89_VERSION))
RTW89_LICENSE = GPL-2.0
RTW89_LICENSE_FILES = LICENSE

$(eval $(kernel-module))
$(eval $(generic-package))
