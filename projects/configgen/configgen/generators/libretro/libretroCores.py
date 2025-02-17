from configgen.Emulator import Emulator
from configgen.crt.CRTTypes import CRTAdapter
from configgen.settings.keyValueSettings import keyValueSettings
from configgen.controllers.controller import ControllerPerPlayer
from configgen.utils.architecture import Architecture


class LibretroCores:

    def __init__(self, system: Emulator, settings: keyValueSettings, controllers: ControllerPerPlayer, rom: str):
        self.system: Emulator = system
        self.settings: keyValueSettings = settings
        self.controllers: ControllerPerPlayer = controllers
        self.rom = rom

    #
    # Core specific configurations
    #

    @staticmethod
    def configureBlueMSX(coreSettings: keyValueSettings):
        # Default core configuration - Actual value for legacy msx folder
        coreSettings.setString("bluemsx_msxtype", '"Auto"')

    @staticmethod
    def configureMAME2003plus(coreSettings: keyValueSettings):
        coreSettings.setString("mame2003-plus_analog", '"digital"')

    @staticmethod
    def configureOpera(coreSettings: keyValueSettings):
        coreSettings.setString("opera_dsp_threaded", '"enabled"')

    @staticmethod
    def configurePARALLELN64(coreSettings: keyValueSettings):
        coreSettings.setString("parallel-n64-boot-device", '"Default"')
        coreSettings.setString("parallel-n64-64dd-hardware", '"disabled"')
        coreSettings.setString("parallel-n64-gfxplugin", '"auto"')
        coreSettings.setString("parallel-n64-alt-map", '"disabled"')

    @staticmethod
    def configureMUPEN64(coreSettings: keyValueSettings):
        coreSettings.setString("mupen64plus-alt-map", '"False"')

    @staticmethod
    # (pc98) force the "joymode" option to have automatic joystick support.
    def configureNPKAI(coreSettings: keyValueSettings):
        coreSettings.setString("np2kai_joymode", '"Keypad"')

    #
    # System specific configurations
    #

    @staticmethod
    def configureAmigaCDTV(coreSettings: keyValueSettings):
        coreSettings.setString("puae_model", '"CDTV"')

    @staticmethod
    def configureAmigaCD32(coreSettings: keyValueSettings):
        coreSettings.setString("puae_model", '"CD32"')

    @staticmethod
    def configureAmiga1200(coreSettings: keyValueSettings):
        coreSettings.setString("puae_model", '"A1200"')

    @staticmethod
    def configureAmiga600(coreSettings: keyValueSettings):
        coreSettings.setString("puae_model", '"A600"')

    @staticmethod
    def configureAmstradCPC(coreSettings: keyValueSettings):
        coreSettings.setString("cap32_model", '"6128"')

    @staticmethod
    def configureAmstradGX4000(coreSettings: keyValueSettings):
        coreSettings.setString("cap32_model", '"6128+ (experimental)"')
        coreSettings.setString("cap32_gfx_colors", "24bit")

    @staticmethod
    def configureAtari5200(coreSettings: keyValueSettings):
        coreSettings.setString("atari800_system", '"5200"')

        import configgen.recalboxFiles as recalboxFiles
        a800settings = keyValueSettings(recalboxFiles.atari800CustomConfig)
        a800settings.loadFile(True)
        a800settings.setString("MACHINE_TYPE", "Atari 5200")
        a800settings.setInt("RAM_SIZE", 16)
        a800settings.setInt("STEREO_POKEY", 0)
        a800settings.setInt("BUILTIN_BASIC", 0)
        a800settings.saveFile()

    @staticmethod
    def configureAtari800(coreSettings: keyValueSettings):
        coreSettings.setString("atari800_system", '"130XE (128K)"')

        import configgen.recalboxFiles as recalboxFiles
        a800settings = keyValueSettings(recalboxFiles.atari800CustomConfig)
        a800settings.loadFile(True)
        a800settings.setString("MACHINE_TYPE", "Atari XL/XE")
        a800settings.setInt("RAM_SIZE", 64)
        a800settings.setInt("STEREO_POKEY", 1)
        a800settings.setInt("BUILTIN_BASIC", 1)
        a800settings.saveFile()

    def configureAtariST(self, _):
        #print("ATARI DETECTED!")
        MACHINE_ST = 0
        MACHINE_STE = 1
        MACHINE_TT = 2
        MACHINE_FALCON = 3,
        MACHINE_MEGA_STE = 4

        # Load config
        from configgen.settings.iniSettings import IniSettings
        import configgen.recalboxFiles as recalboxFiles
        atariStSettings = IniSettings(recalboxFiles.hatariCustomConfig)
        atariStSettings.loadFile(True)

        # Auto set machine
        from typing import Dict
        # Try identify machine folder
        machines: Dict[str, int] = \
        {
            "atari-st": MACHINE_ST,
            "/st/": MACHINE_ST,
            "atariste": MACHINE_STE,
            "atari-ste": MACHINE_STE,
            "/ste/": MACHINE_STE,
            "ataritt": MACHINE_TT,
            "atari-tt": MACHINE_TT,
            "/tt/": MACHINE_TT,
            "atarimegaste": MACHINE_MEGA_STE,
            "atari-megaste": MACHINE_MEGA_STE,
            "atari-mega-ste": MACHINE_MEGA_STE,
            "megaste": MACHINE_MEGA_STE,
            "mega-ste": MACHINE_MEGA_STE,
            "falcon": MACHINE_FALCON,
            "atari-falcon": MACHINE_FALCON,
        }
        defaultMachine: int = MACHINE_ST
        romLower: str = self.rom.lower()
        for key in machines:
            if key in romLower:
                defaultMachine: int = machines[key]
                break
        print("MACHINE: {}".format(defaultMachine))
        atariStSettings.setString("System", "nMachineType", str(defaultMachine))

        # Select bios
        import os
        subdir: str = "atarist"
        biosPath = os.path.join(recalboxFiles.BIOS, subdir, "st.img")
        if   defaultMachine == MACHINE_STE     : biosPath = os.path.join(recalboxFiles.BIOS, subdir, "ste.img")
        elif defaultMachine == MACHINE_TT      : biosPath = os.path.join(recalboxFiles.BIOS, subdir, "tt.img")
        elif defaultMachine == MACHINE_MEGA_STE: biosPath = os.path.join(recalboxFiles.BIOS, subdir, "megaste.img")
        elif defaultMachine == MACHINE_FALCON  : biosPath = os.path.join(recalboxFiles.BIOS, subdir, "falcon.img")
        if not os.path.exists(biosPath):
            biosPath = os.path.join(recalboxFiles.BIOS, subdir, "tos.img")
            if not os.path.exists(biosPath):
                biosPath = os.path.join(recalboxFiles.BIOS, "tos.img")
                if not os.path.exists(biosPath):
                    biosPath = os.path.join(recalboxFiles.BIOS, subdir, "emutos.img")
        if "emutos" in self.rom:
            biosPath = os.path.join(recalboxFiles.BIOS, subdir, "emutos.img")
        atariStSettings.setString("ROM", "szTosImageFileName", str(biosPath))
        print("BIOS   : {}".format(biosPath))

        # Save config
        atariStSettings.saveFile()

    @staticmethod
    def configure64DD(coreSettings: keyValueSettings):
        coreSettings.setString("parallel-n64-boot-device", '"64DD IPL"')
        coreSettings.setString("parallel-n64-64dd-hardware", '"enabled"')
        coreSettings.setString("parallel-n64-gfxplugin", '"gln64"')
        coreSettings.setString("parallel-n64-alt-map", '"disabled"')
        coreSettings.setString("mupen64plus-alt-map", '"False"')

    @staticmethod
    def configureSpectravideo(coreSettings: keyValueSettings):
        coreSettings.setString("bluemsx_msxtype", '"SVI - Spectravideo SVI-328 MK2"')

    @staticmethod
    def configureMsx1(coreSettings: keyValueSettings):
        coreSettings.setString("bluemsx_msxtype", '"MSX"')

    @staticmethod
    def configureMsx2(coreSettings: keyValueSettings):
        coreSettings.setString("bluemsx_msxtype", '"MSX2+"')

    @staticmethod
    def configureMsxTurboR(coreSettings: keyValueSettings):
        coreSettings.setString("bluemsx_msxtype", '"MSXturboR"')

    @staticmethod
    def configureOdyssey2(coreSettings: keyValueSettings):
        coreSettings.setString("o2em_bios", '"o2rom.bin"')

    @staticmethod
    def configureVideoPacPlus(coreSettings: keyValueSettings):
        coreSettings.setString("o2em_bios", '"g7400.bin"')

    @staticmethod
    def configureSg1000(coreSettings: keyValueSettings):
        coreSettings.setString("bluemsx_msxtype", "SEGA - SC-3000")

    def configureNaomi2(self, coreSettings: keyValueSettings):
        if Architecture().isPi5:
            coreSettings.setString("reicast_alpha_sorting", '"per-strip (fast, least accurate)"')
            coreSettings.setString("reicast_anisotropic_filtering", '"2"')
            coreSettings.setString("reicast_sh4clock", '"300"')
        else:
            coreSettings.setString("reicast_alpha_sorting", '"per-triangle (normal)"')
            coreSettings.setString("reicast_anisotropic_filtering", '"4"')
            coreSettings.setString("reicast_sh4clock", '"200"')

    @staticmethod
    def configureSwanstation(coreSettings: keyValueSettings):
        coreSettings.setString("duckstation_Controller2.Type", '"DigitalController"')

    @staticmethod
    def configureDosBoxPure(coreSettings: keyValueSettings):
        coreSettings.setString("dosbox_pure_savestate", '"rewind"')

    def configureFlycast(self, coreSettings: keyValueSettings):
        coreSettings.setString("reicast_alpha_sorting", '"per-triangle (normal)"')
        coreSettings.setString("reicast_anisotropic_filtering", '"4"')
        coreSettings.setString("reicast_sh4clock", '"200"')
        if self.system.CRTAdapter == CRTAdapter.RECALBOXRGBJAMMA:
            coreSettings.setString("reicast_allow_service_buttons", '"enabled"')

    def configureFBNeo(self, coreSettings: keyValueSettings):
        if self.system.CRTAdapter == CRTAdapter.RECALBOXRGBJAMMA:
            coreSettings.setString("fbneo-diagnostic-input", '"L3"')

    def configureScummvm(self, coreSettings: keyValueSettings):
        import configgen.recalboxFiles as recalboxFiles
        import configparser
        scummvmSettings = configparser.ConfigParser(
                strict=False, allow_no_value=True, empty_lines_in_values=False
            )
        scummvmSettings.read(recalboxFiles.libretroScummvmConfig)
        scummvmSettings.set("scummvm", "extrapath", recalboxFiles.libretroScummvmBios)
        if self.system.CRTEnabled:
            scummvmSettings.set("scummvm", "gui_scale", "200")
        else:
            scummvmSettings.set("scummvm", "gui_scale", "100")
        with open(recalboxFiles.libretroScummvmConfig, 'w') as f:
            scummvmSettings.write(f)

    # Fill cores configuration
    def fillCoresConfiguration(self):
        settings = self.settings

        # Specific configuration handlers per core
        specificCoreHandlers =\
        {
            "bluemsx" : LibretroCores.configureBlueMSX,
            "mame2003_plus": LibretroCores.configureMAME2003plus,
            "mupen64plus_nx" : LibretroCores.configureMUPEN64,
            "opera": LibretroCores.configureOpera,
            "parallel_n64": LibretroCores.configurePARALLELN64,
            "np2kai" : LibretroCores.configureNPKAI,
            "swanstation" : LibretroCores.configureSwanstation,
            "dosbox_pure" : LibretroCores.configureDosBoxPure,
            "flycast": self.configureFlycast,
            "flycast-next": self.configureFlycast,
            "fbneo": self.configureFBNeo,
            "scummvm": self.configureScummvm,
        }

        # Get handler and execute
        core = self.system.Core
        if core in specificCoreHandlers:
            function = specificCoreHandlers.get(core)
            function(settings)

        # Specific configuration handlers per system
        specificSystemHandlers =\
        {
            "amiga600"     : LibretroCores.configureAmiga600,
            "amiga1200"    : LibretroCores.configureAmiga1200,
            "amigacdtv"    : LibretroCores.configureAmigaCDTV,
            "amigacd32"    : LibretroCores.configureAmigaCD32,
            "atari5200"    : LibretroCores.configureAtari5200,
            "atari800"     : LibretroCores.configureAtari800,
            "atarist"      : self.configureAtariST,
            "amstradcpc"   : LibretroCores.configureAmstradCPC,
            "gx4000"       : LibretroCores.configureAmstradGX4000,
            "spectravideo" : LibretroCores.configureSpectravideo,
            "64dd"         : LibretroCores.configure64DD,
            "msx1"         : LibretroCores.configureMsx1,
            "msx2"         : LibretroCores.configureMsx2,
            "msxturbor"    : LibretroCores.configureMsxTurboR,
            "pc98"         : LibretroCores.configureNPKAI,
            "odyssey2"     : LibretroCores.configureOdyssey2,
            "videopacplus" : LibretroCores.configureVideoPacPlus,
            "sg1000"       : LibretroCores.configureSg1000,
            "naomi2"       : self.configureNaomi2,
        }

        # Get handler and execute
        if self.system.Name in specificSystemHandlers:
            function = specificSystemHandlers.get(self.system.Name)
            function(settings)
