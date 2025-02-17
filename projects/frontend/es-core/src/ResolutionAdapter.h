//
// Created by bkg2k on 15/03/2022.
//
#pragma once

#include <utils/String.h>
#include <vector>
#include "Resolutions.h"

class ResolutionAdapter
{
  public:
    // Resolution holder
    struct Resolution
    {
      int Display;      //!< Display index
      int Width;        //!< Resolution width
      int Height;       //!< Resolution height
      int Bpp;          //!< Bit per plane (color deepness)
      int Frequency;    //!< Frequency (Hz)
      bool Interlaced;  //!< Interlaced mode
      bool IsDefault;   //!< True if this is the default resolution (Desktop)

      //! Default constructor
      Resolution()
        : Display(0)
        , Width(0)
        , Height(0)
        , Bpp(0)
        , Frequency(0)
        , Interlaced(false)
        , IsDefault(false)
      {}

      Resolution(int display, int width, int height, int bpp, int frequency, bool interlaced, bool def)
        : Display(display)
        , Width(width)
        , Height(height)
        , Bpp(bpp)
        , Frequency(frequency)
        , Interlaced(interlaced)
        , IsDefault(def)
      {}

      [[nodiscard]] String ToRawString() const
      {
        return String(Width)
               .Append('x')
               .Append(Height);
      }

      [[nodiscard]] String ToString() const
      {
        return String("Display ")
               .Append(Display)
               .Append(": ")
               .Append(Width)
               .Append('x')
               .Append(Height)
               .Append(" - ")
               #ifdef USE_KMSDRM
               .Append(Interlaced ? "interlaced " : "progressive ")
               #else
               //.append(Bpp)
               //.append("Bpp ")
               #endif
               .Append(Frequency)
               .Append("Hz Max")
               .Append(IsDefault ? " (Default)" : "");
      }
    };
    typedef std::vector<Resolution> ResolutionList;

    //! Adjust the given resolution to the best matching available resolution
    bool AdjustResolution(int display, const String& value, Resolutions::SimpleResolution& output, bool filtered = true);

    //! Resolution list with bpm and frequency
    const ResolutionList& ResolutionsDetailed(bool filterHighResolutions) { return GetResolutionDetailedList(filterHighResolutions); }

    //! Resolution list simplified: one entry by couple (width, height)
    const ResolutionList& Resolutions(bool filterHighResolutions) { return GetResolutionList(filterHighResolutions); }

    //! Default resolution, replacing the desktop resolution
    Resolution DefaultResolution(bool filtered = true) { return GetDefaultResolution(filtered); }

    /*!
     * @brief Get Crt resolution
     * @param w output width
     * @param h output height
     * @return true if a CRT resolution has been set in w & h
     */
    static bool GetCRTResolution(int& w, int& h);

private:
    ResolutionList mResolutionsDetailed;
    ResolutionList mResolutions;

    /*!
     * @brief Get resolution list under maxWidth & maxHeight
     * use 0 to ignore one or both max values
     * if no resolution is availaiable under the given maximum, return all available resolution
     * @param filterHighResolutions Set to true to filter resolutions known to be slow on the givent board
     * @return Resolution list
     */
    const ResolutionList& GetResolutionDetailedList(bool filterHighResolutions);

    /*!
     * @brief check if the given resolution must be filtered out because it's known to be slow
     * on the current board
     * @param w Width to test
     * @param h Height to test
     * @return True if the given resolution has to be filtered out. False otherwise
     */
    static bool FilterHighResolution(int w, int h);

    //! Get resolution list
    const ResolutionList& GetResolutionList(bool filterHighResolutions);

    //! Get default resolution
    Resolution GetDefaultResolution(bool filtered = true);

    /*!
     * @brief Get maximum resolution regarding the current board
     * @param w Output maximum with
     * @param h Output maximum height
     * @param strict When set to true, the algorithm is not allowed to get resolutions higher, whatever happens
     */
    static void GetMaximumResolution(int& w, int& h, bool& strict);
};



