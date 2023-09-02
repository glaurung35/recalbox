#!/usr/bin/env python
import pytest
from configgen.Emulator import Emulator
from configgen.generators.linapple.linappleGenerator import LinappleGenerator
from configgen.settings.keyValueSettings import keyValueSettings

import configgen.controllers.controller as controllersConfig

from tests.generators.FakeArguments import Arguments


@pytest.fixture
def emulator():
    return LinappleGenerator('tests/resources/linapple', 'tests/tmp/linapple')


@pytest.fixture
def system():
    return Emulator(name='apple2', videoMode='1920x1080', ratio='auto', emulator='linapple', core='linapple')


@pytest.mark.usefixtures("controller_configuration")
def test_simple_generate_apple_ii(emulator, system, controller_configuration):
    command = emulator.generate(system, controller_configuration, keyValueSettings("", False), Arguments('path/to/test'))
    assert command.array == ['/usr/bin/linapple']
