################################################################################
#
# EVMAPY
#
################################################################################
EVMAPY_VERSION = bd65338c236cd30b4f2d7835733ea5d6b108b75d
EVMAPY_SITE =  $(call github,kempniu,evmapy,$(EVMAPY_VERSION))
EVMAPY_SETUP_TYPE = distutils

define EVMAPY_FIXCHARS
        sed -i -e s+"Michał Kępień"+"Michal Kepien"+ $(@D)/*.py $(@D)/evmapy/*.py
endef
EVMAPY_PRE_CONFIGURE_HOOKS += EVMAPY_FIXCHARS

$(eval $(python-package))
