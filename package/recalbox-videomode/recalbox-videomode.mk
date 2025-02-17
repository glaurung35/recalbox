################################################################################
#
# Videomode command line utility
#
################################################################################

RECALBOX_VIDEOMODE_VERSION = 1.0
RECALBOX_VIDEOMODE_SITE = $(TOPDIR)/../projects/videomode
RECALBOX_VIDEOMODE_SITE_METHOD = local
RECALBOX_VIDEOMODE_LICENSE = MIT
RECALBOX_VIDEOMODE_DEPENDENCIES = sdl2

define RECALBOX_VIDEOMODE_RPI_FIXUP
	$(SED) 's|/opt/vc/include|$(STAGING_DIR)/usr/include|g' $(@D)/CMakeLists.txt
	$(SED) 's|/opt/vc/lib|$(STAGING_DIR)/usr/lib|g' $(@D)/CMakeLists.txt
	$(SED) 's|/usr/lib|$(STAGING_DIR)/usr/lib|g' $(@D)/CMakeLists.txt
endef

RECALBOX_VIDEOMODE_PRE_CONFIGURE_HOOKS += RECALBOX_VIDEOMODE_RPI_FIXUP

ifeq ($(BR2_ENABLE_DEBUG),y)
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_BUILD_TYPE=Debug
endif

RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_C_ARCHIVE_CREATE="<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>"
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_C_ARCHIVE_FINISH=true
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_CXX_ARCHIVE_CREATE="<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>"
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_CXX_ARCHIVE_FINISH=true
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_AR="$(TARGET_CC)-ar"
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_C_COMPILER="$(TARGET_CC)"
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_CXX_COMPILER="$(TARGET_CXX)"
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_LINKER="$(TARGET_LD)"
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_C_FLAGS="$(COMPILER_COMMONS_CFLAGS_EXE)"
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_CXX_FLAGS="$(COMPILER_COMMONS_CXXFLAGS_EXE)"
RECALBOX_VIDEOMODE_CONF_OPTS += -DCMAKE_LINKER_EXE_FLAGS="$(COMPILER_COMMONS_LDFLAGS_EXE)"

$(eval $(cmake-package))
