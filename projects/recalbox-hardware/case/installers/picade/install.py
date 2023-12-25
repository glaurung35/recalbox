import os
import logger
from installers.base.install import InstallBase
from settings import keyValueSettings

class Install(InstallBase):

    BASE_SOURCE_FOLDER = InstallBase.BASE_SOURCE_FOLDER + "picade/"
    DTBO_FILE = "/boot/overlays/picade.dtbo"
    SOUND_FILE = "/etc/asound.conf"
    UDEV_FILE = "/etc/udev/rules.d/10-picade.rules"
    RECALBOX_CONF = "/recalbox/share/system/recalbox.conf"

    def __init__(self):
        InstallBase.__init__(self)

    def InstallHardware(self, case):
        logger.hardlog("Installing Picade Case hardware")
        try:
            os.system("mount -o remount,rw /")
            os.system("mount -o remount,rw /boot")

            # Install /boot/recalbox-user-config.txt
            if os.system('echo -e "\ndtoverlay=picade\ndtoverlay=hifiberry-dac\nforce_eeprom_read=0\ndtparam=up=\ndtparam=down=\ndtparam=left=\ndtparam=right=\ndtparam=button1=\ndtparam=button2=\ndtparam=button3=\ndtparam=button4=\ndtparam=button5=\ndtparam=button6=\ndtparam=enter=\n# dtparam=escape=\ndtparam=coin=\ndtparam=start=\n" >> /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error editing /boot/recalbox-user-config.txt")
                return False
            logger.hardlog("Picade: recalbox-user-config.txt installed")

            # Install /boot/overlays/picade.dtbo
            if os.system('cp {}assets/picade.dtbo {}'.format(self.BASE_SOURCE_FOLDER, self.DTBO_FILE)) != 0:
                logger.hardlog("Picade: error copying picade.dtbo")
                return False
            logger.hardlog("Picade: picade.dtbo installed")

            # Install /etc/asound.conf
            if os.system('cp {}assets/asound.conf {}'.format(self.BASE_SOURCE_FOLDER, self.SOUND_FILE)) != 0:
                logger.hardlog("Picade: error copying asound.conf")
                return False
            logger.hardlog("Picade: asound.conf installed")

            # Install /etc/udev/rules.d/10-picade.rules
            if os.system('cp {}assets/10-picade.rules {}'.format(self.BASE_SOURCE_FOLDER, self.UDEV_FILE)) != 0:
                logger.hardlog("Picade: error copying 10-picade.rules")
                return False
            logger.hardlog("Picade: 10-picade.rules installed")

        except Exception as e:
            logger.hardlog("Picade: Exception = {}".format(e))
            return False

        finally:
            os.system("mount -o remount,ro /")
            os.system("mount -o remount,ro /boot")

        logger.hardlog("Picade Case hardware installed successfully!")
        return True

    def InstallSoftware(self, case):
        logger.hardlog("Installing Picade Case software")
        try:
            recalboxConf = keyValueSettings(self.RECALBOX_CONF, False)
            recalboxConf.loadFile()

            # Switch audio device
            recalboxConf.setOption("audio.device", "alsa_card.platform-soc_sound:analog-output:output:stereo-fallback")
            recalboxConf.saveFile()
            logger.hardlog("Picade: set audio device")

            # Enable GPIO controllers
            recalboxConf.setOption("controllers.gpio.enabled", "1")
            recalboxConf.saveFile()
            logger.hardlog("Picade: enable gpio controllers")

            # Define default GPIO controllers arguments
            recalboxConf.setOption("controllers.gpio.args", "map=5 gpio=12,6,20,16,24,23,9,25,10,11,5,8,27")
            recalboxConf.saveFile()
            logger.hardlog("Picade: set gpio controllers arguments")

        except Exception as e:
            logger.hardlog("Picade: Exception = {}".format(e))
            return ""

        finally:
            os.system("mount -o remount,ro /")
            os.system("mount -o remount,ro /boot")

        logger.hardlog("Picade Case software installed successfully!")
        return case

    def UninstallHardware(self, case):
        logger.hardlog("Uninstalling Picade Case hardware")
        try:
            os.system("mount -o remount,rw /")
            os.system("mount -o remount,rw /boot")

            # Uninstall /boot/recalbox-user-config.txt
            if os.system('sed -i "/dtoverlay=picade/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtoverlay")
                return False
            if os.system('sed -i "/dtoverlay=hifiberry-dac/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtoverlay")
                return False
            if os.system('sed -i "/force_eeprom_read=0/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing force_eeprom_read")
                return False
            if os.system('sed -i "/dtparam=up=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam up")
                return False
            if os.system('sed -i "/dtparam=down=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam down")
                return False
            if os.system('sed -i "/dtparam=left=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam left")
                return False
            if os.system('sed -i "/dtparam=right=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam right")
                return False
            if os.system('sed -i "/dtparam=button1=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam button1")
                return False
            if os.system('sed -i "/dtparam=button2=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam button2")
                return False
            if os.system('sed -i "/dtparam=button3=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam button3")
                return False
            if os.system('sed -i "/dtparam=button4=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam button4")
                return False
            if os.system('sed -i "/dtparam=button5=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam button5")
                return False
            if os.system('sed -i "/dtparam=button6=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam button6")
                return False
            if os.system('sed -i "/dtparam=enter=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam enter")
                return False
            if os.system('sed -i "/# dtparam=escape=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam escape")
                return False
            if os.system('sed -i "/dtparam=coin=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam coin")
                return False
            if os.system('sed -i "/dtparam=start=/d" /boot/recalbox-user-config.txt') != 0:
                logger.hardlog("Picade: error removing dtparam start")
                return False
            logger.hardlog("Picade: recalbox-user-config.txt uninstalled")

            # Uninstall /boot/overlays/picade.dtbo
            if os.system("rm -f {}".format(self.DTBO_FILE)) != 0:
                logger.hardlog("Picade: error removing picade.dtbo")
                return False
            logger.hardlog("Picade: picade.dtbo uninstalled")

            # Uninstall /etc/asound.conf
            if os.system("rm -f {}".format(self.SOUND_FILE)) != 0:
                logger.hardlog("Picade: error removing asound.conf")
                return False
            logger.hardlog("Picade: asound.conf uninstalled")

            # Uninstall /etc/udev/rules.d/10-picade.rules
            if os.system("rm -f {}".format(self.UDEV_FILE)) != 0:
                logger.hardlog("Picade: error removing 10-picade.rules")
                return False
            logger.hardlog("Picade: 10-picade.rules uninstalled")

        except Exception as e:
            logger.hardlog("Picade: Exception = {}".format(e))
            return False

        finally:
            os.system("mount -o remount,ro /")
            os.system("mount -o remount,ro /boot")

        return True

    def UninstallSoftware(self, case):
        logger.hardlog("Uninstalling Picade Case software")
        try:
            # Load recalbox.conf
            recalboxConf = keyValueSettings(self.RECALBOX_CONF, False)
            recalboxConf.loadFile()

            # Disable GPIO controllers
            recalboxConf.setOption("controllers.gpio.enabled", "0")
            recalboxConf.saveFile()
            logger.hardlog("Picade: GPIO controllers disabled")

        except Exception as e:
            logger.hardlog("Picade: Exception = {}".format(e))
            return False

        finally:
            os.system("mount -o remount,ro /")
            os.system("mount -o remount,ro /boot")

        return True

    def GetInstallScript(self, case):

        return None;
