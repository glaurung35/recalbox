from unittest.mock import patch, PropertyMock

import pytest
from configgen.Emulator import Emulator, ExtraArguments
from configgen.generators.libretro.crtswitchres.LibretroConfigCRTSwitchres import LibretroConfigCRTSwitchres
from configgen.settings.keyValueSettings import keyValueSettings


@pytest.fixture(autouse=True)
def before(mocker):
    mocker.patch('configgen.utils.architecture.Architecture.isPi5', return_value=False, new_callable=mocker.PropertyMock)

def configureForCrt(emulator: Emulator, crtvideostandard="auto", crtresolutiontype="auto", crtscreentype="15kHz",
                    crtadaptor="recalboxrgbdual", crtregion="auto", crtscanlines="", rotation=0, verticalgame=False,
                    crtsuperrez = "1"
                    ):
    emulator.configure(keyValueSettings(""),
                       ExtraArguments("", "", "", "", "", "", "", "", crtvideostandard, crtresolutiontype,
                                      crtscreentype,
                                      crtadaptor, crtregion,
                                      crtscanlines, rotation=rotation, verticalgame=verticalgame, crtsuperrez=crtsuperrez))
    return emulator



def test_given_15khz_should_enable_retroarch_switchres():
    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="15kHz", crtadaptor="recalboxrgbdual")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 1

def test_given_dynamic_superrez_should_enable_superrez():
    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="15kHz", crtadaptor="recalboxrgbdual", crtsuperrez="1")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution_super"] == "1"

def test_given_native_should_enable_native():
    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="15kHz", crtadaptor="recalboxrgbdual", crtsuperrez="0")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution_super"] == "0"

def test_given_rpi5_should_disable_interlaced(mocker):
    mocker.patch('configgen.utils.architecture.Architecture.isPi5', return_value=True, new_callable=mocker.PropertyMock)

    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="15kHz", crtadaptor="recalboxrgbdual", crtsuperrez="0")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 1
    assert config_lines["crt_switch_resolution_super"] == "0"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 1

def test_given_rpi4_should_not_disable_interlaced(mocker):
    mocker.patch('configgen.utils.architecture.Architecture.isPi5', return_value=False, new_callable=mocker.PropertyMock)

    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="15kHz", crtadaptor="recalboxrgbdual", crtsuperrez="0")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 1
    assert config_lines["crt_switch_resolution_super"] == "0"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 0

def test_given_tate_game_on_rotated_screen_should_rotate(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        rotation= 3, verticalgame=True)
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["video_rotation"] == 3

def test_given_tate_game_on_yoko_screen_should_not_rotate(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        rotation=0, verticalgame=True)
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["video_rotation"] == 0

def test_given_yoko_game_on_tate_screen_should_rotate(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        rotation=3, verticalgame=False)
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["video_rotation"] == 3


def test_given_15kHz_screen_and_auto_resolution_then_configure_switchres(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="15kHz", crtresolutiontype="auto", crtsuperrez="1920")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 1
    assert config_lines["crt_switch_resolution_super"] == "1920"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 0

def test_given_15kHz_screen_and_240p_resolution_then_avoid_interlacd_modes(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="15kHz", crtresolutiontype="240p", crtsuperrez="1920")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 1
    assert config_lines["crt_switch_resolution_super"] == "1920"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 1

def test_given_31kHz_screen_and_auto_resolution_then_configure_switchres(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="31kHz", crtresolutiontype="auto", crtsuperrez="1920")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 2
    assert config_lines["crt_switch_resolution_super"] == "1920"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 0

def test_given_31kHz_screen_and_doublefreq_resolution_then_configure_switchres(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="31kHz", crtresolutiontype="doublefreq")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 3


def test_given_31kHz_yoko_screen_and_yoko_game_480p_and_scanlines_then_configure_scanlines(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="31kHz", crtresolutiontype="480p", crtscanlines="heavy")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 2
    assert config_lines["video_shader_enable"] == '"true"'
    assert config_lines["video_shader_dir"] == '"/recalbox/share/shaders/"'
    assert config_lines["video_shader"] == '/recalbox/share/shaders/rrgbd-scanlines-heavy.glslp'

def test_given_31kHz_tate_screen_and_yoko_game_480p_and_scanlines_then_skip_scanlines(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="31kHz", crtresolutiontype="480p", crtscanlines="heavy", verticalgame=False, rotation=3)
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 2
    assert config_lines["video_shader_enable"] == '"false"'

def test_given_31kHz_yoko_screen_and_tate_game_480p_and_scanlines_then_skip_scanlines(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="31kHz", crtresolutiontype="480p", crtscanlines="heavy", verticalgame=True, rotation=0)
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 2
    assert config_lines["video_shader_enable"] == '"false"'

def test_given_15kHz_and_scanlines_then_skip_scanlines(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="15kHz", crtresolutiontype="auto", crtscanlines="heavy")
    config_lines = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == 1
    assert config_lines["video_shader_enable"] == '"false"'
