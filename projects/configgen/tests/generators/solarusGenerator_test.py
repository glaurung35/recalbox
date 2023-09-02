#!/usr/bin/env python
import pytest
from configgen.Emulator import Emulator
import configgen.generators.solarus.solarusGenerator as solarusGenerator
from configgen.generators.solarus.solarusGenerator import SolarusGenerator
from configgen.settings.keyValueSettings import keyValueSettings

import configgen.controllers.controller as controllersConfig

from tests.generators.FakeArguments import Arguments


@pytest.fixture
def emulator():
    solarusGenerator.recalboxFiles.solarusConfig = 'tests/tmp/solarus.conf'
    return SolarusGenerator()


@pytest.fixture
def system():
    return Emulator(name='solarus', videoMode='1920x1080', ratio='auto', emulator='solarus', core='solarus')


@pytest.mark.usefixtures("controller_configuration")
def test_simple_generate_solarus(emulator, system, controller_configuration):
    command = emulator.generate(system, controller_configuration, keyValueSettings("", False), Arguments('path/to/test'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/solarus-run', '-fullscreen=yes',
                             '-cursor-visible=no', '-bilinear-filtering',
                             '-quit-combo=10+9', 'path/to/test']
