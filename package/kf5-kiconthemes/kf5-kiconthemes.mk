################################################################################
#
# kf5-kiconthmes
#
################################################################################

KF5_KICONTHEMES_VERSION = $(KF5_VERSION)
KF5_KICONTHEMES_SITE = $(KF5_SITE)
KF5_KICONTHEMES_SOURCE = kiconthemes-$(KF5_KICONTHEMES_VERSION).tar.xz
KF5_KICONTHEMES_LICENSE = \
	LGPL-2.1 or LGPL-3.0 or LicenseRef-KDE-Accepted-LGPL (library), \
	GPL-2.0 or GPL-3.0 or LicenseRef-KDE-Accepted-GPL (autotests, examples)

KF5_KICONTHEMES_LICENSE_FILES = \
	LICENSES/GPL-2.0-only.txt \
	LICENSES/GPL-3.0-only.txt \
	LICENSES/LGPL-2.1-only.txt \
	LICENSES/LGPL-3.0-only.txt \
	LICENSES/LicenseRef-KDE-Accepted-GPL.txt \
	LICENSES/LicenseRef-KDE-Accepted-LGPL.txt

KF5_KICONTHEMES_DEPENDENCIES = kf5-extra-cmake-modules qt5base kf5-karchive kf5-kconfigwidgets kf5-kitemviews
KF5_KICONTHEMES_INSTALL_STAGING = YES

# Uses __atomic_fetch_add_4
ifeq ($(BR2_TOOLCHAIN_HAS_LIBATOMIC),y)
KF5_KICONTHEMES_CONF_OPTS += -DCMAKE_EXE_LINKER_FLAGS=-latomic -DBUILD_DESIGNERPLUGIN=OFF
endif

$(eval $(cmake-package))
