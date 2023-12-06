################################################################################
#
# GL4ES
#
################################################################################

GL4ES_VERSION = e39434a2b17d12be569a1406f3bd23ac6a231347
GL4ES_SITE =  $(call github,ptitSeb,gl4es,$(GL4ES_VERSION))
GL4ES_LICENSE = MIT
GL4ES_LICENSE_FILES = LICENSE
GL4ES_INSTALL_STAGING = YES
GL4ES_DEPENDENCIES = libgles

GL4ES_CONF_OPTS = -DNOX11=ON

$(eval $(cmake-package))
