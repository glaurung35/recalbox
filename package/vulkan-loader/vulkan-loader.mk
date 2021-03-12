################################################################################
#
# vulkan-loader
#
################################################################################

VULKAN_LOADER_VERSION = 1.3.259
VULKAN_LOADER_SITE = $(call github,KhronosGroup,Vulkan-Loader,v$(VULKAN_LOADER_VERSION))
VULKAN_LOADER_LICENSE = Apache-2.0
VULKAN_LOADER_LICENSE_FILES = LICENSE.txt
VULKAN_LOADER_INSTALL_STAGING = YES
VULKAN_LOADER_DEPENDENCIES = vulkan-headers

VULKAN_LOADER_CONF_OPTS = -DVulkanHeaders_INCLUDE_DIR=$(STAGING_DIR)/usr/include/ \
													-DVulkanRegistry_DIR=$(TARGET_DIR)/usr/share/vulkan/registry

ifeq ($(BR2_PACKAGE_XORG7),y)
VULKAN_LOADER_DEPENDENCIES += libxcb xserver_xorg-server
VULKAN_LOADER_CONF_OPTS += \
	-DBUILD_WSI_XCB_SUPPORT=ON \
	-DBUILD_WSI_XLIB_SUPPORT=ON
else
VULKAN_LOADER_CONF_OPTS += \
	-DBUILD_WSI_XCB_SUPPORT=OFF \
	-DBUILD_WSI_XLIB_SUPPORT=OFF
endif

ifeq ($(BR2_PACKAGE_WAYLAND),y)
VULKAN_LOADER_DEPENDENCIES += wayland
VULKAN_LOADER_CONF_OPTS += -DBUILD_WSI_WAYLAND_SUPPORT=ON
else
VULKAN_LOADER_CONF_OPTS += -DBUILD_WSI_WAYLAND_SUPPORT=OFF
endif

$(eval $(cmake-package))
