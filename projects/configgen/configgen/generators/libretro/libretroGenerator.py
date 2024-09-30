#!/usr/bin/env python
import os
from typing import List, Dict, Any

import configgen.recalboxFiles as recalboxFiles
from configgen.Command import Command
from configgen.Emulator import Emulator
from configgen.generators.Generator import Generator, ControllerPerPlayer
from configgen.generators.libretro.libretroRetroarch import LibretroRetroarch
from configgen.settings.keyValueSettings import keyValueSettings
from configgen.utils.Rotation import Rotation
import configgen.generators.libretro.libretroConfigurations as libretroConfigurations
from configgen.utils.Vulkan import Vulkan
from configgen.utils.architecture import Architecture


class LibretroGenerator(Generator):

    # Main entry of the module
    def config_upgrade(self, version: str) -> bool:
        import configgen.generators.libretro.libretroConfigurations as libretroConfigurations
        return libretroConfigurations.LibretroConfiguration.updateLibretroConfig(version)

    # Build command ligne arguments for Netplay
    @staticmethod
    def getNetplayArguments(system: Emulator) -> List[str]:
        # Netplay command holder
        result = []

        if system.Netplay:
            if system.NetplayHostMode:
                result.append("--host")
                if system.HasHash:
                    result.extend(["--hash", system.Hash])
            else:
                if len(system.NetplayIP) != 0:
                    result.extend(["--connect", system.NetplayIP])
                else:
                    raise ValueError("You must specify n IP in client mode")
            # port and nick are now directly in config file (see createNetplayConfig)

        return result
    @staticmethod
    def createNetplayConfig(system: Emulator) -> (Dict[str, Any], Dict[str, Any]):
        config = {}
        config["netplay_ip_port"] = system.NetplayPort
        config["netplay_nickname"] = '"{}"'.format(system.NetplayUsername)
        return config

    # Overlay management
    @staticmethod
    def processOverlays(system: Emulator, romName: str, configs: List[str], recalboxOptions: keyValueSettings):
        import os.path
        # tate mode do not support overlay
        # If we are on super game boy mode, we do not enable overlays
        # Widescreen means no overlays
        if system.Rotation or system.SuperGameBoy or system.WideScreenMode:
            return
        # If we are in crt mode, we only allow recalbox default 240p overlays
        if system.CRTEnabled:
            if system.RecalboxOverlays:
                crtOverlayFile = "{}/{}/{}.cfg".format(recalboxFiles.RECALBOX_240P_OVERLAYS, system.Name,
                                                       system.Name)
                if os.path.isfile(crtOverlayFile):
                    configs.append(crtOverlayFile)
        # Overlays are applied only when we are not in wide core
        else:
            # User overlays
            userOverlayApplied = False
            overlayFile = "{}/{}/.overlay.cfg".format(recalboxFiles.OVERLAYS, system.Name)
            if os.path.isfile(overlayFile):
                # System global configuration
                configs.append(overlayFile)
                userOverlayApplied = True
            else:
                overlayFile = "{}/.overlay.cfg".format(recalboxFiles.OVERLAYS)
                if os.path.isfile(overlayFile):
                    # All system global configuration
                    configs.append(overlayFile)
                    userOverlayApplied = True
            overlayFile = "{}/{}/{}.cfg".format(recalboxFiles.OVERLAYS, system.Name, romName)
            if os.path.isfile(overlayFile):
                # Rom file overlay
                configs.append(overlayFile)
                userOverlayApplied = True
            else:
                overlayFile = "{}/{}/{}.cfg".format(recalboxFiles.OVERLAYS, system.Name, system.Name)
                if os.path.isfile(overlayFile):
                    # System overlay
                    configs.append(overlayFile)
                    userOverlayApplied = True
            if not userOverlayApplied:
                # The recalbox overlays should be added only if
                # global.recalboxoverlays=1 or system.recalboxoverlays activated
                if system.RecalboxOverlays:
                    # ratio = we can activate when ratio is not 16/9 and 16/10
                    if system.Ratio not in ["16/9", "16/10"]:
                        # screen resolution that can support overlays are over 1.5 ratio (as it is float > 1.51)
                        from configgen.utils.resolutions import ResolutionParser
                        resolution = ResolutionParser(system.VideoMode)
                        if resolution.ratio() > 1.51:
                            defaultOverlayFile = "{}/{}/{}.cfg".format(recalboxFiles.RECALBOX_OVERLAYS, system.Name,
                                                                       system.Name)
                            if os.path.isfile(defaultOverlayFile):
                                configs.append(defaultOverlayFile)

    # Build appendable configurations files argument
    def getAppendConfigs(self, system: Emulator, rom: str, externalOverrides: str, recalboxOptions: keyValueSettings) -> \
            List[str]:
        # Extra configs
        configs = []
        import os.path
        romName = os.path.basename(rom)

        # Custom configs - per core - DEPRECATED
        customCfg = "{}/{}.cfg".format(recalboxFiles.retroarchRoot, system.Name)
        if os.path.isfile(customCfg):
            configs.append(customCfg)

        # Custom configs - per game - DEPRECATED
        customGameCfg = "{}/{}/{}.cfg".format(recalboxFiles.retroarchRoot, system.Name, romName)
        if os.path.isfile(customGameCfg):
            configs.append(customGameCfg)

        # Process overlays
        self.processOverlays(system, romName, configs, recalboxOptions)

        # In-place override takes priority over all
        if os.path.isfile(externalOverrides):
            configs.append(externalOverrides)

        if not configs:
            return []

        return ["--appendconfig", "|".join(configs)]

    # Create crt configuration
    @staticmethod
    def createCrtConfiguration(system: Emulator, rom: str, recalboxOptions: keyValueSettings,
                               retroarchConfig: keyValueSettings, coreConfig: keyValueSettings,
                               retroarchOverrides: keyValueSettings):
        # recalbox-crt-options.cfg options

        # Retroarch CRT configuration
        if system.CRTV2:
            from configgen.generators.libretro.crtswitchres.LibretroConfigCRTSwitchres import LibretroConfigCRTSwitchres
            retroOptions, coreOptions = LibretroConfigCRTSwitchres().createConfigFor(system, rom)
            for option in retroOptions.items():
                retroarchConfig.setString(option[0], option[1])
            for option in coreOptions.items():
                coreConfig.setString(option[0], option[1])
        else:
            from configgen.generators.libretro.crt.LibretroConfigCRT import LibretroConfigCRT
            from configgen.crt.CRTConfigParser import CRTConfigParser
            from configgen.crt.CRTModeOffsetter import CRTModeOffsetter
            libretro_crt_configurator = LibretroConfigCRT(CRTConfigParser(), CRTModeOffsetter(), system.CRTV2)
            for option in libretro_crt_configurator.createConfigFor(system, rom).items():
                retroarchConfig.setString(option[0], option[1])
            # Core configuration
            from configgen.generators.libretro.crt.LibretroCoreConfigCRT import LibretroCoreConfigCRT
            core_config = LibretroCoreConfigCRT().createConfigFor(system)
            for core_option in core_config.items():
                coreConfig.setString(core_option[0], core_option[1])

        retroarchConfig.saveFile()
        coreConfig.saveFile()
        # Most specific code ever (it's here because of *.retroarch.cfg in /recalbox/share_init/roms/vectrex)
        if system.Name == "vectrex":
            retroarchOverrides.setString("aspect_ratio_index", retroarchConfig.getString("aspect_ratio_index", "24"))
            retroarchOverrides.saveFile()
        if system.Name == "n64":
            retroarchConfig.setBool("video_smooth", True)

    # Create tate mode configuration
    @staticmethod
    def createTateModeConfiguration(system: Emulator) -> (Dict[str, Any], Dict[str, Any]):
        config = {
            "video_allow_rotate": '"true"',
            "input_overlay_auto_rotate": "true",
            "video_rotation": 0,
        }
        coreConfig = {
            "wswan_rotate_display": '"manual"',
            "wswan_rotate_keymap": '"enabled"'
        }

        if system.Rotation.value > 0:
            print("[Configgen.libretroGenerator] Rotation enabled: {}".format(system.Rotation.value))
            config["video_rotation"] = system.Rotation.value
            if system.VerticalGame:
                config["aspect_ratio_index"] = 0
            else:
                config["aspect_ratio_index"] = 8
            config["video_scale_integer"] = '"false"'
            if system.Core == "mednafen_wswan":
                config["video_rotation"] = (system.Rotation.value + 1) % 4
            if system.Core == "flycast" or system.Core == "flycast-next":
                if system.Name == "dreamcast":
                    config["video_rotation"] = (system.Rotation.value + 1) % 4
                config["input_player1_analog_dpad_mode"] = 3
            if system.Name == "saturn":
                config["video_rotation"] = (system.Rotation.value + 1) % 4

        return config, coreConfig

    @staticmethod
    def configureBsnesHD(system: Emulator) -> Dict[str, Any]:
        return {"bsnes_mode7_scale": '"1x"', "bsnes_mode7_wsMode": '"all"' if system.WideScreenMode else '"none"'}

    @staticmethod
    def configureGenesisWide(system: Emulator) -> Dict[str, Any]:
        if not system.WideScreenMode:
            return {"genesis_plus_gx_wide_h40_extra_columns": "0"}
        from configgen.utils.resolutions import ResolutionParser
        ratio = ResolutionParser(system.VideoMode).ratio()
        currentColumns = 10
        # Ratio to columns (16/9 and more => 10 cols, 1.6 = 16/10 => 6 cols, 1.5 = GOA => 2 cols, 1.34 = 4/3 => 0 cols)
        if ratio <= 1.6:
            currentColumns = 6
        if ratio <= 1.5:
            currentColumns = 2
        if ratio <= 1.34:
            currentColumns = 0
        config: Dict[str, Any] = {"genesis_plus_gx_wide_h40_extra_columns": "{}".format(currentColumns), }
        return config

    def configureFlycastWide(system: Emulator) -> Dict[str, Any]:
        return {"reicast_widescreen_hack": '"enabled"' if system.WideScreenMode else '"disabled"'}

    @staticmethod
    def createHDWidescreenConfig(system: Emulator) -> (Dict[str, Any], Dict[str, Any], str):
        libretroConfig: Dict[str, Any] = {}
        coreConfig: Dict[str, Any] = {}
        choosenCore = system.Core
        # Skip widescreen when tate
        system.WideScreenMode = system.WideScreenMode and system.Rotation == Rotation.none

        systemsToWideScreenOrHdCore = {"snes": "bsneshd", "megadrive": "genesisplusgxwide", "dreamcast": "flycast",
                                       "naomi": "flycast", "atomiswave": "flycast", "psx": "pcsx_rearmed",
                                       "saturn": "yabasanshiro", "n64": "parallel_n64"}
        widescreenCoreToCoreConfig = {"bsneshd": LibretroGenerator.configureBsnesHD,
                                      "genesisplusgxwide": LibretroGenerator.configureGenesisWide,
                                      "flycast": LibretroGenerator.configureFlycastWide}
        hdCoreToCoreConfig = {
            "flycast":
                [
                    {"reicast_internal_resolution": '"1024x768"', "reicast_cable_type": '"VGA (RGB)"'},
                    {"reicast_internal_resolution": '"640x480"', "reicast_cable_type": '"VGA (RGB)"'}
                ],
            "pcsx_rearmed":
                [
                    {"pcsx_rearmed_neon_enhancement_enable": '"enabled"'},
                    {"pcsx_rearmed_neon_enhancement_enable": '"disabled"'}
                ],
            "yabasanshiro":
                [
                    {"yabasanshiro_resolution_mode": '"2x"'},
                    {"yabasanshiro_resolution_mode": '"1x"'}
                ],
            "parallel_n64":
                [
                    {"parallel-n64-parallel-rdp-upscaling": '"4x"', "parallel-n64-screensize": '"1440x1080"'},
                    {"parallel-n64-parallel-rdp-upscaling": '"1x"', "parallel-n64-screensize": '"320x240"' },
                ]
        }

        # HD
        if system.Name in systemsToWideScreenOrHdCore:
            if system.HDMode:
                choosenCore = systemsToWideScreenOrHdCore[system.Name]
                print("[Configgen.libretroGenerator] Setting new core for hdmode : {}".format(choosenCore))
            if choosenCore in hdCoreToCoreConfig:
                for config in hdCoreToCoreConfig[choosenCore][0 if system.HDMode else 1].items():
                    coreConfig[config[0]] = config[1]

        # Widescreen
        if system.Name in systemsToWideScreenOrHdCore:
            if system.WideScreenMode:
                choosenCore = systemsToWideScreenOrHdCore[system.Name]
                print("[Configgen.libretroGenerator] Setting new core for widescreenmode : {}".format(choosenCore))
            if choosenCore in widescreenCoreToCoreConfig:
                for config in widescreenCoreToCoreConfig[choosenCore](system).items():
                    coreConfig[config[0]] = config[1]

        # Update system wide widescreen if the widescreen is really activated
        system.WideScreenMode = system.WideScreenMode and choosenCore in widescreenCoreToCoreConfig.keys()
        return libretroConfig, coreConfig, choosenCore

    # Create run ahead configuration
    @staticmethod
    def createRunAheadConfiguration(system: Emulator, retroarchConfig: keyValueSettings):
        defaults = {
            "run_ahead_secondary_instance": "false",
            "run_ahead_enabled": "false",
            "run_ahead_frames": "1"
        }
        activated = {
            "run_ahead_secondary_instance": "false",
            "run_ahead_enabled": "true",
            "run_ahead_frames": "1"
        }
        RunAheadSupportedSystems = ["snes", "megadrive", "mastersystem", "nes", "gb", "gbc", "gamegear"]
        RunAheadSupportedCores = ["fbneo"]
        configToSet = activated if system.RunAhead and (
                system.Name in RunAheadSupportedSystems or system.Core in RunAheadSupportedCores) else defaults
        for option in configToSet.items():
            retroarchConfig.setString(option[0], option[1])
        retroarchConfig.saveFile()

    # Create zerolag configuration
    @staticmethod
    def createReduceLatencyConfiguration(system: Emulator, retroarchConfig: keyValueSettings):
        defaults = {
            "video_max_swapchain_images": "3",
            "video_frame_delay_auto": "true",
        }
        activated = {
            "video_max_swapchain_images": "2",
            "video_frame_delay_auto": "true",
        }
        ReduceLatencyupportedSystems = ["snes", "megadrive", "mastersystem", "nes", "gb", "gbc", "gamegear"]
        ReduceLatencyupportedCores = ["fbneo"]
        configToSet = activated if system.ReduceLatency and (
                system.Name in ReduceLatencyupportedSystems or system.Core in ReduceLatencyupportedCores) else defaults
        for option in configToSet.items():
            retroarchConfig.setString(option[0], option[1])
        retroarchConfig.saveFile()

    @staticmethod
    def createSuperGameBoyConfiguration(system: Emulator, retroarchConfig: keyValueSettings,
                                        coreConfig: keyValueSettings):
        coreConfig.setString("mgba_sgb_borders", '"OFF"')
        coreConfig.setString("mgba_gb_model", '"Autodetect"')
        coreConfig.setString("mesen-s_gbmodel", '"Auto"')

        if system.Name in ["gb", "gbc"] and system.SuperGameBoy:
            retroarchConfig.setString("aspect_ratio_index", '"22"')
            coreConfig.setString("mgba_sgb_borders", '"ON"')
            coreConfig.setString("mgba_gb_model", '"Super Game Boy"')
            coreConfig.setString("mesen-s_gbmodel", '"Super Game Boy"')
        retroarchConfig.saveFile()
        coreConfig.saveFile()

    @staticmethod
    def createVideoDriverConfiguration(system: Emulator, retroarchConfig: keyValueSettings):
        vulkanCores = ["flycast-next", "ppsspp"]
        naomi2OnPi5 = system.Name == "naomi2" and Architecture.isPi5
        if Vulkan.vulkanAvailable() and system.Core in vulkanCores and system.VulkanDriver and not naomi2OnPi5:
            retroarchConfig.setString("video_driver", "vulkan")
        else:
            retroarchConfig.removeOption("video_driver")
        if system.Core == "scummvm" and system.HasShaderFile:
            retroarchConfig.setString("video_threaded", "false")
            retroarchConfig.setString("video_shader_enable", system.HasShaderFile)
        retroarchConfig.saveFile()

    @staticmethod
    def createFBNeoBlitterConfig(system: Emulator, rom: str) -> (Dict[str, str]):
        romName = os.path.basename(rom).removesuffix(".zip")
        blitterConfig = {
            "mmpork": [60, 53],
            "deathsml": [53,57],
            "dsmbl": [53, 57],
            "espgal2": [45, 55],
            "espgal2a": [45, 55],
            "espgal2b": [45, 55],
            "mushisam": [100, 60],
            "mushisama": [100, 60],
            "mushisamb": [100, 60],
            "futaribl": [100, 57],
            "futariblj": [100, 57],
            "akatana": [49, 63],
            "ddpsdoj": [45, 57],
        }
        if system.Core == "fbneo" and romName in blitterConfig:
            return {
                "fbneo-cpu-speed-adjust": '"{}%"'.format(blitterConfig[romName][0]),
                "fbneo-dipswitch-{}-Blitter_Delay".format(romName): '"{}"'.format(blitterConfig[romName][1]),
            }
        return {"fbneo-cpu-speed-adjust": '"100%"'}

    @staticmethod
    def createRumbleconfiguration(system: Emulator) -> (Dict[str, Any], Dict[str, Any]):
        retroarchConfig: Dict[str, Any] = {}
        coreConfig: Dict[str, Any] = {}

        # DualShock
        rumbleRetroarchConfig = {
            "pcsx_rearmed":
                [
                    {"input_libretro_device_p0": '"512"'},
                    {"input_libretro_device_p0": '"1"'}
                ],
            "mednafen_psx_hw":
                [
                    {"input_libretro_device_p0": '"517"'},
                    {"input_libretro_device_p0": '"1"'}
                ],
            "mednafen_psx":
                [
                    {"input_libretro_device_p0": '"517"'},
                    {"input_libretro_device_p0": '"1"'}
                ],
            "swanstation":
                [
                    {"input_libretro_device_p0": '"261"'},
                    {"input_libretro_device_p0": '"1"'}
                ]
        }

        rumbleCoreConfig = {
            "dolphin":
                [
                    {"dolphin_enable_rumble": '"enabled"'},
                    {"dolphin_enable_rumble": '"disabled"'}
                ],
            "flycast":
                [
                    {"reicast_enable_purupuru": '"enabled"'},
                    {"reicast_enable_purupuru": '"disabled"'}
                ],
            "flycast-next":
                [
                    {"reicast_enable_purupuru": '"enabled"'},
                    {"reicast_enable_purupuru": '"disabled"'}
                ],
            "mupen64plus_next":
                [
                    {"mupen64plus-pak1": '"rumble"'},
                    {"mupen64plus-pak1": '"memory"'}
                ],
            "parallel_n64":
                [
                    {"parallel-n64-pak1": '"rumble"'},
                    {"parallel-n64-pak1": '"memory"'}
                ],
            "pcsx_rearmed":
                [
                    {"pcsx_rearmed_vibration": '"enabled"'},
                    {"pcsx_rearmed_vibration": '"disabled"'}
                ],
            "pcsx2":
                [
                    {"pcsx2_rumble_enable": '"enabled"'},
                    {"pcsx2_rumble_enable": '"disabled"'}
                ],
            "swanstation":
                [
                    {"swanstation_Controller_EnableRumble": '"true"'},
                    {"swanstation_Controller_EnableRumble": '"false"'}
                ]
        }

        if system.Core in rumbleRetroarchConfig:
            for config in rumbleRetroarchConfig[system.Core][0 if system.Rumble else 1].items():
                retroarchConfig[config[0]] = config[1]

        if system.Core in rumbleCoreConfig:
            for config in rumbleCoreConfig[system.Core][0 if system.Rumble else 1].items():
                coreConfig[config[0]] = config[1]

        return retroarchConfig, coreConfig

    def createRemapConfig(system: Emulator, rom: str, config: keyValueSettings):
        remapConfig = keyValueSettings(recalboxFiles.retroarchRemapCommon, True)
        remapConfig.loadFile(True)
        remapConfig.clear()
        remapConfigDevice = config.getOptionByRegex('input_libretro_device_p\d+$')

        for option in remapConfigDevice.items():
            remapConfig.setString(option[0], '"' + option[1] + '"')

        for i in range(1, len(remapConfigDevice) + 1):
            value = config.getInt("input_player{}_analog_dpad_mode".format(i), 0)
            remapConfig.setString("input_player{}_analog_dpad_mode".format(i), '"' + str(value) + '"')
            remapConfig.setString("input_remap_port_p{}".format(i), '"' + str(i - 1) + '"' )

        remapConfig.saveFile()

    # recalbox-crt-options.cfg options
    # Create configuration file
    @staticmethod
    def createConfigurationFile(system: Emulator, playersControllers: ControllerPerPlayer, rom: str, demo: bool,
                                nodefaultkeymap: bool, recalboxOptions: keyValueSettings) -> (str, str, List[str]):
        # Setup system configuration
        configuration = libretroConfigurations.LibretroConfiguration(system, playersControllers, rom, demo,
                                                                     nodefaultkeymap, recalboxOptions)
        retroarchConfig, retroarchOverrides = configuration.createRetroarchConfiguration()
        coreConfig = configuration.createCoreConfiguration()

        # Rumble configuration - defined before lightgun to not break lightgun feature
        rumbleRetroarchConfig, rumbleCoreConfig = LibretroGenerator.createRumbleconfiguration(system)
        for option in rumbleRetroarchConfig.items():
            retroarchConfig.setString(option[0], option[1])
        retroarchConfig.saveFile()
        for option in rumbleCoreConfig.items():
            coreConfig.setString(option[0], option[1])
        coreConfig.saveFile()

        # setup wiimotes lightgun configuration
        from configgen.generators.libretro.libretroLightGuns import libretroLightGun
        lightgunConfig = libretroLightGun(system, rom, demo, retroarchConfig, coreConfig)
        lightgunConfig.createLightGunConfiguration()

        # tate mode config
        tateLRConfig, tateCoreConfig = LibretroGenerator.createTateModeConfiguration(system)
        for option in tateLRConfig.items():
            retroarchConfig.setString(option[0], option[1])
        retroarchConfig.saveFile()
        for option in tateCoreConfig.items():
            coreConfig.setString(option[0], option[1])
        coreConfig.saveFile()

        # Supergameboy config, core is selected by ES. Overlays are processed after that so there is a specific rule in it
        LibretroGenerator.createSuperGameBoyConfiguration(system, retroarchConfig, coreConfig)

        # HD and widescreen config
        libretroConfigHD, coreConfigHD, newCoreHD = LibretroGenerator.createHDWidescreenConfig(system)
        for option in libretroConfigHD.items():
            retroarchConfig.setString(option[0], option[1])
        retroarchConfig.saveFile()
        for option in coreConfigHD.items():
            coreConfig.setString(option[0], option[1])
        coreConfig.saveFile()
        system.Core = newCoreHD

        # video driver config
        LibretroGenerator.createVideoDriverConfiguration(system, retroarchConfig)

        # crt config
        # - should be after tate as it will change ratio but keep other tate config
        # - should be after hd and widescreen as it set resolutions and scaling
        if system.CRTEnabled:
            LibretroGenerator.createCrtConfiguration(system, rom, recalboxOptions, retroarchConfig, coreConfig,
                                                     retroarchOverrides)
        # Latency config
        LibretroGenerator.createReduceLatencyConfiguration(system, retroarchConfig)
        # Run Ahead config
        LibretroGenerator.createRunAheadConfiguration(system, retroarchConfig)
        # FBNeo Blitter Config
        blitterCoreConfig = LibretroGenerator.createFBNeoBlitterConfig(system, rom)
        for option in blitterCoreConfig.items():
            coreConfig.setString(option[0], option[1])
        coreConfig.saveFile()

        # Netplay (force in configfile nick and port)
        netplayConfig = LibretroGenerator.createNetplayConfig(system)
        for option in netplayConfig.items():
            retroarchConfig.setString(option[0], option[1])
        retroarchConfig.saveFile()

        # Disable notification when use common remap file
        retroarchConfig.setString("notification_show_remap_load", "false")
        coreCapitalize = system.Core.capitalize()
        romName = os.path.splitext(os.path.basename(rom))[0]
        if (os.path.isfile("{}/{}/{}.rmp".format(recalboxFiles.retroarchRemap, coreCapitalize, coreCapitalize))
                or os.path.isfile("{}/{}/{}.rmp".format(recalboxFiles.retroarchRemap, coreCapitalize, system.Name))
                or os.path.isfile("{}/{}/{}.rmp".format(recalboxFiles.retroarchRemap, coreCapitalize, romName))):
            retroarchConfig.setString("notification_show_remap_load", "true")

        # Reduce volume for flycast
        if system.Core == "flycast" or system.Core == "flycast-next":
            retroarchConfig.setString("audio_volume", "-2.0")
        else:
            retroarchConfig.setString("audio_volume", "0.0")

        # Force 10 players for Saturn
        retroarchConfig.setString("input_max_users", '"8"')
        if system.Name == "saturn":
            retroarchConfig.setString("input_max_users", '"12"')

        retroarchConfig.saveFile()

        # Create remap file
        LibretroGenerator.createRemapConfig(system, rom, retroarchConfig)

        commandArgs = configuration.getCommandLineArguments(retroarchConfig, coreConfig)

        return configuration.getRetroarchConfigurationFileName(), \
            configuration.getRetroarchOverridesFileName(), \
            commandArgs

    # Configure retroarch and return a command
    def generate(self, system: Emulator, playersControllers: ControllerPerPlayer, recalboxOptions: keyValueSettings,
                 args):

        # Set recalbox default config file if no user defined one
        newConfigFileName, overrideFileName, commandArgs = self.createConfigurationFile(system, playersControllers,
                                                                                        args.rom, args.demo,
                                                                                        args.nodefaultkeymap,
                                                                                        recalboxOptions)
        configFileName = system.ConfigFile if system.HasConfigFile else newConfigFileName

        # Manage special scummvm roms
        # To keep compatibility with existing scummvm scraping systems as well as with the standalone core,
        # rom may contain the upper folder game_folder.scummvm
        # In such case we must look for the inner file.scummvm and use it instead
        rom = args.rom
        if system.Core == 'scummvm':
            import os.path
            if os.path.isdir(args.rom):
                scummfiles = [fn for fn in os.listdir(args.rom) if fn.endswith('.scummvm')]
                if len(scummfiles) == 1:
                    rom = os.path.join(args.rom, scummfiles[0])

        # Retroarch core on the filesystem
        retroarchCore = recalboxFiles.retroarchCores + system.Core + recalboxFiles.libretroExt

        # The command to run
        commandArray = [recalboxFiles.recalboxBins[system.Emulator]]
        # Verbose?
        if args.verbose:
            commandArray.extend(["--verbose"])

        # Disabled softpatching
        if args.disabledsoftpatching:
            commandArray.extend(["--no-patch"])

        # ips patch path
        if args.ips != '':
            commandArray.extend(["--ips", args.ips])

        # bps patch path
        if args.bps != '':
            commandArray.extend(["--bps", args.bps])

        # ups patch path
        if args.ups != '':
            commandArray.extend(["--ups", args.ups])

        # entryslot number
        if args.entryslot != '':
            commandArray.extend(["--entryslot", args.entryslot])

        # Core & config
        commandArray.extend(["-L", retroarchCore, "--config", configFileName])
        # Extra configs - pass in-place override last
        commandArray.extend(self.getAppendConfigs(system, rom, overrideFileName, recalboxOptions))
        # Netplay mode
        commandArray.extend(self.getNetplayArguments(system))
        # Converted command args
        commandArray.extend(commandArgs)
        # Extra system/core arguments
        commandArray.extend(self.buildExtraArguments(args.system, system.Core))

        # Optional arguments from recalbox.conf
        if system.HasArgs: commandArray.extend(system.Args)

        # Sub-system roms (pc88)
        roms = self.buildRomArguments(rom, system.Core, args.verbose, args.demo)
        commandArray.extend(roms)

        return Command(videomode=system.VideoMode, array=commandArray)

    @staticmethod
    def buildExtraArguments(system: str, core: str) -> List[str]:
        if system == "neogeocd" and core == "fbneo":
            return ["--subsystem", "neocd"]
        if system == "channelf" and core == "fbneo":
            return ["--subsystem", "chf"]

        return []

    @staticmethod
    def buildRomArguments(rom: str, core: str, verbose: bool, demo: bool) -> List[str]:
        # x68000
        if core == "px68k":
            # Open configuration
            import os.path
            x68kconfig = os.path.join(recalboxFiles.BIOS, "keropi/config")
            from configgen.settings.iniSettings import IniSettings
            settings = IniSettings(x68kconfig, False)
            # HDD or floppy disk?
            size = os.path.getsize(rom) >> 20
            if size >= 2:
                # HDD
                settings.setString("WinX68k", "HDD0", rom)
            else:
                # Floppy
                from configgen.utils.diskCollector import DiskCollector
                collector = DiskCollector(rom, 2, verbose)
                settings.setString("WinX68k", "FDD0", collector.disks[0])
                if collector.Count > 1:
                    settings.setString("WinX68k", "FDD1", collector.disks[1])
            settings.saveFile()

        # quasi88 (Pc88) use retroarch subsystem's to load multiple content
        if core == "quasi88":
            from configgen.utils.diskCollector import DiskCollector
            collector = DiskCollector(rom, 6, verbose)
            if collector.Count > 1:
                roms = ["--subsystem", "pc88_{}_disk".format(collector.Count)]
                roms.extend(collector.Disks)
                return roms

        # Vic20 core with split cartridges
        if core == "vice_xvic":
            import os.path
            rom, romExt = os.path.splitext(rom)
            cartridges = \
                {
                    ".20": "-cart2",
                    ".40": "-cart4",
                    ".60": "-cart6",
                    ".70": "-cart6",
                    ".a0": "-cartA",
                    ".b0": "-cartB",
                }
            if romExt in cartridges:
                carts = []
                for ext in cartridges:
                    if os.path.exists(rom + ext):
                        carts.append(cartridges[ext])
                        carts.append('"' + rom + ext + '"')
                return ['xvic {}'.format(' '.join(carts))]
            else:
                rom = rom + romExt

        # Demo mode: take the first disk, always
        if demo:
            from configgen.utils.diskCollector import DiskCollector
            collector = DiskCollector(rom, 6, verbose)
            if collector.Count > 1:
                return [collector.Disks[0]]

        # macintosh
        if core == "minivmac":
            with open(recalboxFiles.minivmacRomFile, 'w') as f:
                f.write('"Mini vMac" "' + recalboxFiles.minivmacOsFile + '" "' + rom + '"')

            rom = [recalboxFiles.minivmacRomFile]
            return rom

        return [rom]
