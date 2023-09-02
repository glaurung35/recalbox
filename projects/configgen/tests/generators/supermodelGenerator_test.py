#!/usr/bin/env python
import pytest
from configgen.Emulator import Emulator
import configgen.generators.supermodel.supermodelGenerator as supermodelGenerator
import configgen.generators.supermodel.supermodelControllers as supermodelControllers
from configgen.generators.supermodel.supermodelGenerator import SupermodelGenerator
from configgen.settings.keyValueSettings import keyValueSettings

import configgen.controllers.controller as controllersConfig

from tests.generators.FakeArguments import Arguments


@pytest.fixture
def emulator():
    supermodelGenerator.recalboxFiles.supermodelConfigFile = 'tests/resources/ConfigModel3.ini'
    supermodelControllers.recalboxFiles.supermodelControlsIni = 'tests/tmp/supermodel.ini'
    return SupermodelGenerator()


@pytest.fixture
def system():
    return Emulator(name='supermodel', videoMode='1920x1080', ratio='auto', emulator='supermodel', core='supermodel')


@pytest.mark.usefixtures("controller_configuration")
def test_simple_generate_supermodel(emulator, system, mocker, controller_configuration, fake_process):
    fake_process.register_subprocess(
        ["tvservice -s"],
        stdout=['[{ "code":1, "width":1920, "height":1080 }]']
    )
    mocker.patch('platform.machine', return_value='x86_64')
    command = emulator.generate(system, controller_configuration, keyValueSettings("", False), Arguments('path/to/test'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/supermodel', 'path/to/test',
                             '-new-scsp',
                             '-fullscreen', '-res=1920,1080',
                             '-no-multi-texture', '-legacy3d',
                             '-crosshairs=1', '-gpu-multi-threaded',
                             '-ppc-frequency=100',
                             '-load-state=/recalbox/share/saves/model3/',
                             '-log-output=/recalbox/share/system/configs/model3/Supermodel.log',
                             '-log-level=info']
