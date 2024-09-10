################################################################################
#
# lgpio
#
################################################################################

LGPIO_VERSION = 746f0df43774175090b93abcc860b6733eefc09b
LGPIO_SITE = $(call github,joan2937,lg,$(LGPIO_VERSION))
LGPIO_LICENSE = unlicense.org
LGPIO_DEPENDENCIES = 

$(eval $(generic-package))
