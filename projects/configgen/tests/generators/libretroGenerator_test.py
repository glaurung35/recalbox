#!/usr/bin/env python
import shutil
from pathlib import Path

import pytest
import configgen.generators.libretro.libretroConfigurations as libretroConfigurations
import configgen.generators.libretro.libretroGenerator as libretroGenerator
import configgen.generators.libretro.libretroLightGuns as libretroLightGuns
import configgen.generators.libretro.libretroControllers as libretroControllers
from configgen.Command import Command
from configgen.Emulator import Emulator, ExtraArguments
from configgen.crt.Mode import Mode
from configgen.generators.libretro.libretroGenerator import LibretroGenerator
from configgen.settings.keyValueSettings import keyValueSettings
import configgen.controllers.controller as controllersConfig
from tests.Givens import givenThoseFiles
from tests.generators.FakeArguments import Arguments
from unittest.mock import MagicMock


def fake_button_mapping():
    '''fake PS3 button mapping
    key is the event code and the value is the button posistion'''
    return {
        201: 0, 202: 1, 203: 2, 204: 3, 210: 4, 211: 5, 212: 6, 213: 7,
        214: 8, 215: 9, 221: 10, 222: 11, 223: 12, 224: 13, 231: 14,
        251: 15, 252: 16, 277: 17, 278: 18, 279: 19, 280: 20,
    }


@pytest.fixture
def emulator():
    libretroConfigurations.recalboxFiles.retroarchRoot = 'tests/tmp/ra'
    libretroConfigurations.recalboxFiles.retroarchCustom = 'tests/tmp/ra-custom.cfg'
    libretroConfigurations.recalboxFiles.retroarchCoreCustom = 'tests/tmp/ra-core-custom.cfg'
    libretroGenerator.recalboxFiles.BIOS = 'tests/tmp/bios/'
    libretroLightGuns.esLightGun = 'tests/resources/lightgun.cfg'
    libretroLightGuns.GAME_INFO_PATH = 'tests/resources/es_state.inf'

    map_controller_buttons_mock = MagicMock(return_value=fake_button_mapping())
    libretroControllers.LibretroControllers._MapControllerButtons = map_controller_buttons_mock

    return LibretroGenerator()


@pytest.fixture
def emulator_mayflash():
    libretroLightGuns.recalboxFiles.esLightGun = 'tests/tmp/lightgun.cfg'
    libretroConfigurations.recalboxFiles.retroarchRoot = 'tests/tmp/ra'
    libretroConfigurations.recalboxFiles.retroarchCustom = 'tests/tmp/ra-custom.cfg'
    libretroConfigurations.recalboxFiles.retroarchCoreCustom = 'tests/tmp/ra-core-custom.cfg'
    libretroGenerator.recalboxFiles.BIOS = 'tests/tmp/bios/'
    shutil.copyfile('tests/resources/lightgun.cfg', 'tests/tmp/lightgun.cfg')

    return LibretroGenerator()


@pytest.fixture
def system_nes():
    return Emulator(name='nes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fceumm')


@pytest.fixture
def system_gb_with_overlays():
    gb = Emulator(name='gb', videoMode='1920x1080', ratio='auto', emulator='libretro', core='gb')
    recalbox_conf = keyValueSettings("", True)
    recalbox_conf.setString("global.recalboxoverlays", "1")
    gb.configure(recalbox_conf, ExtraArguments("", "", "", "", "", "", "", "", "", "", "", ""))
    return gb


@pytest.fixture
def system_snes():
    return Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x')


@pytest.fixture
def system_px68k():
    return Emulator(name='x68000', videoMode='1920x1080', ratio='auto', emulator='libretro', core='px68k')


@pytest.fixture
def system_quasi88():
    return Emulator(name='pc88', videoMode='1920x1080', ratio='auto', emulator='libretro', core='quasi88')


