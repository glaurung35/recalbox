#!/usr/bin/env python
import pytest
from configgen.Emulator import Emulator
import configgen.generators.devilutionx.devilutionxGenerator as devilutionxGenerator
from configgen.generators.devilutionx.devilutionxGenerator import DevilutionxGenerator
from configgen.settings.keyValueSettings import keyValueSettings
import configgen.controllers.controller as controllersConfig
from tests.generators.FakeArguments import Arguments

@pytest.fixture
def emulator():
    return DevilutionxGenerator()

@pytest.fixture
def system():
    return Emulator(name='devilutionx', videoMode='640x480', ratio='auto', emulator='devilutionx', core='devilutionx')

@pytest.mark.usefixtures("controller_configuration")
def test_simple_generate_devilutionx(emulator, system, controller_configuration):
    command = emulator.generate(system, controller_configuration, keyValueSettings("", False), Arguments('path/to/test.mpq'))
    assert command.videomode == '640x480'
    assert command.array == ['/usr/bin/devilutionx', '--data-dir', 'path/to', '--save-dir', 'path/to', '--config-dir', 'path/to']
