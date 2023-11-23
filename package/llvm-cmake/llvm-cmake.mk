################################################################################
#
# llvm-cmake
#
################################################################################

LLVM_CMAKE_VERSION = 15.0.7
LLVM_CMAKE_SITE = https://github.com/llvm/llvm-project/releases/download/llvmorg-$(LLVM_CMAKE_VERSION)
LLVM_CMAKE_SOURCE = cmake-$(LLVM_CMAKE_VERSION).src.tar.xz
LLVM_CMAKE_LICENSE = Apache-2.0 with exceptions

define HOST_LLVM_CMAKE_INSTALL_CMDS
	mkdir -p $(HOST_DIR)/lib/cmake/llvm
	cp -Rv $(@D)/Modules/* $(HOST_DIR)/lib/cmake/llvm
endef

$(eval $(host-generic-package))
