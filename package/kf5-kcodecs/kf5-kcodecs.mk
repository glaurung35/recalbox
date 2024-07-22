################################################################################
#
# kf5-kcodecs
#
################################################################################

KF5_KCODECS_VERSION = $(KF5_VERSION)
KF5_KCODECS_SITE = $(KF5_SITE)
KF5_KCODECS_SOURCE = kcodecs-$(KF5_KCODECS_VERSION).tar.xz
KF5_KCODECS_LICENSE = \
	LGPL-2.1 or LGPL-3.0 or LicenseRef-KDE-Accepted-LGPL (library), \
	GPL-2.0 or GPL-3.0 or LicenseRef-KDE-Accepted-GPL (autotests, examples)

KF5_KCODECS_LICENSE_FILES = \
	LICENSES/GPL-2.0-only.txt \
	LICENSES/GPL-3.0-only.txt \
	LICENSES/LGPL-2.1-only.txt \
	LICENSES/LGPL-3.0-only.txt \
	LICENSES/LicenseRef-KDE-Accepted-GPL.txt \
	LICENSES/LicenseRef-KDE-Accepted-LGPL.txt

KF5_KCODECS_DEPENDENCIES = kf5-extra-cmake-modules qt5base
KF5_KCODECS_INSTALL_STAGING = YES

# Uses __atomic_fetch_add_4
ifeq ($(BR2_TOOLCHAIN_HAS_LIBATOMIC),y)
KF5_KCODECS_CONF_OPTS += -DCMAKE_EXE_LINKER_FLAGS=-latomic
endif

$(eval $(cmake-package))
