################################################################################
#
# python-evdev
#
################################################################################

PYTHON_EVDEV_VERSION = v0.7.0
PYTHON_EVDEV_SITE =  $(call github,gvalkov,python-evdev,$(PYTHON_EVDEV_VERSION))
PYTHON_EVDEV_LICENSE = BSD-2-Clause
PYTHON_EVDEV_LICENSE_FILES = LICENSE
PYTHON_EVDEV_SETUP_TYPE = setuptools

$(eval $(python-package))
