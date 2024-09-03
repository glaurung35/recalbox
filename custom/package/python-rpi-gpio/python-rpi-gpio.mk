################################################################################
#
# python-rpi-gpio
#
################################################################################

PYTHON_RPI_GPIO_VERSION = 2debec30e78fca89abc83186b4d841f04c38930f
PYTHON_RPI_GPIO_SITE = $(call github,waveform80,rpi-lgpio,$(PYTHON_RPI_GPIO_VERSION))
PYTHON_RPI_GPIO_LICENSE = MIT
PYTHON_RPI_GPIO_LICENSE_FILES = LICENCE.txt
PYTHON_RPI_GPIO_SETUP_TYPE = setuptools

$(eval $(python-package))
