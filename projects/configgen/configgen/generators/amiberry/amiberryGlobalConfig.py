#!/usr/bin/env python
import os

import recalboxFiles
from generators.amiberry.amiberryKickstarts import KickstartManager
from settings.keyValueSettings import keyValueSettings


class AmiberryGlobalConfig:

    def __init__(self, userGlobalSettingsFile, finalGlobalSettingsFile):
        self.globalSettingsFile = finalGlobalSettingsFile
        self.userSettingsFile = userGlobalSettingsFile

    def createGlobalSettings(self):
        # Load user settings first
        settings = keyValueSettings(self.userSettingsFile)
        settings.loadFile(True)

        # Set default settings if they do not exists
        settings.setDefaultOption("read_config_descriptions", "yes")
        settings.setDefaultOption("write_logfile", "yes")
        settings.setDefaultOption("scanlines_by_default", "no")
        settings.setDefaultOption("speedup_cycles_jit_pal", "10000")
        settings.setDefaultOption("speedup_cycles_jit_ntsc", "6667")
        settings.setDefaultOption("speedup_cycles_nonjit", "256")
        settings.setDefaultOption("speedup_timelimit_jit", "-5000")
        settings.setDefaultOption("speedup_timelimit_nonjit", "-5000")
        settings.setDefaultOption("speedup_timelimit_jit_turbo", "0")
        settings.setDefaultOption("speedup_timelimit_nonjit_turbo", "0")

        # Forced values
        settings.setOption("Quickstart", "1")
        settings.setOption("path", recalboxFiles.amiberryMountPoint)
        settings.setOption("config_path", recalboxFiles.amiberryMountPoint + "/conf")
        settings.setOption("controllers_path", recalboxFiles.amiberryMountPoint + "/conf")
        settings.setOption("retroarch_config", recalboxFiles.amiberryMountPoint + "/conf/retroarch.cfg")
        settings.setOption("rom_path", recalboxFiles.BIOS + '/')

        # Save file
        settings.changeSettingsFile(self.globalSettingsFile)
        settings.saveFile()

        # Hack the settings to add non-unique key/value tupples
        with open(self.globalSettingsFile, "a") as sf:
            sf.write("ROMs=" + str(len(KickstartManager.BIOS_LIST)) + '\n')
            for rom in KickstartManager.BIOS_LIST:
                itype, name = KickstartManager.BIOS_LIST[rom]
                path = os.path.join(recalboxFiles.BIOS, rom + ".rom")
                sf.write("ROMName=" + name + '\n')
                sf.write("ROMPath=" + path + '\n')
                sf.write("ROMType=" + str(itype) + '\n')
