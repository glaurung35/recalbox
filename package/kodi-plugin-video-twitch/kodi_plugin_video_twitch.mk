################################################################################
#
# kodi twitch plugin
#
################################################################################

KODI_PLUGIN_VIDEO_TWITCH_RELEASE = matrix
KODI_PLUGIN_VIDEO_TWITCH_VERSION = 2.6.2
KODI_PLUGIN_VIDEO_TWITCH_PLUGINNAME = plugin.video.twitch
KODI_PLUGIN_VIDEO_TWITCH_SOURCE = $(KODI_PLUGIN_VIDEO_TWITCH_PLUGINNAME)-$(KODI_PLUGIN_VIDEO_TWITCH_VERSION)+$(KODI_PLUGIN_VIDEO_TWITCH_RELEASE).1.zip
KODI_PLUGIN_VIDEO_TWITCH_SITE = https://github.com/anxdpanic/$(KODI_PLUGIN_VIDEO_TWITCH_PLUGINNAME)/releases/download/v$(KODI_PLUGIN_VIDEO_TWITCH_VERSION)
KODI_PLUGIN_VIDEO_TWITCH_LICENSE = GPL-3.0-only
KODI_PLUGIN_VIDEO_TWITCH_LICENSE_FILES = $(KODI_PLUGIN_VIDEO_TWITCH_PLUGINNAME)/LICENSES/GPL-3.0-only

KODI_PLUGIN_VIDEO_TWITCH_TARGET_DIR=$(TARGET_DIR)/usr/share/kodi/addons

define KODI_PLUGIN_VIDEO_TWITCH_EXTRACT_CMDS
	@unzip -q -o $(DL_DIR)/kodi-plugin-video-twitch/$(KODI_PLUGIN_VIDEO_TWITCH_SOURCE) -d $(@D)
endef

define KODI_PLUGIN_VIDEO_TWITCH_INSTALL_TARGET_CMDS
	@mkdir -p $(KODI_PLUGIN_VIDEO_TWITCH_TARGET_DIR)
	@cp -r $(@D)/$(KODI_PLUGIN_VIDEO_TWITCH_PLUGINNAME) $(KODI_PLUGIN_VIDEO_TWITCH_TARGET_DIR)
endef

$(eval $(generic-package))
