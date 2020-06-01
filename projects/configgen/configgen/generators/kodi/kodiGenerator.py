#!/usr/bin/env python
import Command
import recalboxFiles
from generators.Generator import Generator
import kodiConfig

class KodiGenerator(Generator):
    # Main entry of the module
    # Configure kodi inputs and return the command to run
    def generate(self, system, rom, playersControllers, demo, nodefaultkeymap, recalboxSettings):
        if not system.config['configfile']:
            kodiConfig.writeKodiControllersConfig(playersControllers)
        commandArray = [recalboxFiles.recalboxBins[system.config['emulator']]]
        return Command.Command(videomode=system.config['videomode'], array=commandArray)
