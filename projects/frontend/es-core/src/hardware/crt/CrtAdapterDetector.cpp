//
// Created by bkg2k on 20/12/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <hardware/crt/CrtAdapterDetector.h>
#include <utils/Files.h>
#include <RecalboxConf.h>
#include <hardware/crt/CrtNull.h>
#include <hardware/crt/CrtVga666.h>
#include <hardware/crt/CrtRGBPi.h>
#include <hardware/crt/CrtPi2Scart.h>
#include <hardware/crt/CrtRGBDual.h>
#include <CrtConf.h>
#include "CrtRGBJamma.h"

CrtAdapterType CrtAdapterDetector::DetectCrtAdapter(bool& automaticallyDetected)
{
  // Reset
  CrtAdapterType result = CrtAdapterType::None;
  automaticallyDetected = false;

  // RGB Dual hat
  #ifndef OPTION_RECALBOX_SIMULATE_RRGBD
  String vendor = Files::LoadFile(Path(sHatVendorFile));
  String product = Files::LoadFile(Path(sHatProductFile));
  if (vendor.rfind(sRecalboxVendorString) == 0 &&
      product.rfind(sRGBDualProductString) == 0)
  {
  #endif
    result = CrtAdapterType::RGBDual;
    automaticallyDetected = true;
  #ifndef OPTION_RECALBOX_SIMULATE_RRGBD
  }
  else
  {
    // User config
    result = CrtConf::Instance().GetSystemCRT();
  }
  #endif

  return result;
}

ICrtInterface* CrtAdapterDetector::CreateCrtBoard()
{
  bool automatic = false;
  switch(DetectCrtAdapter(automatic))
  {
    case CrtAdapterType::Vga666: return new CrtVga666(automatic);
    case CrtAdapterType::RGBPi: return new CrtRGBPi(automatic);
    case CrtAdapterType::Pi2Scart: return new CrtPi2Scart(automatic);
    case CrtAdapterType::RGBDual: return new CrtRGBDual(automatic);
    case CrtAdapterType::RGBJamma: return new CrtRGBJamma(automatic);
    case CrtAdapterType::RGBJammaV2: return new CrtRGBJamma(automatic);
    case CrtAdapterType::None:
    default: break;
  }

  return new CrtNull(automatic);
}