@pytest.fixture
def controller_configuration():
    uuid = "060000004c0500006802000000010000"
    return controllersConfig.Controller.LoadUserControllerConfigurations(
        p1index=0, p1guid=uuid, p1name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p1devicepath="/nonexistent/device/event1", p1nbaxes=-1,
        p1nbhats=-1, p1nbbuttons=-1,
        p2index=1, p2guid=uuid, p2name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p2devicepath="/nonexistent/device/event2", p2nbaxes=-1,
        p2nbhats=-1, p2nbbuttons=-1,
        p3index=2, p3guid=uuid, p3name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p3devicepath="/nonexistent/device/event3", p3nbaxes=-1,
        p3nbhats=-1, p3nbbuttons=-1,
        p4index=3, p4guid=uuid, p4name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p4devicepath="/nonexistent/device/event4", p4nbaxes=-1,
        p4nbhats=-1, p4nbbuttons=-1,
        p5index=4, p5guid=uuid, p5name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p5devicepath="/nonexistent/device/event5", p5nbaxes=-1,
        p5nbhats=-1, p5nbbuttons=-1,
        p6index=5, p6guid=uuid, p6name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p6devicepath="/nonexistent/device/event6", p6nbaxes=-1,
        p6nbhats=-1, p6nbbuttons=-1,
        p7index=6, p7guid=uuid, p7name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p7devicepath="/nonexistent/device/event7", p7nbaxes=-1,
        p7nbhats=-1, p7nbbuttons=-1,
        p8index=7, p8guid=uuid, p8name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p8devicepath="/nonexistent/device/event8", p8nbaxes=-1,
        p8nbhats=-1, p8nbbuttons=-1,
        p9index=8, p9guid=uuid, p9name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p9devicepath="/nonexistent/device/event9", p9nbaxes=-1,
        p9nbhats=-1, p9nbbuttons=-1,
        p10index=9, p10guid=uuid, p10name="PLAYSTATION(R)3 Controller (00:48:E8:D1:63:25)", p10devicepath="/nonexistent/device/event10",
        p10nbaxes=-1, p10nbhats=-1, p80nbbuttons=-1
    )


