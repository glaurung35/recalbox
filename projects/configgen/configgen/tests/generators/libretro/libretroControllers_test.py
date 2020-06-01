#!/usr/bin/env python

import sys
import os.path
import unittest
import shutil

from configgen.settings.keyValueSettings import keyValueSettings

sys.path.append(
    os.path.abspath(os.path.join(os.path.dirname(__file__), "../../..")))

import configgen.generators.libretro.libretroControllers as libretroControllers
import configgen.controllersConfig as controllersConfig
from configgen.controllersConfig import Controller
from configgen.Emulator import Emulator
from configgen.settings.unixSettings import UnixSettings


RETROARCH_CONFIG = os.path.abspath(os.path.join(os.path.dirname(__file__), "tmp/retroarchcustom.cfg"))
RETROARCH_CORE_CONFIG = os.path.abspath(os.path.join(os.path.dirname(__file__), "tmp/retroarchcorecustom.cfg"))

shutil.copyfile(os.path.abspath(os.path.join(os.path.dirname(__file__), "../../resources/retroarchcustom.cfg.origin")),
                RETROARCH_CONFIG)

shutil.copyfile(os.path.abspath(os.path.join(os.path.dirname(__file__), "../../resources/retroarchcores.cfg")),
                RETROARCH_CORE_CONFIG)

shutil.copyfile(os.path.abspath(os.path.join(os.path.dirname(__file__), "../../resources/es_input.cfg.origin")),
                os.path.abspath(os.path.join(os.path.dirname(__file__), "tmp/es_input.cfg")))

# Injecting test recalbox.conf
libretroControllers.settingsRoot = os.path.abspath(os.path.join(os.path.dirname(__file__), "tmp"))
# Injecting test es_input
controllersConfig.esInputs = os.path.abspath(os.path.join(os.path.dirname(__file__), "tmp/es_input.cfg"))
# Injecting retroarch configuration
libretroControllers.coreSettings = UnixSettings(RETROARCH_CORE_CONFIG, separator=' ')

# Test objects


