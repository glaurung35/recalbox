from typing import Dict

from configgen.Command import Command
import configgen.recalboxFiles as recalboxFiles
from configgen.Emulator import Emulator
from configgen.generators.Generator import Generator
from configgen.controllers.controller import ControllerPerPlayer
from configgen.settings.keyValueSettings import keyValueSettings
from configgen.controllers.inputItem import InputItem
import os

class DevilutionxGenerator(Generator):

    def generate(self, system: Emulator, playersControllers: ControllerPerPlayer, recalboxOptions: keyValueSettings, args) -> Command:

        """
        Load, override keys and save back emulator's configuration file
        This way, any modification is kept across emulator launches
        """

        # Command line arguments
        root_dir = os.path.dirname(args.rom)
        commandArray = [recalboxFiles.recalboxBins[system.Emulator], "--data-dir", root_dir, "--save-dir", root_dir, "--config-dir", root_dir]

        # Add extra arguments
        if system.HasArgs: commandArray.extend(system.Args)

        return Command(videomode=system.VideoMode, array=commandArray)
