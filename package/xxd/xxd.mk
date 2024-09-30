################################################################################
#
# xxd
#
################################################################################

XXD_VERSION = 084dc9dec6b6a6d048934916aa9a539d49ba898d
XXD_SITE =  $(call github,ConorOG,xxd,$(XXD_VERSION))
XXD_LICENSE = CUSTOM
XXD_LICENSE_FILES = README

define HOST_XXD_BUILD_CMDS
	$(MAKE) -C $(@D)
endef

define HOST_XXD_INSTALL_CMDS
	$(INSTALL) -D $(@D)/xxd $(HOST_DIR)/bin/xxd
endef

$(eval $(host-generic-package))
