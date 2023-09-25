from typing import Dict

from configgen.Command import Command
import configgen.recalboxFiles as recalboxFiles
from configgen.Emulator import Emulator
from configgen.generators.Generator import Generator
from configgen.controllers.controller import ControllerPerPlayer
from configgen.settings.keyValueSettings import keyValueSettings
from configgen.controllers.inputItem import InputItem


class SdlpopGenerator(Generator):

    SDL_CONTROLLER_BUTTON_A: int = 0
    SDL_CONTROLLER_BUTTON_B: int = 1
    SDL_CONTROLLER_BUTTON_X: int = 2
    SDL_CONTROLLER_BUTTON_Y: int = 3
    SDL_CONTROLLER_BUTTON_BACK: int = 4
    SDL_CONTROLLER_BUTTON_GUIDE: int = 5
    SDL_CONTROLLER_BUTTON_START: int = 6
    SDL_CONTROLLER_BUTTON_LEFTSTICK: int = 7
    SDL_CONTROLLER_BUTTON_RIGHTSTICK: int = 8
    SDL_CONTROLLER_BUTTON_LEFTSHOULDER: int = 9
    SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: int = 10
    SDL_CONTROLLER_BUTTON_DPAD_UP: int = 11
    SDL_CONTROLLER_BUTTON_DPAD_DOWN: int = 12
    SDL_CONTROLLER_BUTTON_DPAD_LEFT: int = 13
    SDL_CONTROLLER_BUTTON_DPAD_RIGHT: int = 14

    SDL_CONTROLLER_AXIS_LEFTX: int = 0
    SDL_CONTROLLER_AXIS_LEFTY: int = 1
    SDL_CONTROLLER_AXIS_RIGHTX: int = 2
    SDL_CONTROLLER_AXIS_RIGHTY: int = 3
    SDL_CONTROLLER_AXIS_TRIGGERLEFT: int = 4
    SDL_CONTROLLER_AXIS_TRIGGERRIGHT: int = 5

    __HatToDirections: Dict[int, str] = \
    {
        1: "Up",
        2: "Right",
        3: "Down",
        4: "Left"
    }

    @staticmethod
    def __BuildAxis(inp: int, sign: int = 0) -> str:
        return "{}Axis{}".format(('+' if sign > 0 else ('-' if sign < 0 else '')), inp)

    @staticmethod
    def __BuildButton(self, inp: InputItem, signless: bool = False) -> str:
        if inp.IsButton: return "Button{}".format(inp.Id)
        if inp.IsAxis  : return "{}Axis{}".format(('' if signless else ('+' if int(inp.Value) >= 0 else '-')), inp.Id)
        if inp.IsHat   : return "Hat{} {}".format(inp.Id, self.__HatToDirections[int(inp.Value)])
        return "Unknown"

    def generate(self, system: Emulator, playersControllers: ControllerPerPlayer, recalboxOptions: keyValueSettings, args) -> Command:

        """
        Load, override keys and save back emulator's configuration file
        This way, any modification is kept across emulator launches
        """

        # Generate game controller database
        with open(recalboxFiles.sdlpopControllersFile, "w") as f:
            for controller in playersControllers.values():
                f.write(controller.generateSDLGameDBLine() + '\n')

        # Config file
        from configgen.settings.iniSettings import IniSettings
        settings = IniSettings(recalboxFiles.sdlpopConfigFile, True)
        settings.loadFile(True)\
                .defineBool('true', 'false')

        settings.setString("General", "start_fullscreen", "true")                                   # Fullscreen
        settings.setString("CustomGameplay", "use_custom_options", "true")                          # Use custom options - enables the next line
        settings.setString("CustomGameplay", "start_minutes_left", "-1")                            # Disable 60 minutes time limit
        settings.setString("General", "gamecontrollerdb_file", recalboxFiles.sdlpopControllersFile) # Controllers configuration

        # Save config
        settings.saveFile()

        # Command line arguments
        commandArray = [recalboxFiles.recalboxBins[system.Emulator], args.rom]

        # Add extra arguments
        if system.HasArgs: commandArray.extend(system.Args)

        return Command(videomode=system.VideoMode, array=commandArray)
