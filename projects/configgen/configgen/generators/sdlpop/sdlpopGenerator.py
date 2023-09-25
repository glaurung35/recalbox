from configgen.Command import Command
import configgen.recalboxFiles as recalboxFiles
from configgen.Emulator import Emulator
from configgen.generators.Generator import Generator, ControllerPerPlayer
from configgen.settings.keyValueSettings import keyValueSettings


class SdlpopGenerator(Generator):

    def generate(self, system: Emulator, playersControllers: ControllerPerPlayer, recalboxOptions: keyValueSettings, args) -> Command:

        """
        Load, override keys and save back emulator's configuration file
        This way, any modification is kept across emulator launches
        """

        from configgen.settings.iniSettings import IniSettings
        settings = IniSettings(recalboxFiles.sdlpopConfigFile, True)
        settings.loadFile(True)\
                .defineBool('true', 'false')

        settings.setString("General", "start_fullscreen", "true")            # Fullscreen
        settings.setString("CustomGameplay", "start_minutes_left", "-1")     # Disable 60 minutes time limit

        # for index in playersControllers:
        #     controller = playersControllers[index]
        #     if controller.PlayerIndex == 1:
        #         settings.setString("joydev1", controller.DeviceName)
        #         if controller.HasHotkey: settings.setInt("joyhkey1",  controller.Hotkey.Id)
        #         if controller.HasStart:  settings.setInt("joystart1", controller.Start.Id)
        #         if controller.HasA:      settings.setInt("joya1",     controller.A.Id)
        #         if controller.HasB:      settings.setInt("joyb1",     controller.B.Id)
        #         if controller.HasX:      settings.setInt("joyx1",     controller.X.Id)
        #         if controller.HasY:      settings.setInt("joyy1",     controller.Y.Id)
        #     if controller.PlayerIndex == 2:
        #         settings.setString("joydev2", controller.DeviceName)
        #         if controller.HasHotkey: settings.setInt("joyhkey2",  controller.Hotkey.Id)
        #         if controller.HasStart:  settings.setInt("joystart2", controller.Start.Id)
        #         if controller.HasA:      settings.setInt("joya2",     controller.A.Id)
        #         if controller.HasB:      settings.setInt("joyb2",     controller.B.Id)
        #         if controller.HasX:      settings.setInt("joyx2",     controller.X.Id)
        #         if controller.HasY:      settings.setInt("joyy2",     controller.Y.Id)

        settings.saveFile()

        commandArray = [recalboxFiles.recalboxBins[system.Emulator], args.rom]

        if system.HasArgs: commandArray.extend(system.Args)

        return Command(videomode=system.VideoMode, array=commandArray)
