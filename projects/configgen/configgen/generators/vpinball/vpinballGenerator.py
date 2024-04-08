import os
import configparser
import shutil
from configgen.Command import Command
import configgen.recalboxFiles as recalboxFiles
from configgen.Emulator import Emulator
from configgen.controllers.controller import ControllerPerPlayer
from configgen.generators.Generator import Generator
from configgen.generators.vpinball.vpinballControllers import VpinballControllers
from configgen.settings.keyValueSettings import keyValueSettings
from configgen.utils.recallog import recallog


class VpinballGenerator(Generator):
    def get_b2s_size(self, xsize):
        size = [290, 75]
        width = size[0] * xsize
        height = size[1] * xsize
        return width, height

    def get_b2s_x(self, scr1Res, scr2Res, b2sXsize):
        windowx = (int(scr1Res) + (int(scr2Res) / 2)) - (int(b2sXsize / 2))
        return windowx

    def generate(self, system: Emulator, playersControllers: ControllerPerPlayer, recalboxOptions: keyValueSettings, args) -> Command:
        """
        Load, override keys and save back emulator's configuration file
        This way, any modification is kept accross emulator launhes
        """

        conf = keyValueSettings(recalboxFiles.recalboxConf)
        conf.loadFile(True)
        have_custom = False

        if not os.path.exists(recalboxFiles.vpinballRootFolder):
            os.makedirs(recalboxFiles.vpinballRootFolder)

        if not os.path.exists(recalboxFiles.vpinballConfigFile):
            shutil.copy(
                "/usr/bin/vpinball/assets/Default_VPinballX.ini",
                recalboxFiles.vpinballConfigFile,
            )

        if os.path.exists(recalboxFiles.vpinballRootFolder + '/VPinballX-custom.ini'):
            recallog("*** Using Custom VPinballX.ini file ***")
            have_custom = True
            recalboxFiles.vpinballConfigFile = recalboxFiles + '/VPinballX-custom.ini'

        if not have_custom:
            try:
                vpinballSettings = configparser.ConfigParser(
                    strict=False, allow_no_value=True, empty_lines_in_values=False
                )
                vpinballSettings.optionxform = str
                vpinballSettings.read(recalboxFiles.vpinballConfigFile)
            except configparser.DuplicateOptionError:
                recallog("*** Using default VPinballX.ini file ***")
                shutil.copy(
                    "/usr/bin/vpinball/assets/Default_VPinballX.ini",
                    recalboxFiles.vpinballConfigFile,
                )

            if not vpinballSettings.has_section("Standalone"):
                vpinballSettings.add_section("Standalone")
            if not vpinballSettings.has_section("Player"):
                vpinballSettings.add_section("Player")

            soundOutput = conf.getString("pinball.output", "-1")
            ballTrail = int(conf.getString("pinball.trail", "0"))
            cabinetMode = int(conf.getString("pinball.cabinet", "1"))
            b2sSize = int(conf.getString("pinball.b2sxsize", "1"))
            screen1Resolution = conf.getString("system.externalscreen.forceresolution", "1920x1080").split("x")
            screen2Resolution = conf.getString("pinball.screen2.resolution", "$").split("x")
            screen2 = conf.getString("pinball.screen2", "$")
            profile = conf.getString("pinball.profile", "1")

            if soundOutput != "-1":
                vpinballSettings.set("Player", "SoundDevice", soundOutput)
                vpinballSettings.set("Player", "SoundDeviceBG", soundOutput)
            else:
                vpinballSettings.set("Player", "SoundDevice", "")
                vpinballSettings.set("Player", "SoundDeviceBG", "")

            if bool(ballTrail):
                vpinballSettings.set("Player", "BallTrail", "1")
                vpinballSettings.set("Player", "BallTrailStrength", "0.5")
            else:
                vpinballSettings.set("Player", "BallTrail", "")
                vpinballSettings.set("Player", "BallTrailStrength", "")

            vpinballSettings.set("Player", "BGSet", "0")
            vpinballSettings.set("Standalone", "PinMAMEWindow", "")
            vpinballSettings.set("Standalone", "PinMAMEWindowX", "")
            vpinballSettings.set("Standalone", "PinMAMEWindowY", "")
            vpinballSettings.set("Standalone", "PinMAMEWindowWidth", "0")
            vpinballSettings.set("Standalone", "PinMAMEWindowHeight", "0")
            vpinballSettings.set("Standalone", "B2SHideB2SDMD", "1")
            vpinballSettings.set("Standalone", "B2SHideDMD", "1")
            vpinballSettings.set("Standalone", "B2SWindows", "1")
            vpinballSettings.set("Standalone", "B2SBackglassX", "")
            vpinballSettings.set("Standalone", "B2SBackglassY", "")
            vpinballSettings.set("Standalone", "B2SBackglassWidth", "0")
            vpinballSettings.set("Standalone", "B2SBackglassHeight", "0")

            if screen2 != "$":
                vpinballSettings.set("Standalone", "B2SBackglassX", screen1Resolution[0])
                vpinballSettings.set("Standalone", "B2SBackglassY", "0")
                vpinballSettings.set("Standalone", "B2SBackglassWidth", screen2Resolution[0])
                vpinballSettings.set("Standalone", "B2SBackglassHeight", screen2Resolution[1])

            if bool(cabinetMode) and system.Rotation.value > 0:
                if b2sSize != 1:
                    xB2sSize = self.get_b2s_size(b2sSize)
                else:
                    xB2sSize = [290, 75]
                B2sWindowX = self.get_b2s_x(screen1Resolution[0], screen2Resolution[0], xB2sSize[0])
                vpinballSettings.set("Player", "BGSet", "1")
                vpinballSettings.set("Standalone", "PinMAMEWindowX", str(int(B2sWindowX)))
                vpinballSettings.set("Standalone", "PinMAMEWindowY", str(int(screen2Resolution[1]) * 0.8))
                vpinballSettings.set("Standalone", "PinMAMEWindowWidth", str(xB2sSize[0]))
                vpinballSettings.set("Standalone", "PinMAMEWindowHeight", str(xB2sSize[1]))
                vpinballSettings.set("Standalone", "B2SWindows", "1")

            if profile == "1":
                vpinballSettings.set("Player", "FXAA", "0")
                vpinballSettings.set("Player", "Sharpen", "0")
                vpinballSettings.set("Player", "DisableAO", "1")
                vpinballSettings.set("Player", "DynamicAO", "0")
                vpinballSettings.set("Player", "SSRefl", "0")
                vpinballSettings.set("Player", "PFReflection", "3")
                vpinballSettings.set("Player", "ForceAnisotropicFiltering", "0")
                vpinballSettings.set("Player", "AlphaRampAccuracy", "5")
            elif profile == "2":
                vpinballSettings.set("Player", "FXAA", "")
                vpinballSettings.set("Player", "Sharpen", "")
                vpinballSettings.set("Player", "DisableAO", "")
                vpinballSettings.set("Player", "DynamicAO", "")
                vpinballSettings.set("Player", "SSRefl", "")
                vpinballSettings.set("Player", "PFReflection", "")
                vpinballSettings.set("Player", "ForceAnisotropicFiltering", "")
                vpinballSettings.set("Player", "AlphaRampAccuracy", "")
            elif profile == "3":
                vpinballSettings.set("Player", "FXAA", "3")
                vpinballSettings.set("Player", "Sharpen", "2")
                vpinballSettings.set("Player", "DisableAO", "0")
                vpinballSettings.set("Player", "DynamicAO", "1")
                vpinballSettings.set("Player", "SSRefl", "1")
                vpinballSettings.set("Player", "PFReflection", "5")
                vpinballSettings.set("Player", "ForceAnisotropicFiltering", "1")
                vpinballSettings.set("Player", "AlphaRampAccuracy", "10")

            with open(recalboxFiles.vpinballConfigFile, "w") as configfile:
                vpinballSettings.write(configfile)

        # Default args
        arguments = [
            "-PrefPath",
            recalboxFiles.vpinballRootFolder,
            "-Ini",
            recalboxFiles.vpinballConfigFile,
            "-play",
        ]

        # Rom
        arguments.append(args.rom)

        commandArray = [recalboxFiles.recalboxBins[system.Emulator]]
        commandArray.extend(arguments)

        if system.HasArgs:
            commandArray.extend(system.Args)

        return Command(videomode=system.VideoMode, array=commandArray)
