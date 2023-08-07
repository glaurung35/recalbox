################################################################################
#
# spirv-headers
#
################################################################################

SPIRV_HEADERS_VERSION = 1.3.246.1
SPIRV_HEADERS_SITE = $(call github,KhronosGroup,SPIRV-Headers,sdk-$(SPIRV_HEADERS_VERSION))
SPIRV_HEADERS_LICENSE = Apache-2.0
SPIRV_HEADERS_LICENSE_FILES = LICENSE
SPIRV_HEADERS_DEPENDENCIES = host-python3

$(eval $(cmake-package))
$(eval $(host-cmake-package))
