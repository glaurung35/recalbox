//
// Created by digitalLumberjack on 30/03/2023.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <hardware/crt/ICrtInterface.h>
#include <utils/Files.h>

class CrtRGBJamma : public ICrtInterface
{
  public:
    //! Constructor
    explicit CrtRGBJamma(bool automaticallyDetected, BoardType boardType) : ICrtInterface(automaticallyDetected, boardType) {}

    //! A RGB Jamma is attached
    bool IsCrtAdapterAttached() const override {
      #ifdef DEBUG
      return true;
      #else
      return Files::LoadFile(Path(vgaCardConnectedPi4)) == "connected\n"
                                                        || Files::LoadFile(Path(vgaCardConnectedPi3)) == "connected\n" ;
      #endif
    }

    //! This adapter is an RGB JAMMA
    CrtAdapterType GetCrtAdapter() const override { return CrtAdapterType::RGBJamma; }

    //! RGB Jamma has support for 31khz
    bool Has31KhzSupport() const override { return true; }

    //! Return select output frequency
    HorizontalFrequency GetHorizontalFrequency() const override {
      HorizontalFrequency configFreq = static_cast<HorizontalFrequency>(CrtConf::Instance().GetSystemCRTScreenType());
      if(configFreq == ICrtInterface::HorizontalFrequency::Auto) return ICrtInterface::HorizontalFrequency::KHz15;
      return configFreq;
    }

    //! This adapter has no support of forced 50hz
    bool HasForced50hzSupport() const override { return false; }

    //! Get 50hz switch state
    bool MustForce50Hz() const override { return false; }

    std::string& Name() const override { static std::string adapterString("Recalbox RGB JAMMA"); return adapterString; }

    std::string& ShortName() const override { static std::string adapterShortString("recalboxrgbjamma"); return adapterShortString; }

    bool HasMultiSyncSupport() const override { return true; }

  private:
    static constexpr const char* vgaCardConnectedPi4 = "/sys/class/drm/card1-VGA-1/status";
    static constexpr const char* vgaCardConnectedPi3 = "/sys/class/drm/card0-VGA-1/status";
};



