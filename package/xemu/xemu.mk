################################################################################
#
# xemu
#
################################################################################

XEMU_VERSION = b4ce2b58d09baa6dfd17c0bcab2e505df9f310e4
XEMU_SITE = https://github.com/xemu-project/xemu.git
XEMU_SITE_METHOD=git
XEMU_GIT_SUBMODULES=YES
XEMU_LICENSE = GPLv2
XEMU_DEPENDENCIES = sdl2 libsamplerate slirp

XEMU_CONF_OPTS += \
	--target-list=i386-softmmu \
	--disable-cocoa \
	--disable-coreaudio \
	--disable-gtk-clipboard \
	--disable-werror \
	--enable-opengl \
	--enable-avx2
	CFLAGS="$(TARGET_CFLAGS) $(COMPILER_COMMONS_CFLAGS_NOLTO)" \
	CXXFLAGS="$(TARGET_CXXFLAGS) $(COMPILER_COMMONS_CXXFLAGS_NOLTO)" \
	LDFLAGS="$(TARGET_LDFLAGS) $(COMPILER_COMMONS_LDFLAGS_NOLTO)"

$(eval $(autotools-package))

