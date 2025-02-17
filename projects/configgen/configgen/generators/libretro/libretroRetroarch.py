from typing import Dict, List

import configgen.recalboxFiles as recalboxFiles
from configgen.Emulator import Emulator
from configgen.controllers.controller import ControllerPerPlayer
from configgen.settings.keyValueSettings import keyValueSettings


class LibretroRetroarch:

    # Warning: The values in the array must be exactly at the same index than
    # https://github.com/libretro/RetroArch/blob/master/retroarch.c#L3042
    RATIO_INDEXES: List[str] = \
    [
        "4/3",
        "16/9",
        "16/10",
        "16/15",
        "21/9",
        "1/1",
        "2/1",
        "3/2",
        "3/4",
        "4/1",
        "9/16",
        "5/4",
        "6/5",
        "7/9",
        "8/3",
        "8/7",
        "19/12",
        "19/14",
        "30/17",
        "32/9",
        "config", #20
        "squarepixel",
        "coreprovided",
        "custom",
        "fullscreen"
    ]

    # Define systems not compatible with rewind option
    SYSTEM_WITH_NO_REWIND: List[str] = \
    (
        'virtualboy',
        'sega32x',
        'segacd',
        'psx',
        'fba_libretro',
        'vectrex',
        'zxspectrum',
        'mame',
        'n64',
        '3do',
        'jaguar',
        'intellivision',
        'channelf',
        'neogeocd',
        'dreamcast',
        'atomiswave',
        'naomi',
        'ngpc',
        'pcfx',
        'cdi',
    )

    # Define the libretro device type corresponding to the libretro cores, when needed.
    CORE_TO_PLAYER1_DEVICE: Dict[str, int] = \
    {
        'cap32': 513,
        '81': 257,
        'fuse': 513,
        'atari800': 513
    }
    CORE_TO_PLAYER2_DEVICE: Dict[str, int] = \
    {
        'fuse': 513,
        'atari800': 513
    }

    # Netplay modes
    NETPLAY_MODES = ('host', 'client')

    # Define systems compatible with retroachievements
    SYSTEM_COMPATIBLE_RETROACHIEVEMENTS: List[str] = \
    (
        'snes',
        'nes',
        'gba',
        'gb',
        'gbc',
        'megadrive',
        'pcengine',
        'atari2600',
        'fba_libretro',
        'neogeo',
        'virtualboy',
        'ngp',
        'ngpc',
        'lynx',
        'mastersystem',
        'gamegear',
        'n64',
        'satellaview',
        'sufami',
        'neogeocd',
        'colecovision',
        'atari7800'
    )

    # AI Service language indexes - Keep in sync with
    # https://github.com/libretro/RetroArch/blob/master/translation_defines.h
    aiLanguages: List[str] =\
    [
        "AUTO",
        "EN",
        "ES",
        "FR",
        "IT",
        "DE",
        "JP",
        "NL",
        "CS",
        "DA",
        "SV",
        "HR",
        "KO",
        "ZH_CN",
        "ZH_TW",
        "CA",
        "BG",
        "BN",
        "EU",
        "AZ",
        "AR",
        "SQ",
        "AF",
        "EO",
        "ET",
        "TL",
        "FI",
        "GL",
        "KA",
        "EL",
        "GU",
        "HT",
        "IW",
        "HI",
        "HU",
        "IS",
        "ID",
        "GA",
        "KN",
        "LA",
        "LV",
        "LT",
        "MK",
        "MS",
        "MT",
        "NO",
        "FA",
        "PL",
        "PT",
        "RO",
        "RU",
        "SR",
        "SK",
        "SL",
        "SW",
        "TA",
        "TE",
        "TH",
        "TR",
        "UK",
        "UR",
        "VI",
        "CY",
        "YI"
    ]

    def languageIndex(self, language: str) -> str:
        try:
            return str(self.aiLanguages.index(language))
        except ValueError:
            return "0"

    def __init__(self, system: Emulator, settings: keyValueSettings, controllers: ControllerPerPlayer, demo: bool, recalboxOptions: keyValueSettings):
        self.recalboxOptions: keyValueSettings = recalboxOptions
        self.system: Emulator = system
        self.settings: keyValueSettings = settings
        self.controllers: ControllerPerPlayer = controllers
        self.demo: bool = demo

    # Is nVidia driver on?
    @staticmethod
    def hasnVidiaDriver() -> bool:
        import platform
        arch: str = platform.machine()
        if arch == "x86_64":
            import os
            if os.path.exists("/etc/modprobe.d/blacklist.conf"):
               for line in open("/etc/modprobe.d/blacklist.conf"):
                   if "blacklist nouveau" in line:
                       return True
        return False

    # Fill retroarch configuration
    def fillRetroarchConfiguration(self):
        settings: keyValueSettings = self.settings

        # Screen resolution
        from configgen.utils.resolutions import ResolutionParser
        resolution = ResolutionParser(self.system.VideoMode)
        settings.setBool("video_fullscreen", True)
        if resolution.isSet and resolution.selfProcess:
            settings.setInt("video_fullscreen_x", resolution.width) \
                    .setInt("video_fullscreen_y", resolution.height)
        else:
            settings.removeOption("video_fullscreen_x") \
                    .removeOption("video_fullscreen_y")

        # AI System
        aiFrom: str = self.system.TranslateFrom.upper()
        aiTo: str = self.system.TranslateTo.upper()
        if len(aiTo) > 2:
            if not aiTo.startswith("ZH") and not aiTo.startswith("AUTO"):  # Keep chinese region
                aiTo = aiTo[:2]
        aiKey: str = self.system.TranslateAPIKey
        aiUrl: str = self.system.TranslateURL
        settings.setBool("ai_service_enable", self.system.Translate and aiKey != "") \
                .setDefaultInt("ai_service_mode", 0) \
                .setString("ai_service_source_lang", self.languageIndex(aiFrom)) \
                .setString("ai_service_target_lang", self.languageIndex(aiTo))
        if len(aiKey) == 0: aiKey = "RECALBOX"
        if len(aiUrl) == 0: aiUrl = "https://ztranslate.net/service?api_key={}".format(aiKey)
        settings.setString("ai_service_url", aiUrl)

        # Threaded video
        settings.setBool("video_threaded", False)

        # Enable RetroArch option "quit_press_twice"
        settings.setBool("quit_press_twice", self.system.QuitTwice)

        # Control new RA 1.7.7 key: do not allow upscaling higher than x4
        rguiUpscaling: int = settings.getInt("rgui_internal_upscale_level", 1)
        if rguiUpscaling > 4:
            settings.setInt("rgui_internal_upscale_level", 4)
        # Allow extended ASCII
        settings.setBool("rgui_extended_ascii", True)

        # Smoothing?
        settings.setBool("video_smooth", self.system.Smooth)

        # Shaders?
        import os
        settings.setBool("video_shader_enable", self.system.HasShaderFile) \
                .setBool("video_threaded", self.system.HasShaderFile) \
                .setString("video_shader_dir", os.path.dirname(self.system.ShaderFile) if self.system.HasShaderFile else recalboxFiles.shadersRoot)
        if self.system.HasShaderFile:
            settings.setString("video_shader", os.path.basename(self.system.ShaderFile))

        # Screen ratio
        if self.system.Ratio in self.RATIO_INDEXES:
            settings.setInt("aspect_ratio_index", self.RATIO_INDEXES.index(self.system.Ratio)) \
                    .setBool("video_aspect_ratio_auto", False)
        elif self.system.Ratio == "none":
            # Do not fix aspect ratio. Let the user play with RA
            settings.setBool("video_aspect_ratio_auto", False)
        else:
            # default: auto
            settings.setBool("video_aspect_ratio_auto", True) \
                    .removeOption("aspect_ratio_index")

        # Rewind enabled?
        hasRewind: bool = self.system.Rewind and self.system.Name not in self.SYSTEM_WITH_NO_REWIND
        settings.setBool("rewind_enable", hasRewind)

        # Auto-save (not in demo mode! not in netplay mode!)
        hasAutoSave: bool = self.system.AutoSave and not self.demo and not self.system.Netplay
        settings.setBool("savestate_auto_save", hasAutoSave) \
                .setBool("savestate_auto_load", hasAutoSave)

        # Save thumbnail
        settings.setBool("savestate_thumbnail_enable", True)

        # Save folders
        settings.setString("savestate_directory", recalboxFiles.savesDir + self.system.Name) \
                .setString("savefile_directory", recalboxFiles.savesDir + self.system.Name)

        # Extend Controller configurations
        for i in range(0, 11):
            settings.setInt("input_libretro_device_p{}".format(i), 1)
        core = self.system.Core
        # Generic
        if core in self.CORE_TO_PLAYER1_DEVICE:
          settings.setInt("input_libretro_device_p0", self.CORE_TO_PLAYER1_DEVICE[core])
        if core in self.CORE_TO_PLAYER2_DEVICE:
          settings.setInt("input_libretro_device_p1", self.CORE_TO_PLAYER2_DEVICE[core])
        # Sens9x2010 case
        if len(self.controllers) > 2 and self.system.Core == 'snes9x2010':
            settings.setInt("input_libretro_device_p1", 257)
        # Fuse case - Force keyboard as P3
        if self.system.Core == 'fuse':
            settings.setInt("input_libretro_device_p2", 259)
        # 81 case - Force keyboard as P2
        if self.system.Core == '81':
            settings.setInt("input_libretro_device_p1", 259)

        # Retroachievements (not in demo mode!)
        hasRetroAch = self.system.Retroachievements and not self.demo
        settings.setBool("cheevos_enable", hasRetroAch) \
                .setBool("cheevos_hardcore_mode_enable", False)
        if hasRetroAch and self.system.Name in self.SYSTEM_COMPATIBLE_RETROACHIEVEMENTS:
            settings.setString("cheevos_username", self.system.RetroachievementsUsername) \
                    .setString("cheevos_password", self.system.RetroachievementsPassword) \
                    .setBool("cheevos_hardcore_mode_enable", self.system.RetroachievementsHardcore)

        # Integer scale
        settings.setBool("video_scale_integer", self.system.IntegerScale)

        # Display FPS
        settings.setBool("fps_show", self.system.ShowFPS) \
                .setInt("fps_update_interval", 256)

        # Netplay management
        settings.removeOption("netplay_password") \
                .removeOption("netplay_spectate_password") \
                .removeOption("netplay_start_as_spectator")
        if self.system.Netplay:
            settings.setBool("netplay_use_mitm_server", False)
            if self.system.NetplayHostMode:
                if self.system.HasNetplayMITM and self.system.NetplayMITM != "none":
                    settings.setBool("netplay_use_mitm_server", True) \
                            .setString("netplay_mitm_server", self.system.NetplayMITM)
            else:
                settings.setBool("netplay_start_as_spectator", self.system.NetplayViewerOnly)
            # Netplay passwords
            settings.setString("netplay_password", '"' + self.system.NetplayPlayerPassword + '"') \
                    .setString("netplay_spectate_password", '"' + self.system.NetplayViewerPassword + '"')

        # Experimental features
        settings.setBool("recalbox_experimental", self.system.RecalboxExperimental)
        if self.system.RecalboxExperimental:
            settings.setInt("video_max_swapchain_images", 2)
        else:
            settings.setInt("video_max_swapchain_images", 3)

        # Overlay reset
        settings.setString("input_overlay", "")

        # Ignore core informations (allow rewind, save states, netplay)
        settings.setBool("core_info_savestate_bypass", True)

        # Use bios in directories
        # Most of the case are by core name
        match self.system.Core:
            case 'a5200':
                settings.setString("system_directory", recalboxFiles.BIOS + "/atari5200")
            case 'bsnes':
                if self.system.Name == 'gb' or self.system.Name == 'gbc':
                    settings.setString("system_directory", recalboxFiles.BIOS + "/sgb")
                elif self.system.Name == 'satellaview':
                    settings.setString("system_directory", recalboxFiles.BIOS + "/satellaview")
                else:
                    settings.setString("system_directory", recalboxFiles.BIOS)
            case 'bsneshd':
                if self.system.Name == 'gb' or self.system.Name == 'gbc':
                    settings.setString("system_directory", recalboxFiles.BIOS + "/sgb")
                elif self.system.Name == 'satellaview':
                    settings.setString("system_directory", recalboxFiles.BIOS + "/satellaview")
                else:
                    settings.setString("system_directory", recalboxFiles.BIOS)
            case 'cdi2015':
                settings.setString("system_directory", recalboxFiles.BIOS + "/cdi")
            case 'emuscv':
                settings.setString("system_directory", recalboxFiles.BIOS + "/scv")
            case 'fmsx':
                settings.setString("system_directory", recalboxFiles.BIOS + "/msx")
            case 'freechaf':
                settings.setString("system_directory", recalboxFiles.BIOS + "/channelf")
            case 'freeintv':
                settings.setString("system_directory", recalboxFiles.BIOS + "/intellivision")
            case 'gambatte':
                settings.setString("system_directory", recalboxFiles.BIOS + "/" + self.system.Name)
            case 'gearsystem':
                settings.setString("system_directory", recalboxFiles.BIOS + "/" + self.system.Name)
            case 'genesisplusgx':
                settings.setString("system_directory", recalboxFiles.BIOS + "/" + self.system.Name)
            case 'genesisplusgxwide':
                settings.setString("system_directory", recalboxFiles.BIOS + "/" + self.system.Name)
            case 'gpsp':
                settings.setString("system_directory", recalboxFiles.BIOS + "/gba")
            case 'mednafen_pce_fast':
                settings.setString("system_directory", recalboxFiles.BIOS + "/" + self.system.Name)
            case 'mednafen_pcfx':
                settings.setString("system_directory", recalboxFiles.BIOS + "/pcfx")
            case 'mednafen_supergrafx':
                settings.setString("system_directory", recalboxFiles.BIOS + "/" + self.system.Name)
            case 'mesen_s':
                if self.system.SuperGameBoy:
                    settings.setString("system_directory", recalboxFiles.BIOS + "/sgb")
                else:
                    settings.setString("system_directory", recalboxFiles.BIOS + "/" + self.system.Name)
            case 'minivmac':
                settings.setString("system_directory", recalboxFiles.BIOS + "/macintosh")
            case 'mu':
                settings.setString("system_directory", recalboxFiles.BIOS + "/palm")
            case 'o2em':
                settings.setString("system_directory", recalboxFiles.BIOS + "/o2em")
            case 'opera':
                settings.setString("system_directory", recalboxFiles.BIOS + "/3do")
            case 'picodrive':
                settings.setString("system_directory", recalboxFiles.BIOS + "/" + self.system.Name)
            case 'pokemini':
                settings.setString("system_directory", recalboxFiles.BIOS + "/pokemini")
            case 'prosystem':
                settings.setString("system_directory", recalboxFiles.BIOS + "/atari7800")
            case 'sameboy':
                if self.system.SuperGameBoy:
                    settings.setString("system_directory", recalboxFiles.BIOS + "/sgb")
                else:
                    settings.setString("system_directory", recalboxFiles.BIOS + "/" + self.system.Name)
            case 'snes9x':
                if self.system.Name == 'satellaview':
                    settings.setString("system_directory", recalboxFiles.BIOS + "/satellaview")
                elif self.system.Name == 'sufami':
                    settings.setString("system_directory", recalboxFiles.BIOS + "/sufami")
                else:
                    settings.setString("system_directory", recalboxFiles.BIOS)
            case _:
                settings.setString("system_directory", recalboxFiles.BIOS);

        # We have a few to use by system name - when there are multiple cores with same bios
        if self.system.Name == 'fds':
            settings.setString("system_directory", recalboxFiles.BIOS + "/fds")
        elif self.system.Name == 'lynx':
            settings.setString("system_directory", recalboxFiles.BIOS + "/lynx")
        elif self.system.Name == 'nds':
            settings.setString("system_directory", recalboxFiles.BIOS + "/nds")
        elif self.system.Name == 'saturn':
            settings.setString("system_directory", recalboxFiles.BIOS + "/saturn")

        print("Bios directory: {}".format(settings.getString("system_directory", recalboxFiles.BIOS)));
