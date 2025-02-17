#!/bin/bash

mount -o remount,rw /boot

if [ ! -f /boot/recalbox-user-config.txt ]; then
  cat >/boot/recalbox-user-config.txt <<EOF
# Change to your needs

# uncomment if you get no picture on HDMI for a default "safe" mode
#hdmi_safe=1

disable_overscan=1

# uncomment to force a specific HDMI mode (this will force VGA)
#hdmi_group=1
#hdmi_mode=1

# Sound output. Set to 0 or comment for autodetect, 1 for DVI, 2 to force HDMI.
#hdmi_drive=2

config_hdmi_boost=0

# uncomment for composite PAL
#sdtv_mode=2

# uncomment for lirc-rpi
#dtoverlay=lirc-rpi

# uncomment if you have chinese TV display and display is garbled or slow
#hdmi_ignore_edid=0xa5000080
EOF
fi

# force hdmi audio output
# ES should select best port on start
sed -i \
    -e '/^\(audio.device=\).*/{s//\1alsa_card.1:hdmi-output-0/;:a;n;ba;q}' \
    -e '$aaudio.device=alsa_card.1:hdmi-output-0' /recalbox/share/system/recalbox.conf

# Force VSync on PPSSPP
# If file is not here, we let the user take care of it
PPSSPPCONFIGILE="/recalbox/share/system/configs/ppsspp/PSP/SYSTEM/ppsspp.ini"
if [ -f "${PPSSPPCONFIGILE}" ]; then
  sed -i \
    -e 's/^VSyncInterval\s*=.*/VSyncInterval = True/' \
    "${PPSSPPCONFIGILE}"
fi

# Remove old dats files from bios
rm -rf /recalbox/share/bios/fba_029671*.dat
rm -rf /recalbox/share/bios/fba/*.dat
rm -rf /recalbox/share/bios/FinalBurn*.dat
rm -rf /recalbox/share/bios/dc/*.dat.zip
rm -rf /recalbox/share/bios/dc/*barhi.zip
rm -rf /recalbox/share/bios/fbneo/FinalBurn*.dat
rm -rf /recalbox/share/bios/fbneo/light
rm -rf /recalbox/share/bios/mame/*.dat.zip
rm -rf /recalbox/share/bios/mame/*rhi.zip
rm -rf /recalbox/share/bios/mame2000/MAME\ 0.37b5\ XML.dat
rm -rf /recalbox/share/bios/mame2003/mame2003.xml
rm -rf /recalbox/share/bios/mame2003-plus/mame2003-plus.xml
rm -rf /recalbox/share/bios/mame2010/mame2010.xml
rm -rf /recalbox/share/bios/mame2015/mame2014-xml.zip
rm -rf /recalbox/share/bios/mame2015/mame2015-xml.zip

# Remove all .nvmem and .nvmem2 files from saves
rm -rf /recalbox/share/saves/atomiswave/reicast/*.nvmem
rm -rf /recalbox/share/saves/atomiswave/reicast/*.nvmem2

# Remove all 240ptestsuite roms from share, now in share_init since 8.0.1
rm -rf /recalbox/share/roms/240ptestsuite/media
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-Dreamcast-1.25.cdi
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-GB-0.22.gb
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-GBA-0.22.gba
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-MD_Genesis-1.22.bin
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-NES-0.22.nes
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-PCE_TG16-HuCard-1.10.pce
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-SNES-1.061-NTSC.sfc
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-SNES-1.061-PAL.sfc
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-Sega_Mega_CD-1.22-EU.cue
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-Sega_Mega_CD-1.22-EU.iso
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-Sega_Mega_CD-1.22-JP.cue
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-Sega_Mega_CD-1.22-JP.iso
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-Sega_Mega_CD-1.22-US.cue
rm -f /recalbox/share/roms/240ptestsuite/240pTestSuite-Sega_Mega_CD-1.22-US.iso
rm -f /recalbox/share/roms/240ptestsuite/240p_SegaCD_Track-02.wav
rm -f /recalbox/share/roms/240ptestsuite/240p_SegaCD_Track-03.wav
rm -f /recalbox/share/roms/240ptestsuite/LICENCE.md
rm -f /recalbox/share/roms/240ptestsuite/gamelist.xml
# Remove shaders files
rm -f /recalbox/share/shaders/crt-pi.glslp

## Move cfg config for cdi-2015
mkdir -p /recalbox/share/system/configs/cdi/cdi2015
mv /recalbox/share/saves/cdi/cdi2015/cfg /recalbox/share/system/configs/cdi2015/
