################################################################################
#
# mesa3d-headers
#
################################################################################

# mesa3d-headers is inherently incompatible with mesa3d, so error out
# if both are enabled.
ifeq ($(BR_BUILDING)$(BR2_PACKAGE_MESA3D_RPI5)$(BR2_PACKAGE_MESA3D_HEADERS_RPI5),yyy)
$(error mesa3d-headers-rpi5 enabled, but mesa3d-rpi5 enabled too)
endif

# Not possible to directly refer to mesa3d variables, because of
# first/second expansion trickery...
MESA3D_HEADERS_RPI5_VERSION = f59665bb62b579c6cc26155a26ce557f9f3357d2
MESA3D_HEADERS_RPI5_SITE = https://gitlab.freedesktop.org/mesa/mesa.git
MESA3D_HEADERS_RPI5_SITE_METHOD = git
MESA3D_HEADERS_RPI5_DL_SUBDIR = mesa3d
MESA3D_HEADERS_RPI5_LICENSE = MIT, SGI, Khronos
MESA3D_HEADERS_RPI5_LICENSE_FILES = docs/license.rst
MESA3D_HEADERS_RPI5_CPE_ID_VENDOR = mesa3d
MESA3D_HEADERS_RPI5_CPE_ID_PRODUCT = mesa

# Only installs header files
MESA3D_HEADERS_RPI5_INSTALL_STAGING = YES
MESA3D_HEADERS_RPI5_INSTALL_TARGET = NO

MESA3D_HEADERS_RPI5_DIRS = KHR

ifeq ($(BR2_PACKAGE_HAS_LIBGL),y)

MESA3D_HEADERS_RPI5_DIRS += GL

ifeq ($(BR2_PACKAGE_XORG7),y)

# Not using $(SED) because we do not want to work in-place, and $(SED)
# contains -i.
define MESA3D_HEADERS_RPI5_BUILD_DRI_PC
	sed -e 's:@VERSION@:$(MESA3D_HEADERS_RPI5_VERSION):' \
		$(MESA3D_HEADERS_RPI5_PKGDIR)/dri.pc \
		>$(@D)/src/gallium/frontends/dri/dri.pc
endef

define MESA3D_HEADERS_RPI5_INSTALL_DRI_PC
	$(INSTALL) -D -m 0644 $(@D)/include/GL/internal/dri_interface.h \
		$(STAGING_DIR)/usr/include/GL/internal/dri_interface.h
	$(INSTALL) -D -m 0644 $(@D)/src/gallium/frontends/dri/dri.pc \
		$(STAGING_DIR)/usr/lib/pkgconfig/dri.pc
endef

endif # Xorg

endif # OpenGL

ifeq ($(BR2_PACKAGE_HAS_LIBEGL),y)
MESA3D_HEADERS_RPI5_DIRS += EGL
endif

ifeq ($(BR2_PACKAGE_HAS_LIBGLES),y)
MESA3D_HEADERS_RPI5_DIRS += GLES GLES2
endif

ifeq ($(BR2_PACKAGE_HAS_LIBOPENCL),y)
MESA3D_HEADERS_RPI5_DIRS += CL
endif

define MESA3D_HEADERS_RPI5_BUILD_CMDS
	$(MESA3D_HEADERS_RPI5_BUILD_DRI_PC)
endef

define MESA3D_HEADERS_RPI5_INSTALL_STAGING_CMDS
	$(foreach d,$(MESA3D_HEADERS_RPI5_DIRS),\
		cp -dpfr $(@D)/include/$(d) $(STAGING_DIR)/usr/include/ || exit 1$(sep))
	$(MESA3D_HEADERS_RPI5_INSTALL_DRI_PC)
endef

$(eval $(generic-package))
