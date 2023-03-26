################################################################################
#
# rpi-userland-tools
#
################################################################################

RPI_USERLAND_TOOLS_VERSION = 9d5250fd98679e09c4c1a44883f9312a21f1ac6d
RPI_USERLAND_TOOLS_SITE = $(call github,raspberrypi,userland,$(RPI_USERLAND_TOOLS_VERSION))
RPI_USERLAND_TOOLS_LICENSE = BSD-3-Clause
RPI_USERLAND_TOOLS_LICENSE_FILES = LICENCE
RPI_USERLAND_TOOLS_INSTALL_STAGING = YES
RPI_USERLAND_TOOLS_CONF_OPTS = \
	-DVMCS_INSTALL_PREFIX=/usr \
	-DALL_APPS=OFF \
	-DBUILD_MMAL=OFF \
	-DBUILD_MMAL_APPS=OFF

define RPI_USERLAND_TOOLS_LIBFDT_PATCH
echo -e "include_directories(.)\nadd_library(fdt SHARED fdt.c fdt_empty_tree.c fdt_ro.c fdt_rw.c fdt_sw.c fdt_strerror.c fdt_wip.c)\nINSTALL(TARGETS fdt DESTINATION lib)" > $(@D)/opensrc/helpers/libfdt/CMakeLists.txt
endef

RPI_USERLAND_TOOLS_PRE_PATCH_HOOKS += RPI_USERLAND_TOOLS_LIBFDT_PATCH

ifeq ($(BR2_aarch64),y)
RPI_USERLAND_TOOLS_CONF_OPTS += -DARM64=ON
endif

define RPI_USERLAND_TOOLS_POST_TARGET_CLEANUP
	rm -Rf $(TARGET_DIR)/usr/src
endef
RPI_USERLAND_TOOLS_POST_INSTALL_TARGET_HOOKS += RPI_USERLAND_TOOLS_POST_TARGET_CLEANUP

$(eval $(cmake-package))