class TestLibretroController(unittest.TestCase):
    def setUp(self):
        self.PS3UUID = "060000004c0500006802000000010000"
        self.GPIOUUID = "15000000010000000100000000010000"

        self.snes = Emulator('snes', 'snes', 'libretro')
        self.system = Emulator("fakesystem", "fakeemulator", "fakecore")
        basicInputs1 = \
        {
            'a': controllersConfig.Input("a", "button", "10", "1", ""),
            'hotkey': controllersConfig.Input("hotkey", "button", "10", "1", ""),
            'start': controllersConfig.Input("start", "button", "11", "1", "")
        }
        self.basicController1 = Controller("contr1", "joypad", "GUID1", '1', "0", "Joypad1RealName", basicInputs1, nbaxes=6, nbhats=1, nbbuttons=10)
        self.basicControllers = { "1" : self.basicController1 }

    def test_generate_simple_controller(self):
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), self.basicControllers, False)
        config = controllerConfig.fillControllersConfiguration()
        self.assertEquals(config['input_player1_a_btn'], '10')

    def test_generate_ps3_controller_buttons(self):
        controllers = Controller.loadControllerConfig(0, self.PS3UUID, "PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)","", "6", "1", "20",
                                                      -1, 0, "p2controller","", "0", "0", "0",
                                                      -1, 0, "p3controller","", "0", "0", "0",
                                                      -1, 0, "p4controller","", "0", "0", "0",
                                                      -1, 0, "p5controller","", "0", "0", "0",
                                                      -1, 0, "p6controller","", "0", "0", "0",
                                                      -1, 0, "p7controller","", "0", "0", "0",
                                                      -1, 0, "p8controller","", "0", "0", "0",
                                                      -1, 0, "p9controller","", "0", "0", "0",
                                                      -1, 0, "p10controller","", "0", "0", "0")
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), controllers, False)
        config = controllerConfig.fillControllersConfiguration()
        self.assertEquals(config['input_player1_select_btn'], '0')
        self.assertEquals(config['input_player1_a_btn'], '13')
        self.assertEquals(config['input_player1_start_btn'], '3')
        self.assertEquals(config['input_player1_b_btn'], '14')
        self.assertEquals(config['input_player1_l2_btn'], '8')
        self.assertEquals(config['input_player1_r_btn'], '11')
        self.assertEquals(config['input_player1_y_btn'], '15')
        self.assertEquals(config['input_player1_x_btn'], '12')
        self.assertEquals(config['input_player1_l_btn'], '10')

    def test_generate_ps3_controller_joysticks(self):
        controllers = Controller.loadControllerConfig(0, self.PS3UUID, "PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)","", "6", "1", "20",
                                                            -1, 0, "p2controller","", "0", "0", "0",
                                                            -1, 0, "p3controller","", "0", "0", "0",
                                                            -1, 0, "p4controller","", "0", "0", "0",
                                                            -1, 0, "p5controller","", "0", "0", "0",
                                                            -1, 0, "p6controller","", "0", "0", "0",
                                                            -1, 0, "p7controller","", "0", "0", "0",
                                                            -1, 0, "p8controller","", "0", "0", "0",
                                                            -1, 0, "p9controller","", "0", "0", "0",
                                                            -1, 0, "p10controller","", "0", "0", "0")
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), controllers, False)
        config = controllerConfig.fillControllersConfiguration()
        self.assertEquals(config['input_player1_l_y_plus_axis'], '+1')
        self.assertEquals(config['input_player1_l_y_minus_axis'], '-1')
        self.assertEquals(config['input_player1_l_x_plus_axis'], '+0')
        self.assertEquals(config['input_player1_l_x_minus_axis'], '-0')


    def test_generate_joystick_as_directions(self):
        controllers = Controller.loadControllerConfig(0, self.GPIOUUID, "GPIO Controller 1","", "2", "0", "8",
                                                            -1, 0, "p2controller","", "0", "0", "0",
                                                            -1, 0, "p3controller","", "0", "0", "0",
                                                            -1, 0, "p4controller","", "0", "0", "0",
                                                            -1, 0, "p5controller","", "0", "0", "0",
                                                            -1, 0, "p6controller","", "0", "0", "0",
                                                            -1, 0, "p7controller","", "0", "0", "0",
                                                            -1, 0, "p8controller","", "0", "0", "0",
                                                            -1, 0, "p9controller","", "0", "0", "0",
                                                            -1, 0, "p10controller","", "0", "0", "0")
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), controllers, False)
        config = controllerConfig.fillControllersConfiguration()
        self.assertEquals(config['input_player1_up_axis'], '-1')
        self.assertEquals(config['input_player1_down_axis'], '+1')
        self.assertEquals(config['input_player1_right_axis'], '+0')
        self.assertEquals(config['input_player1_left_axis'], '-0')

    def test_generate_specials(self):
        controllers = Controller.loadControllerConfig(0, self.GPIOUUID, "GPIO Controller 1", "", "2", "0", "8",
                                                     -1, 0, "p2controller","", "0", "0", "0",
                                                     -1, 0, "p3controller","", "0", "0", "0",
                                                     -1, 0, "p4controller","", "0", "0", "0",
                                                     -1, 0, "p5controller","", "0", "0", "0",
                                                     -1, 0, "p6controller","", "0", "0", "0",
                                                     -1, 0, "p7controller","", "0", "0", "0",
                                                     -1, 0, "p8controller","", "0", "0", "0",
                                                     -1, 0, "p9controller","", "0", "0", "0",
                                                     -1, 0, "p10controller","", "0", "0", "0")
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), controllers, False)
        config = controllerConfig.fillControllersConfiguration()
        self.assertEquals(config['input_exit_emulator_btn'], '7')


    def test_write_ps3_controller_joysticks(self):
        controllers = Controller.loadControllerConfig(0, self.PS3UUID, "PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)","", "6", "1", "20",
                                                            -1, 0, "p2controller","", "0", "0", "0",
                                                            -1, 0, "p3controller","", "0", "0", "0",
                                                            -1, 0, "p4controller","", "0", "0", "0",
                                                            -1, 0, "p5controller","", "0", "0", "0",
                                                            -1, 0, "p6controller","", "0", "0", "0",
                                                            -1, 0, "p7controller","", "0", "0", "0",
                                                            -1, 0, "p8controller","", "0", "0", "0",
                                                            -1, 0, "p9controller","", "0", "0", "0",
                                                            -1, 0, "p10controller","", "0", "0", "0")
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), controllers, False)
        config = controllerConfig.fillControllersConfiguration()
        self.assertTrue(config["input_player1_l2_btn"], "8")
        self.assertTrue(config["input_player1_analog_dpad_mode"], "1")
        #with open(RETROARCH_CONFIG) as controllerFile:
        #    lines = []
        #    for line in controllerFile:
        #        lines.append(line)
        #    self.assertTrue('input_player1_l2_btn = 8\n' in lines)
        #self.assertTrue(libretroControllers.LibretroControllers.libretroSettings.load("input_player1_analog_dpad_mode"), "1")

    def test_write_only_joystick_controller(self):
        controllers = Controller.loadControllerConfig(0, self.GPIOUUID, "GPIO Controller 1","", "2", "0", "8",
                                                            -1, 0, "p2controller","", "0", "0", "0",
                                                            -1, 0, "p3controller","", "0", "0", "0",
                                                            -1, 0, "p4controller","", "0", "0", "0",
                                                            -1, 0, "p5controller","", "0", "0", "0",
                                                            -1, 0, "p6controller","", "0", "0", "0",
                                                            -1, 0, "p7controller","", "0", "0", "0",
                                                            -1, 0, "p8controller","", "0", "0", "0",
                                                            -1, 0, "p9controller","", "0", "0", "0",
                                                            -1, 0, "p10controller","", "0", "0", "0")
        controllerConfig = libretroControllers.LibretroControllers(self.snes, keyValueSettings(None, True), controllers, False)
        config = controllerConfig.fillControllersConfiguration()
        self.assertTrue(config["input_player1_analog_dpad_mode"], "0")

    def test_write_hotkey(self):
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), self.basicControllers, False)
        controllerConfig.setHotKey()
        config = controllerConfig.settings
        self.assertEqual(config['input_enable_hotkey_btn'], '10')

    def test_generate_ps3_controller_joysticks_right(self):
        controllers = Controller.loadControllerConfig(0, self.PS3UUID, "PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)","", "6", "1", "20",
                                                            -1, 0, "p2controller","", "0", "0", "0",
                                                            -1, 0, "p3controller","", "0", "0", "0",
                                                            -1, 0, "p4controller","", "0", "0", "0",
                                                            -1, 0, "p5controller","", "0", "0", "0",
                                                            -1, 0, "p6controller","", "0", "0", "0",
                                                            -1, 0, "p7controller","", "0", "0", "0",
                                                            -1, 0, "p8controller","", "0", "0", "0",
                                                            -1, 0, "p9controller","", "0", "0", "0",
                                                            -1, 0, "p10controller","", "0", "0", "0")
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), controllers, False)
        config = controllerConfig.fillControllersConfiguration()
        self.assertEquals(config['input_player1_r_y_plus_axis'], '+3')
        self.assertEquals(config['input_player1_r_y_minus_axis'], '-3')
        self.assertEquals(config['input_player1_r_x_plus_axis'], '+2')
        self.assertEquals(config['input_player1_r_x_minus_axis'], '-2')

    def test_generate_with_recalbox_conf_es_menu_off_doesnt_save_hotkeys(self):
        controllers = Controller.loadControllerConfig(0, self.PS3UUID, "PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)","", "6", "1", "20",
                                                            -1, 0, "p2controller","", "0", "0", "0",
                                                            -1, 0, "p3controller","", "0", "0", "0",
                                                            -1, 0, "p4controller","", "0", "0", "0",
                                                            -1, 0, "p5controller","", "0", "0", "0",
                                                            -1, 0, "p6controller","", "0", "0", "0",
                                                            -1, 0, "p7controller","", "0", "0", "0",
                                                            -1, 0, "p8controller","", "0", "0", "0",
                                                            -1, 0, "p9controller","", "0", "0", "0",
                                                            -1, 0, "p10controller","", "0", "0", "0")
        self.system.config["specials"] = "ipsumlorem"
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), controllers, False)
        config = controllerConfig.fillControllersConfiguration()
        del self.system.config["specials"]
        self.assertFalse(config.hasOption('input_load_state_btn'))
        self.assertFalse(config.hasOption('input_menu_toggle_btn'))
        self.assertFalse(config.hasOption('input_state_slot_increase_axis'))

    def test_generate_specialsKeys_off_save_exit_special(self):
        controllers = Controller.loadControllerConfig(0, self.PS3UUID, "PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)","", "6", "1", "20",
                                                            -1, 0, "p2controller","", "0", "0", "0",
                                                            -1, 0, "p3controller","", "0", "0", "0",
                                                            -1, 0, "p4controller","", "0", "0", "0",
                                                            -1, 0, "p5controller","", "0", "0", "0",
                                                            -1, 0, "p6controller","", "0", "0", "0",
                                                            -1, 0, "p7controller","", "0", "0", "0",
                                                            -1, 0, "p8controller","", "0", "0", "0",
                                                            -1, 0, "p9controller","", "0", "0", "0",
                                                            -1, 0, "p10controller","", "0", "0", "0",)
        self.system.config["specials"] = "ipsumlorem"
        controllerConfig = libretroControllers.LibretroControllers(self.system, keyValueSettings(None, True), controllers, False)
        config = controllerConfig.fillControllersConfiguration()
        del self.system.config["specials"]
        self.assertEquals(config['input_exit_emulator_btn'], '3')

