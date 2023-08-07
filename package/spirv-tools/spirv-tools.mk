################################################################################
#
# spirv-tools
#
################################################################################

SPIRV_TOOLS_VERSION = 2023.2
SPIRV_TOOLS_SITE = $(call github,KhronosGroup,SPIRV-Tools,v$(SPIRV_TOOLS_VERSION))
SPIRV_TOOLS_LICENSE = Apache-2.0
SPIRV_TOOLS_LICENSE_FILES = LICENSE
SPIRV_TOOLS_DEPENDENCIES = host-python3 host-spirv-headers

# check spirv-headers if it needs a bump
SPIRV_TOOLS_CONF_OPTS = -DSPIRV-Headers_SOURCE_DIR=$(HOST_DIR)/usr

$(eval $(cmake-package))
