//
// Created by bkg2k on 20/10/22.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

enum CrtResolution: uint
{
  rNone, //!< Null element
  r224p, //!< PAL + NTSC 224p
  r240p, //!< PAL + NTSC 224p
  r288p, //!< PAL only
  r480i, //!< NTSC interlaced hires
  r576i, //!< PAL interlaced hires
  r480p, //!< 31kHz
  r240p120Hz, //!< 31kHz 240
  r320x240p, //!< 320x240
  r384x288p, //!< 384x288
  r1920x480p, //!< 1920x480 (31kHz superrez)
  _rCount, //!< Counter
};

enum Offset: uint
{
  Left,
  Right,
  Up,
  Down,
  Count
};

static int porch[CrtResolution::_rCount][Offset::Count]
{
  // { hback / ratio (max 20), hfront_porch / ratio, Up , Down }
  {0,0,0,0}, // rNone, //!< Null element
  {-20,12,-15,10}, // r224p, //!< PAL + NTSC 224p
  {-20,12,-16,1}, // r240p, //!< PAL + NTSC 224p
  {-20,12,-16,4}, // r288p, //!< PAL only
  {-15,4,-16,3}, // r480i, //!< NTSC interlaced hires
  {-12,4,-12,6}, // r576i, //!< PAL interlaced hires
  {-20,12,-10,10}, // r480p, //!< 31kHz
  {-20,8,-14,4}, // r240p120Hz, //!< 31kHz 240
  {-7,1,-14,4}, // r320x240p, //!< 320x240
  {-6,2,-12,3}, // r384x288p, //!< 384x288
  {-20,8,-10,10}, // r1920x480p
};