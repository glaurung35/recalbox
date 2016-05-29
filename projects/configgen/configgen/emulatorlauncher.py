#!/usr/bin/env python

import argparse
import time
from sys import exit
from Emulator import Emulator
import generators
from generators.libretro.libretroGenerator import LibretroGenerator
from generators.fba2x.fba2xGenerator import Fba2xGenerator
from generators.mupen.mupenGenerator import MupenGenerator
from generators.kodi.kodiGenerator import KodiGenerator
from generators.moonlight.moonlightGenerator import MoonlightGenerator
from generators.scummvm.scummvmGenerator import ScummVMGenerator
from generators.dosbox.dosboxGenerator import DosBoxGenerator
from generators.configManager import ConfigManager
import controllersConfig as controllers
import utils.runner as runner
import signal
import recalboxFiles
import os

generators = {
    'libretro': LibretroGenerator(),
    'fba2x': Fba2xGenerator(),
    'mupen64plus': MupenGenerator(),
    'kodi': KodiGenerator(),
    'moonlight': MoonlightGenerator(),
    'scummvm': ScummVMGenerator(),
    'dosbox': DosBoxGenerator()
}

# List emulators with their cores rest mupen64, scummvm
emulators = dict()
# Nintendo
emulators["snes"] = Emulator(name='snes', emulator='libretro', core='pocketsnes')
emulators["nes"] = Emulator(name='nes', emulator='libretro', core='fceunext')
emulators["n64"] = Emulator(name='n64', emulator='mupen64plus', core='gliden64')
emulators["gba"] = Emulator(name='gba', emulator='libretro', core='gpsp')
emulators["gb"] = Emulator(name='gb', emulator='libretro', core='gambatte')
emulators["gbc"] = Emulator(name='gbc', emulator='libretro', core='gambatte')
emulators["fds"] = Emulator(name='fds', emulator='libretro', core='nestopia')
emulators["virtualboy"] = Emulator(name='virtualboy', emulator='libretro', core='vb')
# Sega
emulators["sg1000"] = Emulator(name='sg1000', emulator='libretro', core='genesisplusgx')
emulators["mastersystem"] = Emulator(name='mastersystem', emulator='libretro', core='picodrive')
emulators["megadrive"] = Emulator(name='megadrive', emulator='libretro', core='picodrive')
emulators["gamegear"] = Emulator(name='gamegear', emulator='libretro', core='genesisplusgx')
emulators["sega32x"] = Emulator(name='sega32x', emulator='libretro', core='picodrive')
emulators["segacd"] = Emulator(name='segacd', emulator='libretro', core='picodrive')
# Arcade
emulators["neogeo"] = Emulator(name='neogeo', emulator='fba2x')
emulators["mame"] = Emulator(name='mame', emulator='libretro', core='mame078')
emulators["fba"] = Emulator(name='fba', emulator='fba2x')
emulators["fba_libretro"] = Emulator(name='fbalibretro', emulator='libretro', core='fba')
# Computers
emulators["msx"] = Emulator(name='msx', emulator='libretro', core='bluemsx')
emulators["msx1"] = Emulator(name='msx1', emulator='libretro', core='bluemsx')
emulators["msx2"] = Emulator(name='msx2', emulator='libretro', core='bluemsx')
emulators["amiga"] = Emulator(name='amiga', emulator='libretro', core='puae')
emulators["amstradcpc"] = Emulator(name='amstradcpc', emulator='libretro', core='cap32')
emulators["atarist"] = Emulator(name='atarist', emulator='libretro', core='hatari')
emulators["zxspectrum"] = Emulator(name='zxspectrum', emulator='libretro', core='fuse')
emulators["odyssey2"] = Emulator(name='odyssey2', emulator='libretro', core='o2em')
emulators["zx81"] = Emulator(name='zx81', emulator='libretro', core='81')
#
emulators["ngp"] = Emulator(name='ngp', emulator='libretro', core='mednafen_ngp')
emulators["ngpc"] = Emulator(name='ngpc', emulator='libretro', core='mednafen_ngp')
emulators["gw"] = Emulator(name='gw', emulator='libretro', core='gw')
emulators["vectrex"] = Emulator(name='vectrex', emulator='libretro', core='vecx')
emulators["lynx"] = Emulator(name='lynx', emulator='libretro', core='mednafen_lynx')
emulators["lutro"] = Emulator(name='lutro', emulator='libretro', core='lutro')
emulators["wswan"] = Emulator(name='wswan', emulator='libretro', core='mednafen_wswan', ratio='16/10')
emulators["wswanc"] = Emulator(name='wswanc', emulator='libretro', core='mednafen_wswan', ratio='16/10')
emulators["pcengine"] = Emulator(name='pcengine', emulator='libretro', core='mednafen_supergrafx')
emulators["pcenginecd"] = Emulator(name='pcenginecd', emulator='libretro', core='mednafen_supergrafx')
emulators["supergrafx"] = Emulator(name='supergrafx', emulator='libretro', core='mednafen_supergrafx')
emulators["atari2600"] = Emulator(name='atari2600', emulator='libretro', core='stella')
emulators["atari7800"] = Emulator(name='atari7800', emulator='libretro', core='prosystem')
emulators["prboom"] = Emulator(name='prboom', emulator='libretro', core='prboom')
emulators["psx"] = Emulator(name='psx', emulator='libretro', core='pcsx_rearmed')
emulators["cavestory"] = Emulator(name='cavestory', emulator='libretro', core='nxengine')
emulators["imageviewer"] = Emulator(name='imageviewer', emulator='libretro', core='imageviewer')
emulators["scummvm"] = Emulator(name='scummvm', emulator='scummvm', videomode='default')
emulators["pc"] = Emulator(name='pc', emulator='dosbox', videomode='default')

