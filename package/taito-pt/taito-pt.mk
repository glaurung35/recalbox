################################################################################
#
# TAITO_PT
#
################################################################################

TAITO_PT_VERSION = c8ac5ed19df744b1ae1efd6304c9245511edc0d1
TAITO_PT_SITE = https://github.com/rtomasa/taito-pt.git
TAITO_PT_SITE_METHOD = git
TAITO_PT_LICENSE = GPL-2.0

$(eval $(kernel-module))
$(eval $(generic-package))
