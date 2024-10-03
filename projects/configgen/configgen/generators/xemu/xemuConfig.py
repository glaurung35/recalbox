#!/usr/bin/env python
from configgen.generators.Generator import Generator
from configgen.Command import Command
from configgen.settings.keyValueSettings import keyValueSettings
from configgen.Emulator import Emulator
from configgen.controllers.inputItem import InputItem
from configgen.controllers.controller import ControllerPerPlayer, Controller
from configgen.settings.iniSettings import IniSettings
import configgen.recalboxFiles as recalboxFiles
from typing import Dict, List
import os
import configparser

def writeIniFile(system,args):
    iniConfig = configparser.ConfigParser(interpolation=None)
    # To prevent ConfigParser from converting to lower case
    iniConfig.optionxform = str
    if os.path.exists(recalboxFiles.xemuConfig):
        try:
            with io.open(recalboxFiles.xemuConfig, 'r', encoding='utf_8_sig') as fp:
                iniConfig.readfp(fp)
        except:
            pass

    createXemuConfig(iniConfig, system,args)
    # save the ini file
    if not os.path.exists(os.path.dirname(recalboxFiles.xemuConfig)):
        os.makedirs(os.path.dirname(recalboxFiles.xemuConfig))
    with open(recalboxFiles.xemuConfig, 'w') as configfile:
        iniConfig.write(configfile)

def createXemuConfig(iniConfig, system,args):
    # Create INI sections
    if not iniConfig.has_section("general"):
        iniConfig.add_section("general")
    if not iniConfig.has_section("sys"):
        iniConfig.add_section("sys")
    if not iniConfig.has_section("sys.files"):
        iniConfig.add_section("sys.files")
    if not iniConfig.has_section("audio"):
        iniConfig.add_section("audio")
    if not iniConfig.has_section("display.quality"):
        iniConfig.add_section("display.quality")
    if not iniConfig.has_section("display.window"):
        iniConfig.add_section("display.window")
    if not iniConfig.has_section("display.ui"):
        iniConfig.add_section("display.ui")
    if not iniConfig.has_section("input.bindings"):
        iniConfig.add_section("input.bindings")
    if not iniConfig.has_section("net"):
        iniConfig.add_section("net")
    if not iniConfig.has_section("net.udp"):
        iniConfig.add_section("net.udp")
        

    # Boot Animation Skip
    
    iniConfig.set("general", "skip_boot_anim","false")
    
    # Disable welcome screen on first launch
    iniConfig.set("general", "show_welcome", "false")

    # Set Screenshot directory
    iniConfig.set("general", "screenshot_dir", '"/recalbox/share/screenshots"')

    # Fill sys sections
    
    iniConfig.set("sys", "mem_limit", '"64"')
    iniConfig.set("sys.files", "flashrom_path", '"/recalbox/share/bios/xbox/Complex_4627.bin"')
    iniConfig.set("sys.files", "bootrom_path", '"/recalbox/share/bios/xbox/mcpx_1.0.bin"')
    iniConfig.set("sys.files", "hdd_path", '"/recalbox/share/saves/xbox/xbox_hdd.qcow2"')
    iniConfig.set("sys.files", "eeprom_path", '"/recalbox/share/saves/xbox/xemu_eeprom.bin"')
    iniConfig.set("sys.files", "dvd_path", '"' + str(args.rom) + '"')

    # Audio quality
    
    iniConfig.set("audio", "use_dsp", "false")

    # Rendering resolution

    iniConfig.set("display.quality", "surface_scale", "1") #render scale by default

    # start fullscreen
    iniConfig.set("display.window", "fullscreen_on_startup", "true")

   

    # Vsync

    iniConfig.set("display.window", "vsync", "true")

    # don't show the menubar
    iniConfig.set("display.ui", "show_menubar", "false")

    # Scaling
 
    iniConfig.set("display.ui", "fit", '"scale"')

    # Aspect ratio
 
    iniConfig.set("display.ui", "aspect_ratio", '"auto"')
  
    # Network
    # Documentation: https://github.com/xemu-project/xemu/blob/master/config_spec.yml
 
    iniConfig.set("net", "enable", "true")
  