emulators["kodi"] = Emulator(name='kodi', emulator='kodi', videomode='default')
emulators["moonlight"] = Emulator(name='moonlight', emulator='moonlight')


def signal_handler(signal, frame):
    print('Exiting')
    if runner.proc:
        print('killing runner.proc')
        runner.proc.kill()


if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)

    parser = argparse.ArgumentParser(description='emulator-launcher script')
    parser.add_argument("-p1index", help="player1 controller index", type=int, required=False)
    parser.add_argument("-p1guid", help="player1 controller SDL2 guid", type=str, required=False)
    parser.add_argument("-p1name", help="player1 controller name", type=str, required=False)
    parser.add_argument("-p1devicepath", help="player1 controller device", type=str, required=False)
    parser.add_argument("-p2index", help="player2 controller index", type=int, required=False)
    parser.add_argument("-p2guid", help="player2 controller SDL2 guid", type=str, required=False)
    parser.add_argument("-p2name", help="player2 controller name", type=str, required=False)
    parser.add_argument("-p2devicepath", help="player2 controller device", type=str, required=False)
    parser.add_argument("-p3index", help="player3 controller index", type=int, required=False)
    parser.add_argument("-p3guid", help="player3 controller SDL2 guid", type=str, required=False)
    parser.add_argument("-p3name", help="player3 controller name", type=str, required=False)
    parser.add_argument("-p3devicepath", help="player3 controller device", type=str, required=False)
    parser.add_argument("-p4index", help="player4 controller index", type=int, required=False)
    parser.add_argument("-p4guid", help="player4 controller SDL2 guid", type=str, required=False)
    parser.add_argument("-p4name", help="player4 controller name", type=str, required=False)
    parser.add_argument("-p4devicepath", help="player4 controller device", type=str, required=False)
    parser.add_argument("-p5index", help="player5 controller index", type=int, required=False)
    parser.add_argument("-p5guid", help="player5 controller SDL2 guid", type=str, required=False)
    parser.add_argument("-p5name", help="player5 controller name", type=str, required=False)
    parser.add_argument("-p5devicepath", help="player5 controller device", type=str, required=False)    
    parser.add_argument("-system", help="select the system to launch", type=str, required=True)
    parser.add_argument("-rom", help="rom absolute path", type=str, required=True)
    parser.add_argument("-emulator", help="force emulator", type=str, required=False)
    parser.add_argument("-core", help="fore emulator core", type=str, required=False)
    parser.add_argument("-demo", help="mode demo", type=bool, required=False)

    args = parser.parse_args()

    playersControllers = dict()
    if not args.demo:
        # Read the controller configuration
        playersControllers = controllers.loadControllerConfig(args.p1index, args.p1guid, args.p1name, args.p1devicepath,
                                                              args.p2index, args.p2guid, args.p2name, args.p2devicepath,
                                                              args.p3index, args.p3guid, args.p3name, args.p3devicepath,
                                                              args.p4index, args.p4guid, args.p4name, args.p4devicepath,
                                                              args.p5index, args.p5guid, args.p5name, args.p5devicepath)

    systemName = args.system

    # Main Program
    # A generator will configure its emulator, and return a command
    if systemName in emulators:
        system = emulators[systemName]
        systemManager = ConfigManager()

        systemManager.configure(system, args.emulator, args.core)
	# Save dir
	if not os.path.exists(recalboxFiles.savesDir+system.name):
    		os.makedirs(recalboxFiles.savesDir+system.name)

        command = generators[system.config['emulator']].generate(system, args.rom, playersControllers)
        print(command.array)
        exitcode = runner.runCommand(command)
        time.sleep(1)
        exit(exitcode)
