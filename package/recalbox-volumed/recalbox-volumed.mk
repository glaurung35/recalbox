################################################################################
#
# RECALBOX_VOLUMED
#
################################################################################

RECALBOX_VOLUMED_VERSION = custom
RECALBOX_VOLUMED_SITE = $(TOPDIR)/../projects/recalbox-volumed
RECALBOX_VOLUMED_SITE_METHOD = local
RECALBOX_VOLUMED_LICENSE = GPL-3.0
RECALBOX_VOLUMED_LICENSE_FILES = LICENSE
RECALBOX_VOLUMED_DEPENDENCIES = pulseaudio
RECALBOX_VOLUMED_AUTORECONF = YES

$(eval $(autotools-package))
