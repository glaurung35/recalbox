import os
import logger
from installers.base.install import InstallBase
from settings import keyValueSettings


class Install(InstallBase):

    BASE_SOURCE_FOLDER = InstallBase.BASE_SOURCE_FOLDER + "gpi/"

    def __init__(self):
        InstallBase.__init__(self)


    def InstallHardware(self, case):

        if case == "GPiV1":

            logger.hardlog("Installing GPi Case V1 hardware")

            try:
                os.system("mount -o remount,rw /boot")
                # Install /boot/config.txt - most important change first
                sourceConfig = self.BASE_SOURCE_FOLDER + "assets/config.txt"
                os.system("cp /boot/config.txt /boot/config.txt.backup")
                if os.system("cp {} /boot".format(sourceConfig)) != 0:
                    logger.hardlog("GPi: Error installing config.txt")
                    return False
                logger.hardlog("GPi: config.txt installed")

                # Install Overlay
                sourceOverlay = self.BASE_SOURCE_FOLDER + "assets/overlays/*.dtbo"
                if os.system("cp -r {} /boot/overlays".format(sourceOverlay)) != 0:
                    logger.hardlog("GPi: Error installing overlays")
                    return False
                logger.hardlog("GPi: overlays installed")

            except Exception as e:
                logger.hardlog("GPi: Exception = {}".format(e))
                return False

            finally:
                os.system("mount -o remount,ro /")
                os.system("mount -o remount,ro /boot")

            logger.hardlog("GPiV1 hardware installed successfully!")
            return True

        # Unidentified case
        return False


    def InstallSoftware(self, case):

        if case == "GPiV1":

            logger.hardlog("Installing GPi Case V1 software")

            try:
                os.system("mount -o remount,rw /")

                # Switch default resolution
                os.system("sed -i -E 's/([a-z\.]*)videomode=.*/\\1videomode=default/g' /recalbox/share/system/recalbox.conf")
                logger.hardlog("GPi: set default resolution")

                # Set new hostname
                os.system("sed -i -E 's/system.hostname=.*/system.hostname=RECALBOXGPI/g' /recalbox/share/system/recalbox.conf")
                logger.hardlog("GPi: set RECALBOXGPI hostname")

                # Disable kodi
                os.system("sed -i -E 's/kodi.enabled=.*/kodi.enabled=0/g' /recalbox/share/system/recalbox.conf")
                os.system("sed -i -E 's/kodi.xbutton=.*/kodi.xbutton=0/g' /recalbox/share/system/recalbox.conf")
                logger.hardlog("GPi: KODI disabled")

                # Disable virtual gamepad
                os.system("sed -i -E 's/system.virtual-gamepads.enabled=.*/system.virtual-gamepads.enabled=0/g' /recalbox/share/system/recalbox.conf")
                logger.hardlog("GPi: Virtual Gamepad disabled")

                # Disable updates
                os.system("sed -i -E 's/updates.enabled=.*/updates.enabled=0/g' /recalbox/share/system/recalbox.conf")
                logger.hardlog("GPi: Updates disabled")

                # Disable netplay
                os.system("sed -i -E 's/global.netplay=.*/global.netplay=0/g' /recalbox/share/system/recalbox.conf")
                logger.hardlog("GPi: Netplay disabled")

                # Disable webmanager
                os.system("sed -i -E 's/system.manager.enabled=.*/system.manager.enabled=0/g' /recalbox/share/system/recalbox.conf")
                logger.hardlog("GPi: Webmanager disabled")

                # Disable other controllers
                os.system("sed -i -E 's/controllers.xarcade.enabled=.*/controllers.xarcade.enabled=0/g' /recalbox/share/system/recalbox.conf")
                os.system("sed -i -E 's/controllers.bluetooth.enabled=.*/controllers.bluetooth.enabled=0/g' /recalbox/share/system/recalbox.conf")
                os.system("sed -i -E 's/controllers.ps3.enabled=.*/controllers.ps3.enabled=0/g' /recalbox/share/system/recalbox.conf")
                logger.hardlog("GPi: Other controllers disabled")

                # Disable music popups
                os.system("sed -i -E 's/name=|MusicPopupTime| value=|.*|/name=|MusicPopupTime| value=|0|/g' /recalbox/share/system/.emulationstation/es_settings.cfg".replace('|', '"'))
                logger.hardlog("GPi: Music popup disabled")

                # Install GPi XBOX360 config
                os.system("rm -f /recalbox/share/system/.emulationstation/es_input.cfg.org")
                os.system("mv /recalbox/share/system/.emulationstation/es_input.cfg /recalbox/share/system/.emulationstation/es_input.cfg.org")
                os.system("mv " + self.BASE_SOURCE_FOLDER + "assets/es_input.fragment.xml /recalbox/share/system/.emulationstation/es_input.cfg")
                logger.hardlog("GPi: controller updated")

                # Install Theme
                sourceTheme = self.BASE_SOURCE_FOLDER + "assets/theme/recalbox-gpicase"
                if os.system("cp -r {} /recalbox/share/themes/".format(sourceTheme)) == 0:
                    # Switch Theme
                    os.system("sed -i -E 's/name=|ThemeSet| value=|.*|/name=|ThemeSet| value=|recalbox-gpicase|/g' /recalbox/share/system/.emulationstation/es_settings.cfg".replace('|', '"'))
                    logger.hardlog("GPi: GPi-case theme installed")
                else:
                    logger.hardlog("GPi: GPi-case theme NOT installed")

                # install boot image
                sourcePpm = self.BASE_SOURCE_FOLDER + "assets/gpi.ppm"
                if os.system("cp -r {} /boot/boot.ppm".format(sourcePpm)) == 0:
                    logger.hardlog("GPi: GPi-case boot image installed")
                else:
                    logger.hardlog("GPi: GPi-case boot image NOT installed")

                # Copy sound configuration
                os.system("sed -i -E 's/name=|SystemVolume| value=|.*|/name=|SystemVolume| value=|90|/g' /recalbox/share/system/.emulationstation/es_settings.cfg".replace('|', '"'))
                soundConfiguration = self.BASE_SOURCE_FOLDER + "assets/asound.conf"
                if os.system("cp {} /etc/".format(soundConfiguration)) == 0:
                    logger.hardlog("GPi: Sound configuration installed")
                else:
                    logger.hardlog("GPi: Sound configuration NOT installed")

                # Fine tune retroarch
                retroarchOriginOverrides =\
                {
                    "menu_driver": "rgui",
                    "menu_rgui_full_width_layout": "true",
                    "menu_rgui_shadows": "true",
                    "rgui_aspect_ratio": "0",
                    "rgui_aspect_ratio_lock": "1",
                    "rgui_background_filler_thickness_enable": "false",
                    "rgui_border_filler_enable": "true",
                    "rgui_border_filler_thickness_enable": "false",
                    "rgui_browser_directory": "default",
                    "rgui_config_directory": "~/.config/retroarch/config",
                    "rgui_extended_ascii": "true",
                    "rgui_inline_thumbnails": "true",
                    "rgui_internal_upscale_level": "1",
                    "rgui_menu_color_theme": "25",
                    "rgui_menu_theme_preset": "",
                    "rgui_particle_effect": "4",
                    "rgui_particle_effect_speed": "1.000000",
                    "rgui_show_start_screen": "false",
                    "rgui_swap_thumbnails": "false",
                    "rgui_thumbnail_delay": "0",
                    "rgui_thumbnail_downscaler": "0",
                    "video_font_size": "18",
                    "menu_widget_scale_auto": "false",
                    "menu_widget_scale_factor": "2.800000",
                    "menu_widget_scale_factor_windowed": "2.800000",
                }
                for fileName in ("/recalbox/share/system/configs/retroarch/retroarchcustom.cfg.origin",
                                 "/recalbox/share/system/configs/retroarch/retroarchcustom.cfg"):
                    settings = keyValueSettings(fileName, True)
                    settings.loadFile(True)
                    for k, v in retroarchOriginOverrides.items():
                        settings.setOption(k, v)
                    settings.saveFile()
                logger.hardlog("GPi: Retroarch gui set to RGUI")

            except Exception as e:
                logger.hardlog("GPi: Exception = {}".format(e))
                return ""

            finally:
                os.system("mount -o remount,ro /")
                os.system("mount -o remount,ro /boot")

            logger.hardlog("GPiV1 software installed successfully!")
            return case

        # Unidentified case
        return ""


    def UninstallHardware(self, case):

        try:
            os.system("mount -o remount,rw /boot")
            # Uninstall /boot/config.txt
            if os.system("cp /boot/config.txt.backup /boot/config.txt") != 0:
                logger.hardlog("GPi: Error uninstalling config.txt")
                return False
            logger.hardlog("GPi: config.txt uninstalled")

        except Exception as e:
            logger.hardlog("GPi: Exception = {}".format(e))
            return False

        finally:
            os.system("mount -o remount,ro /boot")

        return True


    def UninstallSoftware(self, case):

        try:
            # Switch back to default theme
            os.system("sed -E 's/name=|ThemeSet| value=|.*|/name=|ThemeSet| value=|recalbox-next|/g' /recalbox/share/system/.emulationstation/es_settings.cfg".replace('|', '"'))
            logger.hardlog("GPi: GPi-case theme uninstalled")

            # Remove boot image
            os.system("rm -f /boot/boot.ppm")
            logger.hardlog("GPi: GPi-case boot image uninstalled")

            # Uninstall GPi pad
            os.system("rm -f /recalbox/share/system/.emulationstation/es_input.cfg.gpi")
            os.system("mv /recalbox/share/system/.emulationstation/es_input.cfg /recalbox/share/system/.emulationstation/es_input.cfg.gpi")
            os.system("mv /recalbox/share/system/.emulationstation/es_input.cfg.org /recalbox/share/system/.emulationstation/es_input.cfg")
            logger.hardlog("GPi: Controller uninstalled")

            # Re-enable kodi
            os.system("sed -i -E 's/kodi.enabled=.*/kodi.enabled=1/g' /recalbox/share/system/recalbox.conf")
            os.system("sed -i -E 's/kodi.xbutton=.*/kodi.xbutton=1/g' /recalbox/share/system/recalbox.conf")
            logger.hardlog("GPi: KODI enabled")

            # Re-enable virtual gamepad
            os.system("sed -i -E 's/system.virtual-gamepads.enabled=.*/system.virtual-gamepads.enabled=1/g' /recalbox/share/system/recalbox.conf")
            logger.hardlog("GPi: Virtual Gamepad enabled")

            # Re-enable updates
            os.system("sed -i -E 's/updates.enabled=.*/updates.enabled=1/g' /recalbox/share/system/recalbox.conf")
            logger.hardlog("GPi: Updates enabled")

            # Re-enable netplay
            os.system("sed -i -E 's/netplay.enabled=.*/netplay.enabled=1/g' /recalbox/share/system/recalbox.conf")
            logger.hardlog("GPi: Netplay enabled")

            # Re-enable webmanager
            os.system("sed -i -E 's/system.manager.enabled=.*/system.manager.enabled=1/g' /recalbox/share/system/recalbox.conf")
            logger.hardlog("GPi: Webmanager enabled")

            # Re-enable other controller
            os.system("sed -i -E 's/controllers.xarcade.enabled=.*/controllers.xarcade.enabled=1/g' /recalbox/share/system/recalbox.conf")
            os.system("sed -i -E 's/controllers.bluetooth.enabled=.*/controllers.bluetooth.enabled=1/g' /recalbox/share/system/recalbox.conf")
            os.system("sed -i -E 's/controllers.ps3.enabled=.*/controllers.ps3.enabled=1/g' /recalbox/share/system/recalbox.conf")
            logger.hardlog("GPi: Other controllers enabled")

            # Reset hostname
            os.system("sed -i -E 's/system.hostname=.*/system.hostname=RECALBOX/g' /recalbox/share/system/recalbox.conf")
            logger.hardlog("GPi: Reset RECALBOX hostname")

            # Enable music popups
            os.system("sed -i -E 's/name=|MusicPopupTime| value=|.*|/name=|MusicPopupTime| value=|3|/g' /recalbox/share/system/.emulationstation/es_settings.cfg".replace('|', '"'))
            logger.hardlog("GPi: Music popup enabled")

        except Exception as e:
            logger.hardlog("GPi: Exception = {}".format(e))
            return case

        return ""


    def GetInstallScript(self, case):

        return self.BASE_SOURCE_FOLDER + "assets/phaseInstaller.sh"
