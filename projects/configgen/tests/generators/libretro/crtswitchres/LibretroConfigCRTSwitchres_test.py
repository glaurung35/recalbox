from unittest.mock import patch, PropertyMock

import pytest
from configgen.Emulator import Emulator, ExtraArguments
from configgen.crtswitchres.CRTTypes import RetroarchScreenTypeIndex
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



def test_given_15khz_on_dual_should_enable_retroarch_switchres():
    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="kHz15", crtadaptor="recalboxrgbdual")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz15.value

def test_given_15khz_on_jamma_should_enable_retroarch_switchres():
    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="kHz15", crtadaptor="recalboxrgbjamma")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz15Jamma.value

def test_given_dynamic_superrez_should_enable_superrez():
    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="kHz15", crtadaptor="recalboxrgbdual", crtsuperrez="1")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution_super"] == "1"

def test_given_native_should_enable_native():
    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="kHz15", crtadaptor="recalboxrgbdual", crtsuperrez="0")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution_super"] == "0"

def test_given_rpi5_should_disable_interlaced(mocker):
    mocker.patch('configgen.utils.architecture.Architecture.isPi5', return_value=True, new_callable=mocker.PropertyMock)

    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="kHz15", crtadaptor="recalboxrgbdual", crtsuperrez="0")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz15.value
    assert config_lines["crt_switch_resolution_super"] == "0"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 1

def test_given_rpi4_should_not_disable_interlaced(mocker):
    mocker.patch('configgen.utils.architecture.Architecture.isPi5', return_value=False, new_callable=mocker.PropertyMock)

    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="kHz15", crtadaptor="recalboxrgbdual", crtsuperrez="0")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz15.value
    assert config_lines["crt_switch_resolution_super"] == "0"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 0

def test_given_tate_game_on_rotated_screen_should_rotate(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        rotation= 3, verticalgame=True)
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["video_rotation"] == 3

def test_given_tate_game_on_yoko_screen_should_not_rotate(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        rotation=0, verticalgame=True)
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["video_rotation"] == 0

def test_given_yoko_game_on_tate_screen_should_rotate(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        rotation=3, verticalgame=False)
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["video_rotation"] == 3


