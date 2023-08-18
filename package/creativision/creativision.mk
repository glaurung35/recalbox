################################################################################
#
# Creativision emulator
#
################################################################################

CREATIVISION_VERSION = d18fbe888eef21f7e7ef309a671c7d5834a5c641
CREATIVISION_SITE = $(call gitlab,recalbox,packages/standalone/creativision,$(CREATIVISION_VERSION))
CREATIVISION_LICENSE = CUSTOM
CREATIVISION_DEPENDENCIES = sdl2 sdl2_net libpng zlib zip

ifeq ($(BR2_ENABLE_DEBUG),y)
CREATIVISION_CONF_OPTS += -DCMAKE_BUILD_TYPE=Debug
else
CREATIVISION_CONF_OPTS += -DCMAKE_BUILD_TYPE=Release
endif

CREATIVISION_CONF_OPTS += -DBUILD_SHARED_LIBS=OFF
CREATIVISION_CONF_OPTS += -DCMAKE_C_ARCHIVE_CREATE="<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>"
CREATIVISION_CONF_OPTS += -DCMAKE_C_ARCHIVE_FINISH=true
CREATIVISION_CONF_OPTS += -DCMAKE_AR="$(TARGET_CC)-ar"
CREATIVISION_CONF_OPTS += -DCMAKE_C_COMPILER="$(TARGET_CC)"
CREATIVISION_CONF_OPTS += -DCMAKE_CXX_COMPILER="$(TARGET_CXX)"
CREATIVISION_CONF_OPTS += -DCMAKE_LINKER="$(TARGET_LD)"
CREATIVISION_CONF_OPTS += -DCMAKE_C_FLAGS="$(COMPILER_COMMONS_CFLAGS_EXE)"
CREATIVISION_CONF_OPTS += -DCMAKE_CXX_FLAGS="$(COMPILER_COMMONS_CXXFLAGS_EXE)"
CREATIVISION_CONF_OPTS += -DCMAKE_LD_FLAGS="$(COMPILER_COMMONS_CXXFLAGS_EXE)"
CREATIVISION_CONF_OPTS += -DCMAKE_EXE_LINKER_FLAGS="$(COMPILER_COMMONS_LDFLAGS_EXE)"

define CREATIVISION_INSTALL_TARGET_CMDS
	$(INSTALL) -D $(@D)/creatiVision $(TARGET_DIR)/usr/bin/creatiVision
	mkdir -p $(TARGET_DIR)/usr/share/creatiVision
	$(INSTALL) -D $(@D)/utils/tool_bas2cas $(TARGET_DIR)/usr/share/creatiVision/bas2cas
	$(INSTALL) -D $(@D)/utils/tool_txt2cas $(TARGET_DIR)/usr/share/creatiVision/txt2cas
	$(INSTALL) -D $(@D)/utils/tool_cas2wav $(TARGET_DIR)/usr/share/creatiVision/cas2wav
	$(INSTALL) -D $(@D)/utils/tool_cvgfx $(TARGET_DIR)/usr/share/creatiVision/cvgfx
	$(INSTALL) -D $(@D)/utils/tool_smbloader $(TARGET_DIR)/usr/share/creatiVision/smbloader
endef

$(eval $(cmake-package))
