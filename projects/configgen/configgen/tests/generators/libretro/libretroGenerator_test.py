#!/usr/bin/env python

import sys
import os.path
import unittest
import shutil
import controllersConfig as controllersConfig
import settings.unixSettings as unixSettings
import recalboxFiles

from Emulator import Emulator
from generators.libretro.libretroGenerator import LibretroGenerator

import generators.libretro.libretroConfigurations as libretroConfigurations
import generators.libretro.libretroGenerator as libretroGenerator
from settings.keyValueSettings import keyValueSettings

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../..')))

RETROARCH_ORIGIN_CFG_FILE = os.path.abspath(os.path.join(os.path.dirname(__file__), 'tmp/retroarchcustomorigin.cfg'))
RETROARCH_CUSTOM_CFG_FILE = os.path.abspath(os.path.join(os.path.dirname(__file__), 'tmp/retroarchcustom.cfg'))
RETROARCH_OVERRIDE_CFG_FILE = os.path.abspath(os.path.join(os.path.dirname(__file__), 'tmp/retroarchcustom.cfg.overrides.cfg'))
RECALBOX_CFG_FILE = os.path.abspath(os.path.join(os.path.dirname(__file__), 'tmp/recalbox.conf'))
RETROARCH_CORE_CONFIG = os.path.abspath(os.path.join(os.path.dirname(__file__), "tmp/retroarchcorecustom.cfg"))

PS3UUID = "060000004c0500006802000000010000"

rom = "MyRom.nes"

libretroGen = LibretroGenerator()


