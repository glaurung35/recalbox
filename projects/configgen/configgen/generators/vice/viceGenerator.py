#!/usr/bin/env python
import configgen.Command as Command
import configgen.recalboxFiles as recalboxFiles
from configgen.generators.Generator import Generator


class ViceGenerator(Generator):
    # Main entry of the module
    # Return command
    def generate(self, system, playersControllers, recalboxSettings, args):

        commandArray = [recalboxFiles.recalboxBins[system.config['emulator']],
                        "-config", recalboxFiles.viceConfig,
                        "-autostart", args.rom]
        if 'args' in system.config and system.config['args'] is not None:
            commandArray.extend(system.config['args'])

        return Command.Command(videomode='default', array=commandArray, env={"SDL_VIDEO_GL_DRIVER": "/usr/lib/libGLESv2.so"})