class TestLibretroGeneratorGetValue(unittest.TestCase):
    def test_on_button(self):
        val = libretroControllers.LibretroControllers.getConfigValue(controllersConfig.Input("a", "button", "10", "1","0"))
        self.assertEquals("10", val)

    def test_on_axis(self):
        val = libretroControllers.LibretroControllers.getConfigValue(controllersConfig.Input("down", "axis", "10", "1","0"))
        self.assertEquals("+10", val)
        val = libretroControllers.LibretroControllers.getConfigValue(controllersConfig.Input("down", "axis", "10", "-1","0"))
        self.assertEquals("-10", val)

    def test_on_hat(self):
        val = libretroControllers.LibretroControllers.getConfigValue(controllersConfig.Input("down", "hat", "2", "1","0"))
        self.assertEquals("h2up", val)
        val = libretroControllers.LibretroControllers.getConfigValue(controllersConfig.Input("down", "hat", "3", "8","0"))
        self.assertEquals("h3left", val)

    def test_on_key(self):
        val = libretroControllers.LibretroControllers.getConfigValue(controllersConfig.Input("down", "key", "2", "1","0"))
        self.assertEquals("2", val)
        val = libretroControllers.LibretroControllers.getConfigValue(controllersConfig.Input("down", "key", "3", "8","0"))
        self.assertEquals("3", val)


