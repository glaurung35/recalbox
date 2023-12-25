import os
import logger
from installers.base.install import InstallBase

class Install(InstallBase):

	BASE_SOURCE_FOLDER = InstallBase.BASE_SOURCE_FOLDER + "picade/"
	DTBO_FILE = "/boot/overlays/picade.dtbo"
	SOUND_FILE = "/etc/asound.conf"
	UDEV_FILE = "/etc/udev/rules.d/10-picade.rules"

	def __init__(self):
		InstallBase.__init__(self)

	def InstallHardware(self, case):
		logger.hardlog("Installing Picade Case hardware")
		try:
			os.system("mount -o remount,rw /")
			os.system("mount -o remount,rw /boot")

			if os.system('echo -e "\ndtoverlay=picade\ndtparam=up=\ndtparam=down=\ndtparam=left=\ndtparam=right=\ndtparam=button1=\ndtparam=button2=\ndtparam=button3=\ndtparam=button4=\ndtparam=button5=\ndtparam=button6=\ndtparam=enter=\n# dtparam=escape=\ndtparam=coin=\ndtparam=start=\n" >> /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error editing /boot/recalbox-user-config.txt")
				return False
			if os.system('cp {}assets/picade.dtbo {}'.format(self.BASE_SOURCE_FOLDER, self.DTBO_FILE)) != 0:
				logger.hardlog("Picade: error copying picade.dtbo")
				return False
			if os.system('cp {}assets/asound.conf {}'.format(self.BASE_SOURCE_FOLDER, self.SOUND_FILE)) != 0:
				logger.hardlog("Picade: error copying asound.conf")
				return False
			if os.system('cp {}assets/10-picade.rules {}'.format(self.BASE_SOURCE_FOLDER, self.UDEV_FILE)) != 0:
				logger.hardlog("Picade: error copying 10-picade.rules")
				return False
		except Exception as e:
			logger.hardlog("Picade: Exception = {}".format(e))
			return False

		logger.hardlog("Picade Case hardware installed successfully")
		return True

	def UninstallHardware(self, case):
		logger.hardlog("Uninstalling Picade Case hardware")
		try:
			os.system("mount -o remount,rw /")
			os.system("mount -o remount,rw /boot")

			if os.system('sed -i "/dtoverlay=picade/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtoverlay")
			if os.system('sed -i "/dtparam=up=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam up")
			if os.system('sed -i "/dtparam=down=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam down")
			if os.system('sed -i "/dtparam=left=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam left")
			if os.system('sed -i "/dtparam=right=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam right")
			if os.system('sed -i "/dtparam=button1=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam button1")
			if os.system('sed -i "/dtparam=button2=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam button2")
			if os.system('sed -i "/dtparam=button3=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam button3")
			if os.system('sed -i "/dtparam=button4=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam button4")
			if os.system('sed -i "/dtparam=button5=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam button5")
			if os.system('sed -i "/dtparam=button6=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam button6")
			if os.system('sed -i "/dtparam=enter=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam enter")
			if os.system('sed -i "/# dtparam=escape=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam escape")
			if os.system('sed -i "/dtparam=coin=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam coin")
			if os.system('sed -i "/dtparam=start=/d" /boot/recalbox-user-config.txt') != 0:
				logger.hardlog("Picade: error removing dtparam start")
			if os.system("rm -f {}".format(self.DTBO_FILE)) != 0:
				logger.hardlog("Picade: error removing picade.dtbo")
			if os.system("rm -f {}".format(self.SOUND_FILE)) != 0:
				logger.hardlog("Picade: error removing asound.conf")
			if os.system("rm -f {}".format(self.UDEV_FILE)) != 0:
			    logger.hardlog("Picade: error removing 10-picade.rules")
		except Exception as e:
			logger.hardlog("Picade: Exception = {}".format(e))
		return True

	def GetInstallScript(self, case):

		return None;
