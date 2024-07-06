//
// Created by bkg2k on 23/07/24.
//
#pragma once

enum class SupportTypes
{
    Unknown,   //!< Unknown support
    Cartridge, //!< Cartridge
    Tape,      //!< Tape
    QuickDisk, //!< Quick disk 2.8"
    Disk35,    //!< 3.5" Disc
    Disk525,   //!< 5.25" Disc
    CD,        //!< CD (or DVD)
    HardDrive, //!< Hard drive image
};