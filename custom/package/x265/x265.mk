################################################################################
#
# x265
#
################################################################################

X265_VERSION = 3.5
X265_SOURCE = x265_$(X265_VERSION).tar.gz
X265_SITE = https://bitbucket.org/multicoreware/x265_git/downloads
X265_LICENSE = GPL-2.0+
X265_LICENSE_FILES = COPYING
X265_CPE_ID_VENDOR = multicorewareinc
X265_SUBDIR = source
X265_INSTALL_STAGING = YES

ifeq ($(BR2_i386)$(BR2_x86_64),y)
X265_DEPENDENCIES += host-nasm
endif

# disable altivec, it has build issues
# https://bitbucket.org/multicoreware/x265/issues/320/
ifeq ($(BR2_powerpc64)$(BR2_powerpc64le),y)
X265_CONF_OPTS += -DENABLE_ALTIVEC=OFF
endif

ifeq ($(BR2_SHARED_LIBS)$(BR2_SHARED_STATIC_LIBS),y)
X265_CONF_OPTS += -DENABLE_SHARED=ON -DENABLE_PIC=ON
else
X265_CONF_OPTS += -DENABLE_SHARED=OFF
endif

ifeq ($(BR2_PACKAGE_X265_CLI),y)
X265_CONF_OPTS += -DENABLE_CLI=ON
else
X265_CONF_OPTS += -DENABLE_CLI=OFF
endif

ifeq ($(BR2_ARM_CPU_ARMV6),y)
X265_CONF_OPTS += -DCROSS_COMPILE_ARM=1
endif

$(eval $(cmake-package))