class TestLibretroGenerator(unittest.TestCase):
    def setUp(self):
        self.snes = Emulator(name='snes', videomode='4', core='snes9x2002', shaders='', ratio='auto', smooth='2',
                             rewind='false', emulator='libretro')
        self.snes2 = Emulator(name='snes', videomode='4', core='snes9x2002', shaders='', ratio='auto', smooth='2',
                              rewind='false', emulator='libretro')
        self.nes = Emulator(name='nes', videomode='6', core='snes9x2005', shaders='', ratio='16/9', smooth='1',
                            rewind='false', configfile='/myconfigfile.cfg', emulator='libretro')

        # test inputs
        self.basicInputs1 = {'hotkey': controllersConfig.Input("hotkey", "button", "10", "1","")}
        self.basicController1 = controllersConfig.Controller("contr1", "joypad", "GUID1", 1, 0, "Joypad1RealName",
                                                             self.basicInputs1,
                                                             nbaxes=6, nbhats=1, nbbuttons=10)

        self.sdl2controler = controllersConfig.Controller("contr1", "joypad", "030000003512000012ab000010010000", 2, 1,
                                                          "Bluetooth Wireless Controller   ", self.basicInputs1,
                                                          nbaxes=6, nbhats=1, nbbuttons=10)
        self.controllers = dict()
        self.controllers['1'] = self.basicController1

        self.sdl2controllers = dict()
        self.sdl2controllers['1'] = self.basicController1
        self.sdl2controllers['2'] = self.sdl2controler

        shutil.copyfile(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../resources/retroarchcustom.cfg.origin')), RETROARCH_CUSTOM_CFG_FILE)
        shutil.copyfile(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../resources/retroarchcustom.cfg.origin')), RETROARCH_ORIGIN_CFG_FILE)
        shutil.copyfile(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../resources/recalbox.conf.origin')), RECALBOX_CFG_FILE)
        shutil.copyfile(os.path.abspath(os.path.join(os.path.dirname(__file__), "../../resources/retroarchcores.cfg")), RETROARCH_CORE_CONFIG)

        libretroGenerator.recalboxFiles.retroarchCustom = RETROARCH_CUSTOM_CFG_FILE
        libretroGenerator.recalboxFiles.retroarchCustomOrigin = RETROARCH_ORIGIN_CFG_FILE
        libretroConfigurations.coreSettings = unixSettings.UnixSettings(RETROARCH_CORE_CONFIG, separator=' ')
        libretroConfigurations.libretroSettings = unixSettings.UnixSettings(RETROARCH_CUSTOM_CFG_FILE, separator=' ')

    def test_generate_system_no_custom_settings(self):
        command = libretroGen.generate(self.snes, rom, dict(), False, False, keyValueSettings("", False))
        self.assertEquals(command.videomode, '4')
        self.assertEquals(command.array,
                          [recalboxFiles.recalboxBins["libretro"], '-L', '/usr/lib/libretro/snes9x2002_libretro.so', '--config',
                           RETROARCH_CUSTOM_CFG_FILE, '--appendconfig', RETROARCH_OVERRIDE_CFG_FILE,
                           #'--set-shader', 'myshaders.gpslp',
                           'MyRom.nes'])

    def test_generate_system_custom_settings(self):
        command = libretroGen.generate(self.nes, rom, dict(), False, False, keyValueSettings("", False))
        self.assertEquals(command.videomode, '6')
        self.assertEquals(command.array,
                          [recalboxFiles.recalboxBins["libretro"], '-L', '/usr/lib/libretro/snes9x2005_libretro.so', '--config', '/myconfigfile.cfg',
                           '--appendconfig', RETROARCH_OVERRIDE_CFG_FILE,
                           #'--set-shader', 'myshaders.gpslp',
                           'MyRom.nes'])

    def test_generate_forced_input_config(self):
        command = libretroGen.generate(self.nes, rom, dict(), False, False, keyValueSettings("", False))
        self.assertEquals(command.videomode, '6')
        self.assertEquals(command.array,
                          [recalboxFiles.recalboxBins["libretro"], '-L', '/usr/lib/libretro/snes9x2005_libretro.so', '--config', '/myconfigfile.cfg',
                           '--appendconfig', RETROARCH_OVERRIDE_CFG_FILE,
                           #'--set-shader', 'myshaders.gpslp',
                           'MyRom.nes'])

    def test_custom_inputdriver_override_choice(self):
        self.snes.config['inputdriver'] = 'sdl2'
        builder = libretroConfigurations.LibretroConfiguration(self.snes, self.sdl2controllers, "/rom.rom", False, False, keyValueSettings("", False))
        builder.overrideLibretroConfigurationFiles(None, RETROARCH_CUSTOM_CFG_FILE)
        retroconf, retroover = builder.createRetroarchConfiguration()
        self.assertEquals(retroconf['input_joypad_driver'], 'sdl2')

    def test_standard_inputdriver(self):
        builder = libretroConfigurations.LibretroConfiguration(self.snes, self.controllers, "/rom.rom", False, False, keyValueSettings("", False))
        builder.overrideLibretroConfigurationFiles(None, RETROARCH_CUSTOM_CFG_FILE)
        retroconf, retroover = builder.createRetroarchConfiguration()
        self.assertEquals(retroconf['input_joypad_driver'], 'udev')

    def test_inputdriver_none_specified(self):
        builder = libretroConfigurations.LibretroConfiguration(self.snes, self.sdl2controllers, "/rom.rom", False, False, keyValueSettings("", False))
        builder.overrideLibretroConfigurationFiles(None, RETROARCH_CUSTOM_CFG_FILE)
        retroconf, retroover = builder.createRetroarchConfiguration()
        self.assertEquals(retroconf['input_joypad_driver'], 'sdl2')

    def test_inputdriver_auto(self):
        self.snes.config['inputdriver'] = 'auto'
        builder = libretroConfigurations.LibretroConfiguration(self.snes, self.sdl2controllers, "/rom.rom", False, False, keyValueSettings("", False))
        builder.overrideLibretroConfigurationFiles(None, RETROARCH_CUSTOM_CFG_FILE)
        retroconf, retroover = builder.createRetroarchConfiguration()
        self.assertEquals(retroconf['input_joypad_driver'], 'sdl2')

    def test_remove_hotkeys_on_configure_with_es_menu_none(self):
        # Injecting test es_input
        controllersConfig.esInputs = os.path.abspath(os.path.join(os.path.dirname(__file__), "tmp/es_input.cfg"))  # TODO: WORKAROUND TO INITIALIZATION ORDER FAILURE! PLEASE REWORK!
        controllers = controllersConfig.Controller.loadControllerConfig(0, PS3UUID, "PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", "", "6", "1", "20",
                                                                        -1, 0, "p2controller","","0", "0", "0",
                                                                        -1, 0, "p3controller","","0", "0", "0",
                                                                        -1, 0, "p4controller","","0", "0", "0",
                                                                        -1, 0, "p5controller","","0", "0", "0",
                                                                        -1, 0, "p6controller","","0", "0", "0",
                                                                        -1, 0, "p7controller","","0", "0", "0",
                                                                        -1, 0, "p8controller","","0", "0", "0",
                                                                        -1, 0, "p9controller","","0", "0", "0",
                                                                        -1, 0, "p10controller","","0", "0", "0")

        command = libretroGen.generate(self.snes, rom, controllers, False, False, keyValueSettings("", False))
        self.assertEquals(libretroConfigurations.libretroSettings.load('input_menu_toggle_btn'), '14')
        self.snes2.config['specials'] = "none"
        command = libretroGen.generate(self.snes2, rom, controllers, False, False, keyValueSettings("", False))
        self.assertEquals(libretroConfigurations.libretroSettings.load('input_menu_toggle_btn'), None)


if __name__ == '__main__':
    unittest.main()
