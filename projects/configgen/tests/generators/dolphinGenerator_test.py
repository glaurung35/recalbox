#!/usr/bin/env python
import pytest
import shutil
from configgen.Emulator import Emulator
import configgen.generators.dolphin.dolphinControllers as dolphinControllers
import configgen.generators.dolphin.dolphinGenerator as dolphinGenerator
from configgen.generators.dolphin.dolphinGenerator import DolphinGenerator
from configgen.settings.keyValueSettings import keyValueSettings

import configgen.controllers.controller as controllersConfig

from tests.generators.FakeArguments import Arguments


@pytest.fixture
def emulator():
    dolphinGenerator.recalboxFiles.dolphinIni = 'tests/tmp/dolphin.ini'
    dolphinGenerator.recalboxFiles.dolphinSYSCONF = 'tests/tmp/dolphin.SYSCONF'
    dolphinGenerator.recalboxFiles.dolphinGFX = 'tests/tmp/dolphin-GFX.ini'
    dolphinControllers.recalboxFiles.dolphinHKeys = 'tests/tmp/dolphin-Hotkeys.ini'
    dolphinControllers.recalboxFiles.dolphinConfig = 'tests/tmp'
    shutil.copyfile("tests/resources/dolphin.SYSCONF", "tests/tmp/dolphin.SYSCONF")
    return DolphinGenerator()


@pytest.fixture
def system_wii():
    return Emulator(name='wii', videoMode='1920x1080', ratio='auto', emulator='dolphin', core='dolphin')


@pytest.fixture
def system_gamecube():
    return Emulator(name='gamecube', videoMode='1920x1080', ratio='auto', emulator='dolphin', core='dolphin')


@pytest.mark.usefixtures("controller_configuration")
def test_simple_generate_wii(emulator, system_wii, mocker, controller_configuration):
    mocker.patch("configgen.generators.dolphin.dolphinControllers.EvdevGetJoystickName", return_value="Sony PLAYSTATION(R)3 Controller")
    command = emulator.generate(system_wii, controller_configuration, keyValueSettings("", False), Arguments('path/to/test'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/dolphin-emu', '-e', 'path/to/test']


@pytest.mark.usefixtures("controller_configuration")
def test_simple_generate_gamecube(emulator, system_gamecube, mocker, controller_configuration):
    mocker.patch("configgen.generators.dolphin.dolphinControllers.EvdevGetJoystickName", return_value="Sony PLAYSTATION(R)3 Controller")
    command = emulator.generate(system_gamecube, controller_configuration, keyValueSettings("", False), Arguments('path/to/test'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/dolphin-emu', '-e', 'path/to/test']
