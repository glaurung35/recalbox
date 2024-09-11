#!/usr/bin/env python
from configgen.Command import Command
from configgen.Emulator import Emulator
from configgen.controllers.controller import Controller, ControllerPerPlayer
from configgen.generators.Generator import Generator
from configgen.settings.keyValueSettings import keyValueSettings
import configgen.recalboxFiles as recalboxFiles


class IkemenGenerator(Generator):

    # Main entry of the module
    # Return scummvm command
    def generate(self, system: Emulator, playersControllers: ControllerPerPlayer, recalboxOptions: keyValueSettings, args) -> Command:

        # Rom
        arguments = []
        arguments.append(args.rom)

        commandArray = [recalboxFiles.recalboxBins[system.Emulator]]
        commandArray.extend(arguments)

        if system.HasArgs:
            commandArray.extend(system.Args)

        return Command(videomode=system.VideoMode, array=commandArray)