def test_simple_generate_snes(emulator, system_snes, controller_configuration):
    command = emulator.generate(system_snes, controller_configuration, keyValueSettings("", False),
                                Arguments('path/to/test'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/retroarch', '-L', '/usr/lib/libretro/snes9x_libretro.so',
                             '--config', 'tests/tmp/ra-custom.cfg',
                             '--appendconfig', 'tests/tmp/ra-custom.cfg.overrides.cfg',
                             'path/to/test']


def test_simple_generate_nes_mayflash(emulator_mayflash, system_nes, controller_configuration, mocker):
    mocker.patch('os.path.exists', return_value=True)
    command = emulator_mayflash.generate(system_nes, controller_configuration, keyValueSettings("", False),
                                         Arguments('path/to/duckhunt'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/retroarch', '-L', '/usr/lib/libretro/fceumm_libretro.so',
                             '--config', 'tests/tmp/ra-custom.cfg',
                             '--appendconfig', 'tests/tmp/ra-custom.cfg.overrides.cfg',
                             'path/to/duckhunt']


def test_simple_generate_px68k_hdd(emulator, system_px68k, controller_configuration, mocker):
    mocker.patch('os.path.getsize', return_value=2097152)
    command = emulator.generate(system_px68k, controller_configuration, keyValueSettings("", False),
                                Arguments('path/to/test'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/retroarch', '-L', '/usr/lib/libretro/px68k_libretro.so',
                             '--config', 'tests/tmp/ra-custom.cfg',
                             '--appendconfig', 'tests/tmp/ra-custom.cfg.overrides.cfg',
                             'path/to/test']


def test_simple_generate_px68k_floppy(emulator, system_px68k, controller_configuration, mocker):
    mocker.patch('os.path.getsize', return_value=720000)
    command = emulator.generate(system_px68k, controller_configuration, keyValueSettings("", False),
                                Arguments('path/to/test'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/retroarch', '-L', '/usr/lib/libretro/px68k_libretro.so',
                             '--config', 'tests/tmp/ra-custom.cfg',
                             '--appendconfig', 'tests/tmp/ra-custom.cfg.overrides.cfg',
                             'path/to/test']


def test_simple_generate_quasi88(emulator, system_quasi88, controller_configuration):
    command = emulator.generate(system_quasi88, controller_configuration, keyValueSettings("", False),
                                Arguments('path/to/test'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/retroarch', '-L', '/usr/lib/libretro/quasi88_libretro.so',
                             '--config', 'tests/tmp/ra-custom.cfg',
                             '--appendconfig', 'tests/tmp/ra-custom.cfg.overrides.cfg',
                             'path/to/test']


def test_simple_generate_quasi88_multi_disk_1(emulator, system_quasi88, controller_configuration, mocker):
    mocker.patch("glob.glob", return_value=['path/to/test(Disk 1 of 5).zip',
                                            'path/to/test(Disk 2 of 5).zip',
                                            'path/to/test(Disk 3 of 5).zip',
                                            'path/to/test(Disk 4 of 5).zip',
                                            'path/to/test(Disk 5 of 5).zip'])
    command = emulator.generate(system_quasi88, controller_configuration, keyValueSettings("", False),
                                Arguments('path/to/test(Disk 1 of 5).zip'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/retroarch', '-L', '/usr/lib/libretro/quasi88_libretro.so',
                             '--config', 'tests/tmp/ra-custom.cfg',
                             '--appendconfig', 'tests/tmp/ra-custom.cfg.overrides.cfg',
                             '--subsystem', 'pc88_5_disk',
                             'path/to/test(Disk 1 of 5).zip',
                             'path/to/test(Disk 2 of 5).zip',
                             'path/to/test(Disk 3 of 5).zip',
                             'path/to/test(Disk 4 of 5).zip',
                             'path/to/test(Disk 5 of 5).zip']


def test_simple_generate_quasi88_multi_disk_2(emulator, system_quasi88, controller_configuration, mocker):
    mocker.patch("glob.glob", return_value=['path/to/test(Disk 1 of 5)(Disk A).zip',
                                            'path/to/test(Disk 2 of 5)(Disk B).zip',
                                            'path/to/test(Disk 3 of 5)(Disk C).zip',
                                            'path/to/test(Disk 4 of 5)(Disk D).zip',
                                            'path/to/test(Disk 5 of 5)(Disk E).zip'])
    command = emulator.generate(system_quasi88, controller_configuration, keyValueSettings("", False),
                                Arguments('path/to/test(Disk 1 of 5)(Disk A).zip'))
    assert command.videomode == '1920x1080'
    assert command.array == ['/usr/bin/retroarch', '-L', '/usr/lib/libretro/quasi88_libretro.so',
                             '--config', 'tests/tmp/ra-custom.cfg',
                             '--appendconfig', 'tests/tmp/ra-custom.cfg.overrides.cfg',
                             '--subsystem', 'pc88_5_disk',
                             'path/to/test(Disk 1 of 5)(Disk A).zip',
                             'path/to/test(Disk 2 of 5)(Disk B).zip',
                             'path/to/test(Disk 3 of 5)(Disk C).zip',
                             'path/to/test(Disk 4 of 5)(Disk D).zip',
                             'path/to/test(Disk 5 of 5)(Disk E).zip']


def test_crt_enabled_create_mode_configuration(mocker, emulator, system_snes, controller_configuration):
    recalbox_conf = keyValueSettings("", True)
    givenThoseFiles(mocker, {
        "/recalbox/share/system/configs/crt/systems.txt": "snes,snes9x,pal,15kHz,progressive,snes:288@50p,1740,224\nsnes,snes9x,ntsc,15kHz,progressive,snes:240@60p,0,0",
        "/recalbox/share/system/configs/crt/modes.txt": "snes:240@60p,1920 1 78 192 210 240 1 3 3 16 0 0 0 60 0 37730000 1,60.1\nsnes:288@50p,1920 1 78 192 210 288 1 3 3 16 0 0 0 50 0 37730000 1,50.1"})

    system_snes.configure(recalbox_conf,
                          ExtraArguments("", "", "", "", "", "", "", "", "auto", "progressive", "15kHz", "recalboxrgbdual",
                                          "auto", False,
                                         crt_horizontaloffset_p1920x240=-2, crt_verticaloffset_p1920x240=-2, crt_viewportwidth_p1920x240=-10,
                                         crt_horizontaloffset_p1920x288=1, crt_verticaloffset_p1920x288=-2))

    emulator.generate(system_snes, controller_configuration, recalbox_conf, Arguments('path/to/rom'))
    generated_config = Path(libretroConfigurations.recalboxFiles.retroarchCustom).read_text()
    assert 'crt_switch_timings_ntsc = "1920 1 90 192 198 240 1 5 3 14 0 0 0 60 0 37730000 1"' in generated_config
    assert 'crt_switch_timings_pal = "1920 1 72 192 216 288 1 5 3 14 0 0 0 50 0 37730000 1"' in generated_config
    assert 'custom_viewport_width_ntsc = 1780' in generated_config
    assert 'custom_viewport_width_pal = 1740' in generated_config
    assert 'video_refresh_rate_ntsc = "60.1"' in generated_config
    assert 'video_refresh_rate_pal = "50.1"' in generated_config


def test_crt_enabled_create_overlay_configuration(mocker, emulator, system_gb_with_overlays, controller_configuration):
    recalbox_conf = keyValueSettings("", True)
    recalbox_conf.setString("global.recalboxoverlays", "1")

    system_gb_with_overlays.configure(recalbox_conf,
                                      ExtraArguments("", "", "", "", "", "", "", "", "auto", "progressive", "15kHz",
                                                     "recalboxrgbdual"))

    givenThoseFiles(mocker, {
        "/recalbox/share/system/configs/crt/systems.txt": "gb,gb,pal,15kHz,progressive,gb:pal:240@50p,1740,224\ngb,gb,ntsc,15kHz,progressive,gb:ntsc:224@60p,0,0",
        "/recalbox/share/system/configs/crt/modes.txt": "gb:pal:240@50p,1920 1 78 192 210 240 1 3 3 16 0 0 0 50 0 37730000 1,501\ngb:ntsc:224@60p,1920 1 78 192 210 224 1 3 3 16 0 0 0 60 0 37730000 1,60.1"})

    # Easy way to fake overlay file on file system
    mocker.patch('os.path.isfile', return_value=True)

    commandLine: Command = emulator.generate(system_gb_with_overlays, controller_configuration, recalbox_conf,
                                             Arguments('path/to/rom'))
    assert "/recalbox/share_init/240poverlays/gb/gb.cfg" in commandLine.array[6]


def test_sgb_enabled_configure_core_for_sgb(mocker, emulator, controller_configuration):
    recalbox_conf = keyValueSettings("", True)

    gb = Emulator(name='gb', videoMode='1920x1080', ratio='auto', emulator='libretro', core='gb')
    gb.configure(recalbox_conf, ExtraArguments("", "", "", "", "", "", "", "", "", "", "", "",sgb=True))

    emulator.generate(gb, controller_configuration, recalbox_conf,
                                             Arguments('path/to/rom.zip'))

    retroarchConf = Path(libretroConfigurations.recalboxFiles.retroarchCustom).read_text()
    coreConf = Path(libretroConfigurations.recalboxFiles.retroarchCoreCustom).read_text()
    assert 'aspect_ratio_index = "22"' in retroarchConf
    assert 'mgba_gb_model = "Super Game Boy"' in coreConf
    assert 'mgba_sgb_borders = "ON"' in coreConf
    assert 'mesen-s_gbmodel = "Super Game Boy"' in coreConf

def test_sgb_disabled_configure_core_for_sgb(mocker, emulator, controller_configuration):
    recalbox_conf = keyValueSettings("", True)
    recalbox_conf.setString("global.ratio", "auto")
    gb = Emulator(name='gb', videoMode='1920x1080', ratio='auto', emulator='libretro', core='gb')
    gb.configure(recalbox_conf, ExtraArguments("", "", "", "", "", "", "", "", "", "", "", "", sgb=False))

    emulator.generate(gb, controller_configuration, recalbox_conf,
                                             Arguments('path/to/rom.zip'))

    retroarchConf = Path(libretroConfigurations.recalboxFiles.retroarchCustom).read_text()
    coreConf = Path(libretroConfigurations.recalboxFiles.retroarchCoreCustom).read_text()
    assert 'aspect_ratio_index' not in retroarchConf
    assert 'mgba_gb_model = "Autodetect"' in coreConf
    assert 'mgba_sgb_borders = "OFF"' in coreConf
    assert 'mesen-s_gbmodel = "Auto"' in coreConf
