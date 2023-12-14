from configgen.Emulator import Emulator, ExtraArguments
from configgen.generators.libretro.libretroGenerator import LibretroGenerator
from configgen.settings.keyValueSettings import keyValueSettings


def configure(system: Emulator):
    opts = keyValueSettings("")
    system.configure(opts, ExtraArguments('1920x1080', "", "", "", "", "", "", "", "", "", "", ""))
    return system

def test_given_mmpork_and_gbneo_then_create_core_config(mocker):
    coreConfig = LibretroGenerator().createFBNeoBlitterConfig(configure(Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo')), "mmpork.zip")

    assert coreConfig["fbneo-cpu-speed-adjust"] == '"60%"'
    assert coreConfig["fbneo-dipswitch-mmpork-Blitter_Delay"] == '"53"'


def test_given_any_fbneo_game_and_gbneo_then_restore_cpu_speed(mocker):
    coreConfig = LibretroGenerator().createFBNeoBlitterConfig(configure(Emulator(name='fbneo', videoMode='1920x1080', ratio='auto', emulator='libretro', core='fbneo')), "any.zip")
    assert coreConfig["fbneo-cpu-speed-adjust"] == '"100%"'
