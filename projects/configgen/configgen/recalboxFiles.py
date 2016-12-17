#!/usr/bin/env python
HOME_INIT = '/recalbox/share_init/system/'
HOME = '/recalbox/share/system'
CONF = HOME + '/configs'
SAVES = '/recalbox/share/saves'
SCREENSHOTS = '/recalbox/share/screenshots'
BIOS = '/recalbox/share/bios'
OVERLAYS = '/recalbox/share/overlays'

esInputs = HOME + '/.emulationstation/es_input.cfg'
esSettings = HOME + '/.emulationstation/es_settings.cfg'
recalboxConf = HOME + '/recalbox.conf'
logdir = HOME + '/logs/'

# This dict is indexed on the emulator name, not on the system
recalboxBins = {'dosbox'      : '/usr/bin/dosbox'
              , 'fba2x'       :'/usr/bin/fba2x'
              , 'kodi'        : '/recalbox/scripts/kodilauncher.sh'
              , 'libretro'    : '/usr/bin/retroarch'
              , 'linapple'    : '/usr/bin/linapple'
              , 'moonlight'   : '/usr/bin/moonlight'
              , 'mupen64plus' : '/usr/bin/mupen64plus'
              , 'ppsspp'      : '/usr/bin/PPSSPPSDL'
              , 'reicast'     : '/usr/bin/reicast.elf'
              , 'scummvm'     : '/usr/bin/scummvm'
              , 'vice'        : '/usr/bin/x64'
              , 'dolphin'     : '/usr/bin/dolphin-emu-nogui'
}


retroarchRoot = CONF + '/retroarch'
retroarchCustom = retroarchRoot + '/retroarchcustom.cfg'
retroarchCustomOrigin = retroarchRoot + "/retroarchcustom.cfg.origin"
retroarchCoreCustom = retroarchRoot + "/cores/retroarch-core-options.cfg"

retroarchCores = "/usr/lib/libretro/"
shadersRoot = "/recalbox/share/shaders/presets/"
shadersExt = '.gplsp'
libretroExt = '_libretro.so'
screenshotsDir = "/recalbox/share/screenshots/"
savesDir = "/recalbox/share/saves/"

fbaRoot = CONF + '/fba/'
fbaCustom = fbaRoot + 'fba2x.cfg'
fbaCustomOrigin = fbaRoot + 'fba2x.cfg.origin'


mupenConf = CONF + '/mupen64/'
mupenCustom = mupenConf + "mupen64plus.cfg"
mupenInput = mupenConf + "InputAutoCfg.ini"
mupenSaves = SAVES + "/n64"
mupenMappingUser    = mupenConf + 'input.xml'
mupenMappingSystem  = '/recalbox/share_init/system/configs/mupen64/input.xml'

shaderPresetRoot = "/recalbox/share_init/system/configs/shadersets/"

kodiJoystick = HOME + '/.kodi/userdata/keymaps/recalbox.xml'
kodiMappingUser    = CONF + '/kodi/input.xml'
kodiMappingSystem  = '/recalbox/share_init/system/configs/kodi/input.xml'


moonlightCustom = CONF+'/moonlight'
moonlightConfig = moonlightCustom + '/moonlight.conf'
moonlightGamelist = moonlightCustom + '/gamelist.txt'
moonlightMapping = dict()
moonlightMapping[1] = moonlightCustom + '/mappingP1.conf'
moonlightMapping[2] = moonlightCustom + '/mappingP2.conf'
moonlightMapping[3] = moonlightCustom + '/mappingP3.conf'
moonlightMapping[4] = moonlightCustom + '/mappingP4.conf'

reicastCustom = CONF + '/reicast'
reicastConfig = reicastCustom + '/emu.cfg'
reicastConfigInit = HOME_INIT + 'configs/reicast/emu.cfg'
reicastSaves = SAVES
reicastBios = BIOS

dolphinConfig = CONF + "/dolphin-emu"
dolphinData   = SAVES + "/dolphin-emu"
dolphinIni    = dolphinConfig + '/Dolphin.ini'

ppssppConf = CONF + '/ppsspp/PSP/SYSTEM'
ppssppControls = CONF + '/ppsspp/gamecontrollerdb.txt'
ppssppControlsInit = HOME_INIT + 'configs/ppsspp/PSP/SYSTEM/controls.ini'
ppssppConfig = ppssppConf + '/ppsspp.ini'

dosboxCustom = CONF + '/dosbox'
dosboxConfig = dosboxCustom + '/dosbox.conf'

scummvmSaves = SAVES + '/scummvm'

viceConfig = CONF + "/vice/vice.conf"
