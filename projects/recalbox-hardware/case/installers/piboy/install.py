from enum import Enum
import os
import logger
import shutil
import subprocess
from filemanipulation import sed, stripline
from installers.base.install import InstallBase
from settings import keyValueSettings

# status flags
# ex: 1100 0110
#     ||    |\-VSTAT1
#     ||    \-VSTAT2
#     |\-power button (0 -> off position, 1 -> on position)
#     \-VBus


class PiBoy(Enum):
    PiBoyDMG = 0
    PiBoyXRS = 1


class Install(InstallBase):

    BASE_SOURCE_FOLDER = InstallBase.BASE_SOURCE_FOLDER + "piboy/"
    RECALBOX_CONF = "/recalbox/share/system/recalbox.conf"

    def __init__(self):
        InstallBase.__init__(self)

    def InstallHardware(self, case):

        logger.hardlog("Installing PiBoy DMG/XRS hardware")
        # load module now, or piboy may shutdown
        subprocess.run(['modprobe', 'xpi_gamecon', f"xpicase={PiBoy[case].value}"])

        try:
            os.system("mount -o remount,rw /boot")
            os.system("mount -o remount,rw /")
            files = {
                '/boot/recalbox-user-config.txt': '/boot/recalbox-user-config.txt.backup',
                self.BASE_SOURCE_FOLDER + 'assets/recalbox-user-config.txt': '/boot/recalbox-user-config.txt',
                self.BASE_SOURCE_FOLDER + 'assets/piboy.ppm': '/boot/boot.ppm',
            }
            for source_file, dest_file in files.items():
                installed_file = shutil.copy(source_file, dest_file)
                logger.hardlog(f"PiBoy: {installed_file} installed")

            sed('\s*video=[^ ]+', '', '/boot/cmdline.txt')
            if case == "PiBoyDMG":
                disable_video_id = 1
            elif case == "PiBoyXRS":
                disable_video_id = 2
            sed('noswap', f"noswap video=HDMI-A-{disable_video_id}:d xpi_gamecon.xpicase={PiBoy[case].value}", '/boot/cmdline.txt')
            logger.hardlog(f"PiBoy: set video parameter in cmdline.txt and xpicase value to {PiBoy[case].value}")
        except Exception as e:
            logger.hardlog("PiBoy: Exception = {}".format(e))
            return False

        finally:
            # write 129 to flags in order to reboot properly (instead of shutdown)
            with open("/sys/kernel/xpi_gamecon/flags", "w") as xpiflags:
                xpiflags.write("129\n")

        logger.hardlog("PiBoy DMG hardware installed successfully!")
        return True

    def UninstallHardware(self, case):

        try:
            os.system("mount -o remount,rw /boot")
            os.system("mount -o remount,rw /")
            # Uninstall /boot/recalbox-user-config.txt
            if os.system("cp /boot/recalbox-user-config.txt.backup /boot/recalbox-user-config.txt") != 0:
                logger.hardlog("PiBoy: Error uninstalling recalbox-user-config.txt")
                return False
            logger.hardlog("PiBoy: recalbox-user-config.txt uninstalled")
            os.remove("/boot/boot.ppm")
            logger.hardlog("PiBoy: /boot/boot.ppm uninstalled")
            sed(' video=HDMI-A-.:d', '', '/boot/cmdline.txt')
            sed(' xpi_gamecon.xpicase=[0-1]', '', '/boot/cmdline.txt')
            logger.hardlog("PiBoy: removed video setting in cmdline.txt")

        except Exception as e:
            logger.hardlog("PiBoy: Exception = {}".format(e))
            return False

        finally:
            os.system("mount -o remount,ro /boot")
            os.system("mount -o remount,ro /")

        return True

    def InstallSoftware(self, case):

        if case in ("PiBoyDMG", "PiBoyXRS"):

            logger.hardlog("Installing PiBoy DMG/XRS software")

            try:
                os.system("mount -o remount,rw /")
                # Load recalbox.conf
                recalboxConf = keyValueSettings(self.RECALBOX_CONF, False)
                recalboxConf.loadFile()

                # Set powerswitch.sh config
                recalboxConf.setOption("system.power.switch", "PIBOY")
                logger.hardlog("PiBoy: powerswitch configured")
                # Set wpaf config
                recalboxConf.setOption("hat.wpaf.enabled", "1")
                recalboxConf.setOption("hat.wpaf.board", "piboy")
                logger.hardlog("PiBoy: wpaf configured")
                recalboxConf.setOption("emulationstation.theme.folder", "recalbox-goa2")
                logger.hardlog("PiBoy: theme set to recalbox-goa2")
                recalboxConf.setOption("audio.device", "alsa_card.platform-bcm2835_audio.2:analog-output-headphones")
                logger.hardlog("PiBoy: audio.device set to alsa_card.platform-bcm2835_audio.2:analog-output-headphones")
                recalboxConf.saveFile()
                # Force default videomode
                sed(
                    "([a-zA-Z0-9.].videomode)\\s*=.*",
                    "\\1=default",
                    self.RECALBOX_CONF,
                )
                # Install /etc/init.d/S06volumed
                sourceConfig = self.BASE_SOURCE_FOLDER + "assets/S06volumed"
                if os.system("cp {} /etc/init.d/".format(sourceConfig)) != 0:
                    logger.hardlog("PiBoy: Error installing S06volumed")
                    return ""
                logger.hardlog("PiBoy: S06volumed installed")

                # Install /etc/init.d/S15piboyswitch
                sourceConfig = self.BASE_SOURCE_FOLDER + "assets/S15piboyswitch"
                if os.system("cp {} /etc/init.d/".format(sourceConfig)) != 0:
                    logger.hardlog("PiBoy: Error installing S15piboyswitch")
                    return ""
                logger.hardlog("PiBoy: S15piboyswitch installed")

                # Install /recalbox/share/.retroarch.cfg
                sourceConfig = self.BASE_SOURCE_FOLDER + "assets/piboy-retroarch.cfg"
                if os.system("cp {} /recalbox/share/.retroarch.cfg".format(sourceConfig)) != 0:
                    logger.hardlog("PiBoy: Error installing .retroarch.cfg")
                    return ""
                logger.hardlog("PiBoy: .retroarch.cfg installed")

                # start volumed service for volume wheel to work properly
                os.system("/etc/init.d/S06volumed start")
                # start S15piboyswitch to manage lcd display and theme
                os.system("/etc/init.d/S15piboyswitch start")

            except Exception as e:
                logger.hardlog("PiBoy: Exception = {}".format(e))
                return ""

            finally:
                os.system("mount -o remount,ro /")

            logger.hardlog("PiBoy DMG/XRS software installed successfully!")
            return case

        return ""

    def UninstallSoftware(self, case):

        try:
            os.system("mount -o remount,rw /")
            os.remove("/recalbox/share/.retroarch.cfg")
            logger.hardlog("PiBoy: /recalbox/share/.retroarch.cfg uninstalled")

        except Exception as e:
            logger.hardlog("PiBoy: Exception = {}".format(e))

        try:
            os.system("mount -o remount,rw /")
            os.remove("/etc/init.d/S06volumed")
            logger.hardlog("PiBoy: /etc/init.d/S06volumed uninstalled")
            os.remove("/etc/init.d/S15piboyswitch")
            logger.hardlog("PiBoy: /etc/init.d/S15piboyswitch uninstalled")
            # Load recalbox.conf
            recalboxConf = keyValueSettings(self.RECALBOX_CONF, False)
            recalboxConf.loadFile()

            # Remove powerswitch.sh config
            recalboxConf.removeOption("system.power.switch")
            # Remove wpaf config
            recalboxConf.setOption("hat.wpaf.enabled", "0")
            recalboxConf.removeOption("hat.wpaf.board")
            recalboxConf.saveFile()
            logger.hardlog("PiBoy: powerswitch unconfigured")

        except Exception as e:
            logger.hardlog("PiBoy: Exception = {}".format(e))
            return case

        finally:
            os.system("mount -o remount,ro /")

        return ""

    def GetInstallScript(self, case):

        return None
