import os

from configgen.controllers.inputItem import InputItem
from configgen.settings.keyValueSettings import keyValueSettings
from configgen.generators.Generator import Generator, ControllerPerPlayer
import configgen.recalboxFiles as recalboxFiles
from configgen.Emulator import Emulator
from configgen.Command import Command
from configgen.utils.resolutions import ResolutionParser


class CreativisionGenerator(Generator):

    BIOS_PATH = "/recalbox/share/bios/creativision"

    def __BuildITem(self, name: str, base: str, item: InputItem) -> str:
        if item.IsButton:
            return base + name + "={}".format(item.Id)
        return ""

    def generate(self, system: Emulator, playersControllers: ControllerPerPlayer, recalboxOptions: keyValueSettings, args) -> Command:

        # Option array
        options: list[str] = []

        # Try identify machine folder
        defaultBios: str = "bioscv.rom" # Creativision console by default
        machines: dict[str, str] = \
        {
            "csl": "cslbios.rom",
            "cslcartridge": "cslbios.rom",
            "csl-cartridge": "cslbios.rom",
            "cslcart": "cslbios.rom",
            "csl-cart": "cslbios.rom",
            "csl-sm": "cslbiossm.rom",
            "cslcartridge-sm": "cslbiossm.rom",
            "csl-cartridge-sm": "cslbiossm.rom",
            "cslcart-sm": "cslbiosms.rom",
            "csl-cart-sm": "cslbiossm.rom",
            "cslsm": "cslbiossm.rom",
            "cslcartridgesm": "cslbiossm.rom",
            "csl-cartridgesm": "cslbiossm.rom",
            "cslcartsm": "cslbiosms.rom",
            "csl-cartsm": "cslbiossm.rom",
            "laser2001": "laser2001.rom",
            "laser-2001": "laser2001.rom",
            "salora": "saloram.rom",
            "manager": "saloram.rom",
            "saloramanager": "saloram.rom",
            "salora-manager": "saloram.rom",
            "laser": "laser2001.rom",
            "2001": "laser2001.rom",
        }
        lrom: str = args.rom.lower()
        for key in machines:
            if key in lrom:
                defaultBios = machines[key]
                break

        # Add bios
        options.append("--bios")
        options.append(os.path.join(self.BIOS_PATH, defaultBios))

        # Change machine regarding the chosen bios
        if defaultBios in ("cslbios.rom", "cslbiossm.rom"):
            options.append("-2")
        elif defaultBios in ("saloram.rom", "laser2001.rom"):
            options.append("-3")

        # Choose appropriate "rom" option
        romname, romext = os.path.splitext(args.rom)
        if romext == ".cas":
            options.append("--cassette")
        elif romext == ".dsk":
            options.append("--floppy")
        else:
            options.append("--rom")
        options.append(args.rom)

        # Always fullscreen
        options.append("--fullscreen")
        # Screen resolution
        resolution = ResolutionParser(system.VideoMode)
        if resolution.isSet and resolution.selfProcess:
            options.append("--screenwidth")
            options.append(str(resolution.width))
            options.append("--screenheight")
            options.append(str(resolution.height))

        # Gfx options
        if system.IntegerScale:
            options.append("--integerscale")
        if system.Smooth:
            options.append("--smooth")

        # controller settings
        for player, controller in playersControllers.items():
            if 1 <= player <= 2:
                base: str = "--joy{}".format(player - 1)
                options.append(base + "={}".format(controller.SdlIndex))
                if player == 1:
                    if controller.HasHotkey: options.append(self.__BuildITem("buttonHotkey", base, controller.Hotkey))
                    if controller.HasStart: options.append(self.__BuildITem("buttonStart" , base, controller.Start))
                    if controller.HasX: options.append(self.__BuildITem("buttonX", base, controller.X))
                    if controller.HasY: options.append(self.__BuildITem("buttonY", base, controller.Y))
                    if controller.HasL1: options.append(self.__BuildITem("buttonL1", base, controller.L1))
                if controller.HasA: options.append(self.__BuildITem("button0", base, controller.A))
                if controller.HasB: options.append(self.__BuildITem("button1", base, controller.B))
                if controller.HasUp: options.append(self.__BuildITem("buttonup"   , base, controller.Up))
                if controller.HasDown: options.append(self.__BuildITem("buttondown" , base, controller.Down))
                if controller.HasLeft: options.append(self.__BuildITem("buttonleft" , base, controller.Left))
                if controller.HasRight: options.append(self.__BuildITem("buttonright", base, controller.Right))

        commandArray: list[str] = [ recalboxFiles.recalboxBins[system.Emulator] ]  # Emulator
        commandArray.extend(options)
        if system.HasArgs:
            commandArray.extend(system.Args)

        return Command(videomode=system.VideoMode, array=commandArray)
