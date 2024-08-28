#!/usr/bin/env python
import shutil
import os.path
import configparser
from configgen.Command import Command
from configgen.Emulator import Emulator
from configgen.generators.Generator import Generator, ControllerPerPlayer
import configgen.recalboxFiles as recalboxFiles
import configgen.generators.xemu.xemuConfig as xemuConfig
from configgen.settings.keyValueSettings import keyValueSettings

class XemuGenerator(Generator):

    # Main entry of the module
    # Configure fba and return a command
    def generate(self, system: Emulator, playersControllers: ControllerPerPlayer, recalboxOptions: keyValueSettings, args):
        xemuConfig.writeIniFile(system,args)

        # copy the hdd if it doesn't exist
        if not os.path.exists("/recalbox/share/saves/xbox/xbox_hdd.qcow2"):
            if not os.path.exists("/recalbox/share/saves/xbox"):
                os.makedirs("/recalbox/share/saves/xbox")
            shutil.copyfile("/recalbox/share_init/xemu/data/xbox_hdd.qcow2", "/recalbox/share/saves/xbox/xbox_hdd.qcow2")

        # the command to run
        commandArray = [recalboxFiles.recalboxBins[system.Emulator]]
        commandArray.extend(["-config_path", recalboxFiles.xemuConfig])

        environment = {
            "XDG_CONFIG_HOME": recalboxFiles.CONF,
            
        }

        return Command(videomode=system.VideoMode,array=commandArray, env=environment)