def test_given_kHz15_screen_and_auto_resolution_then_configure_switchres(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHz15", crtresolutiontype="auto", crtsuperrez="1920")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz15.value
    assert config_lines["crt_switch_resolution_super"] == "1920"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 0

def test_given_kHz15_screen_and_240p_resolution_then_avoid_interlacd_modes(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHz15", crtresolutiontype="240p", crtsuperrez="1920")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz15.value
    assert config_lines["crt_switch_resolution_super"] == "1920"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 1

def test_given_kHz31_screen_and_auto_resolution_then_configure_switchres(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHz31", crtresolutiontype="auto", crtsuperrez="1920")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz31.value
    assert config_lines["crt_switch_resolution_super"] == "1920"
    assert config_lines["crt_switch_resolution_no_interlaced"] == 0

def test_given_kHz31_screen_and_doublefreq_resolution_then_configure_switchres(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHz31", crtresolutiontype="doublefreq")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz31at120.value


def test_given_kHz31_yoko_screen_and_yoko_game_480p_and_scanlines_then_configure_scanlines(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHz31", crtresolutiontype="480p", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz31.value
    assert config_lines["video_shader_enable"] == '"true"'
    assert config_lines["video_shader_dir"] == '"/recalbox/share/shaders/"'
    assert config_lines["video_shader"] == '/recalbox/share/shaders/rrgbd-scanlines-heavy.glslp'

def test_given_kHz31_tate_screen_and_yoko_game_480p_and_scanlines_then_skip_scanlines(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHz31", crtresolutiontype="480p", crtscanlines="heavy", verticalgame=False, rotation=3)
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz31.value
    assert config_lines["video_shader_enable"] == '"false"'

def test_given_kHz31_yoko_screen_and_tate_game_480p_and_scanlines_then_skip_scanlines(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHz31", crtresolutiontype="480p", crtscanlines="heavy", verticalgame=True, rotation=0)
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz31.value
    assert config_lines["video_shader_enable"] == '"false"'

def test_given_kHz15_and_scanlines_then_skip_scanlines(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHz15", crtresolutiontype="auto", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz15.value
    assert config_lines["video_shader_enable"] == '"false"'

def test_given_multi1531_and_auto_res_then_use_auto(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHzMulti1531", crtresolutiontype="auto", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHzMulti1531.value

def test_given_multi1531_and_480p_res_then_use_480p(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHzMulti1531", crtresolutiontype="480p", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz31.value

def test_given_multi1525_and_auto_res_then_use_auto(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHzMulti1525", crtresolutiontype="auto", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHzMulti1525.value

def test_given_multi1525_and_240p_res_then_use_240p(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHzMulti1525", crtresolutiontype="240p", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz15.value

def test_given_trisync_and_auto_res_then_use_auto(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHzTriFreq", crtresolutiontype="auto", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHzTriFreq.value

def test_given_trisync_and_240p_res_then_use_240p(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHzTriFreq", crtresolutiontype="240p", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz15.value

def test_given_trisync_and_480p_res_then_use_480p(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHzTriFreq", crtresolutiontype="480p", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz31.value

def test_given_trisync_and_force480p_res_then_use_scanlines(mocker):
    emulator = configureForCrt(
        Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo'),
        crtscreentype="kHzTriFreq", crtresolutiontype="480p", crtscanlines="heavy")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert config_lines["crt_switch_resolution"] == RetroarchScreenTypeIndex.kHz31.value
    assert config_lines["video_shader_enable"] == '"true"'
    assert config_lines["video_shader_dir"] == '"/recalbox/share/shaders/"'
    assert config_lines["video_shader"] == '/recalbox/share/shaders/rrgbd-scanlines-heavy.glslp'


def test_given_forced_video_standard_should_force_snes9x_core_region():
    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="kHz15",  crtvideostandard="ntsc",
        crtadaptor="recalboxrgbdual")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert core_config["snes9x_region"] == '"ntsc"'

    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="kHz15",  crtvideostandard="pal",
        crtadaptor="recalboxrgbdual")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert core_config["snes9x_region"] == '"pal"'

# Missing tests about each video standard, the code has been copied from old crt system, so it was tested

def test_given_jap_region_should_force_genesisplusgx_core_region():
    emulator = configureForCrt(
        Emulator(name='megadrive', videoMode='1920x1080', ratio='auto', emulator='libretro', core='genesisplusgx'),
        crtresolutiontype="progressive", crtscreentype="kHz15",  crtvideostandard="ntsc", crtregion="jp",
        crtadaptor="recalboxrgbdual")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "BareKnuckles.zip")

    assert core_config["genesis_plus_gx_region_detect"] == '"ntsc-j"'

def test_given_jap_region_should_force_picodrive_core_region():
    emulator = configureForCrt(
        Emulator(name='megadrive', videoMode='1920x1080', ratio='auto', emulator='libretro', core='picodrive'),
        crtresolutiontype="progressive", crtscreentype="kHz15",  crtvideostandard="ntsc", crtregion="jp",
        crtadaptor="recalboxrgbdual")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "BareKnuckles.zip")

    assert core_config["picodrive_region"] == '"Japan NTSC"'

    emulator = configureForCrt(
        Emulator(name='megadrive', videoMode='1920x1080', ratio='auto', emulator='libretro', core='picodrive'),
        crtresolutiontype="progressive", crtscreentype="kHz15",  crtvideostandard="ntsc", crtregion="us",
        crtadaptor="recalboxrgbdual")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "BareKnuckles.zip")

    assert core_config["picodrive_region"] == '"US"'

    emulator = configureForCrt(
        Emulator(name='megadrive', videoMode='1920x1080', ratio='auto', emulator='libretro', core='picodrive'),
        crtresolutiontype="progressive", crtscreentype="kHz15",  crtvideostandard="pal", crtregion="eu",
        crtadaptor="recalboxrgbdual")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "BareKnuckles.zip")

    assert core_config["picodrive_region"] == '"Europe"'

def test_given_rgbjamma_then_should_force_ntsc_core_region():
    emulator = configureForCrt(
        Emulator(name='snes', videoMode='1920x1080', ratio='auto', emulator='libretro', core='snes9x'),
        crtresolutiontype="progressive", crtscreentype="kHz15",  crtvideostandard="auto",
        crtadaptor="recalboxrgbjamma")
    config_lines, core_config = LibretroConfigCRTSwitchres().createConfigFor(emulator, "Mario.smc")

    assert core_config["snes9x_region"] == '"ntsc"'