class TestLibretroDualAnalogPSone(unittest.TestCase):
    def setUp(self):
        self.PS3UUID = "060000004c0500006802000000010000"
        basicInputs1 = \
        {
            'a': controllersConfig.Input("a", "button", "10", "1", ""),
            'hotkey': controllersConfig.Input("hotkey", "button", "10", "1", ""),
            'start': controllersConfig.Input("start", "button", "11", "1", "")
        }
        self.basicController1 = Controller("contr1", "joypad", "GUID1", '1', "0", "Joypad1RealName", basicInputs1, nbaxes=6, nbhats=1, nbbuttons=10)

    def test_enable_standard_mode_psx(self):
        val = libretroControllers.LibretroControllers.getAnalogCoreMode(self.basicController1)
        self.assertEquals("standard", val)

    def test_enable_analog_mode_psx(self):
        controllers = Controller.loadControllerConfig(0, self.PS3UUID, "PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)","", "6", "1", "20",
                                                            -1, 0, "p2controller","", "0", "0", "0",
                                                            -1, 0, "p3controller","", "0", "0", "0",
                                                            -1, 0, "p4controller","", "0", "0", "0",
                                                            -1, 0, "p5controller","", "0", "0", "0",
                                                            -1, 0, "p6controller","", "0", "0", "0",
                                                            -1, 0, "p7controller","", "0", "0", "0",
                                                            -1, 0, "p8controller","", "0", "0", "0",
                                                            -1, 0, "p9controller","", "0", "0", "0",
                                                            -1, 0, "p10controller","", "0", "0", "0")
        val = libretroControllers.LibretroControllers.getAnalogCoreMode(controllers['1'])
        self.assertEquals("analog", val)


class TestLibretroGeneratorInputDriverTest(unittest.TestCase):
    def test_udev_by_default(self):
        controllers = dict()
        controllers['1'] =  Controller("contr1", "joypad", "GUID1", '1',  "0", "Joypad1RealName", dict(), nbaxes=6, nbhats=1, nbbuttons=10)
        controllerConfig = libretroControllers.LibretroControllers(None, None, controllers, False)
        driver = controllerConfig.getInputDriver()
        self.assertEquals("udev", driver)


    def test_sdl2_for_nes30pro(self):
        controllers = dict()
        controllers['1'] =  Controller("contr1", "joypad", "030000003512000012ab000010010000", '1',  "0", "Bluetooth Wireless Controller ", dict(), nbaxes=6, nbhats=1, nbbuttons=10)
        controllerConfig = libretroControllers.LibretroControllers(None, None, controllers, False)
        driver = controllerConfig.getInputDriver()
        self.assertEquals("sdl2", driver)


    def test_sdl2_for_fc30pro(self):
        controllers = dict()
        controllers['1'] =  Controller("contr1", "joypad", "05000000103800000900000000010000", '1',  "0", "szmy-power Ltd. Joypad ", dict(), nbaxes=6, nbhats=1, nbbuttons=10)
        controllerConfig = libretroControllers.LibretroControllers(None, None, controllers, False)
        driver = controllerConfig.getInputDriver()
        self.assertEquals("sdl2", driver)


if __name__ == '__main__':
    unittest.main()
