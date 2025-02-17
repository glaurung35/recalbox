//
// Created by bkg2k on 15/03/2022.
//

#include "ResolutionAdapter.h"
#include "Resolutions.h"
#include "CrtConf.h"
#include "utils/String.h"
#include <SDL2/SDL.h>
#include <utils/math/Misc.h>
#include <utils/os/fs/Path.h>
#include <utils/Log.h>
#include <hardware/Board.h>

#ifdef USE_KMSDRM

#include <xf86drmMode.h>
#include <libdrm/drm.h>
#include <fcntl.h>
#include <unistd.h>

const ResolutionAdapter::ResolutionList& ResolutionAdapter::GetResolutionDetailedList(bool filterHighResolutions)
{
  mResolutionsDetailed.clear();

  int dummy = 0;
  bool strict = false;
  GetMaximumResolution(dummy, dummy, strict);

  { LOG(LogDebug) << "[DRM] Enumerating resolution list. High resolution filtering is: " << (filterHighResolutions ? "ON" : "OFF"); }

  Path connectorMainPath("/dev/dri");
  Path::PathList connectorList = connectorMainPath.GetDirectoryContent();

  int connectedMonitors = 0;
  for(const Path& connectorPath : connectorList)
  {
    // Open connector
    int fd = open(connectorPath.ToChars(), O_RDWR);
    if (fd < 0) continue; // Error opening connector

    // Get connector resources
    drmModeResPtr resource = drmModeGetResources(fd);
    if (resource != nullptr)
    {
      { LOG(LogDebug) << "[DRM] Open card '" << connectorPath.ToString() << "' with " << resource->count_connectors << " connectors"; }
      // Run through connectors
      for(int connectorIndex = 0; connectorIndex < resource->count_connectors; ++connectorIndex)
      {
       // Get connectors
        drmModeConnectorPtr connector = drmModeGetConnector(fd, resource->connectors[connectorIndex]);
        if (connector != nullptr)
        {
          { LOG(LogDebug) << "[DRM]   Open connector #" << connectorIndex << " - Connected " << (connector->connection == DRM_MODE_CONNECTED ? "Yes" : "No"); }

          /*
           * If filtering is on, check if default (or element zero if no explicit default)
           * has been filtered. This will change the way we're searching for default resolution
           */
          if (connector->connection == DRM_MODE_CONNECTED)
          {
            bool hasDefaultElement = false;
            bool firstElementHasBeenFiltered = false;
            bool defaultElementHasBeenFiltered = false;
            if (filterHighResolutions)
              for (int m = 0; m < connector->count_modes; ++m)
              {
                drmModeModeInfo& mode = connector->modes[m];
                if ((mode.type & DRM_MODE_TYPE_PREFERRED) != 0) hasDefaultElement = true;
                if (FilterHighResolution(mode.hdisplay, mode.vdisplay))
                {
                  if (m == 0) firstElementHasBeenFiltered = true;
                  if ((mode.type & DRM_MODE_TYPE_PREFERRED) != 0) defaultElementHasBeenFiltered = true;
                }
              }
            /*
             * No true default element?
             * if the first element (considered as default) has been filtered then set the defaultElementHasBeenFiltered
             * so that the following code just deal with *one* composite flag
             */
            if (!hasDefaultElement)
              if (firstElementHasBeenFiltered)
                defaultElementHasBeenFiltered = true;

            /*
             * But if the default resolution has not been filtered out (either true or element 0)
             * reset the filter
            */
            if (defaultElementHasBeenFiltered && !strict)
            {
              filterHighResolutions = false;
              { LOG(LogDebug) << "[DRM]     Default item filtered & strict mode is off: reset filter"; }
            }

            // List & record modes
            int defaultModeIndex = -1;
            for (int m = 0; m < connector->count_modes; ++m)
            {
              drmModeModeInfo &mode = connector->modes[m];
              // Out of bounds?
              if (filterHighResolutions)
                if (FilterHighResolution(mode.hdisplay, mode.vdisplay))
                  continue;
              if ((mode.type & DRM_MODE_TYPE_PREFERRED) != 0) defaultModeIndex = (int)mResolutionsDetailed.size();
              mResolutionsDetailed.push_back({connectedMonitors, mode.hdisplay, mode.vdisplay, -1, (int) mode.vrefresh,
                                     (mode.flags & DRM_MODE_FLAG_INTERLACE) != 0,
                                     (mode.type & DRM_MODE_TYPE_PREFERRED) != 0});
              { LOG(LogDebug) << "[DRM]     Mode #" << m << " : " << mResolutionsDetailed.back().ToString(); }
            }

            // Nothing found
            if (defaultModeIndex < 0)
            {
              // Default behavior is to just take the first element
              defaultModeIndex = 0;

              // Store
              Resolution& r = mResolutionsDetailed[defaultModeIndex];
              { LOG(LogDebug) << "[DRM]     Setting non desktop default resolution: " << r.Width << 'x' << r.Height; }
              r.IsDefault = true;
            }

            // Inc connected monitors index
            connectedMonitors++;
          }
          else { LOG(LogDebug) << "[DRM]   Skipping unconnected connector " << connectorIndex; }
          // Free connector
          drmModeFreeConnector(connector);
        }
        else { LOG(LogDebug) << "[DRM]   Error opening connector " << connectorIndex; }
      }
      // Free resources
      drmModeFreeResources(resource);
    }
    else { LOG(LogDebug) << "[DRM] Error opening card '" << connectorPath.ToString() << '\''; }

    close(fd);
  }

  // Horizontal screens?
  switch(Board::Instance().GetBoardType())
  {
    case BoardType::RG351P:
    case BoardType::OdroidAdvanceGo:
    case BoardType::OdroidAdvanceGoSuper:
    {
      for(Resolution& r : mResolutionsDetailed)
      {
        int tmp = r.Height;
        r.Height = r.Width;
        r.Width = tmp;
      }
      break;
    }
    case BoardType::UndetectedYet:
    case BoardType::Unknown:
    case BoardType::Pi0:
    case BoardType::Pi02:
    case BoardType::Pi1:
    case BoardType::Pi2:
    case BoardType::Pi3:
    case BoardType::Pi3plus:
    case BoardType::Pi4:
    case BoardType::Pi400:
    case BoardType::Pi5:
    case BoardType::UnknownPi:
    case BoardType::PCx86:
    case BoardType::PCx64:
    case BoardType::RG351V:
    case BoardType::RG353P:
    case BoardType::RG353V:
    case BoardType::RG353M:
    case BoardType::RG503:
    default: break;
  }

  if (mResolutionsDetailed.empty() && filterHighResolutions)
  {
    { LOG(LogDebug) << "[DRM] No resolution available with filtering on. Trying w/o filter."; }
    return GetResolutionDetailedList(false);
  }

  return mResolutionsDetailed;
}

#else // SDL2

const ResolutionAdapter::ResolutionList& ResolutionAdapter::GetResolutionDetailedList(bool filterHighResolution)
{
  { LOG(LogDebug) << "[ResolutionAdapter] Getting resolution from sdl"; }

  if (mResolutionsDetailed.empty())
  {
    SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, nullptr };
    int displayCount = SDL_GetNumVideoDisplays();
    for (int i = 0; i < displayCount; ++i)
    {
      SDL_DisplayMode defaultMode = mode;
      SDL_GetDesktopDisplayMode(i, &defaultMode);
      int modesCount = SDL_GetNumDisplayModes(i);
      for (int m = 0; m < modesCount; ++m)
        if (SDL_GetDisplayMode(i, m, &mode) == 0)
        {
          if (FilterHighResolution(mode.w, mode.h)) continue;
          mResolutionsDetailed.push_back({i, mode.w, mode.h, (int)SDL_BITSPERPIXEL(mode.format), mode.refresh_rate, false, mode.w == defaultMode.w && mode.h == defaultMode.h && mode.format == defaultMode.format && mode.refresh_rate == defaultMode.refresh_rate });
          { LOG(LogDebug) << "[ResolutionAdapter]   Mode #" << m << " : " << mResolutionsDetailed.back().ToString(); }
        }
    }
  }

  if (mResolutionsDetailed.empty() && filterHighResolution)
    return GetResolutionDetailedList(false);

  return mResolutionsDetailed;
}

#endif // USE_KMSDRM

bool ResolutionAdapter::AdjustResolution(int display, const String& value, Resolutions::SimpleResolution& output, bool filtered)
{
  { LOG(LogDebug) << "[ResolutionAdapter] Adjusting " << value; }
  // Process default
  if (value == "default")
  {
    Resolution res = GetDefaultResolution(filtered);
    { LOG(LogDebug) << "[ResolutionAdapter] Default resolution asked, adjusting with current default resolution: " << res.Width << 'x' << res.Height; }
    output.Height = res.Height;
    output.Width = res.Width;
    return true;
  }

  // Convert resolution
  Resolutions::SimpleResolution sr = Resolutions::ConvertSimpleResolution(value);
  if (!sr.IsValid())
  {
    { LOG(LogDebug) << "[ResolutionAdapter] " << value << " is invalid!"; }
    return false;
  }

  const ResolutionList& list = GetResolutionDetailedList(filtered);

  // Check if the resolution match an existing resolution
  for(const Resolution& r : list)
    if (r.Display == display &&
        r.Width == sr.Width &&
        r.Height == sr.Height)
    {
      output = { sr.Width, sr.Height };
      { LOG(LogDebug) << "[ResolutionAdapter] " << value << " already match an existing resolution"; }
      return true; // No need to adjust
    }

  // Check best higher height
  const Resolution* best = nullptr;
  for(const Resolution& r : list)
    if (r.Display == display && sr.Width == r.Width)
      if ((best == nullptr && r.Height > sr.Height) || (r.Height > sr.Height && r.Height < best->Height))
        best = &r;
  if (best != nullptr)
  {
    output = { best->Width, best->Height };
    { LOG(LogDebug) << "[ResolutionAdapter] Adjusted to " << output.Width << 'x' << output.Height; }
    return true;
  }

  // Check best lower height
  best = nullptr;
  for(const Resolution& r : list)
    if (r.Display == display && sr.Width == r.Width)
      if ((best == nullptr && r.Height < sr.Height) || (r.Height < sr.Height && r.Height > best->Height))
        best = &r;
  if (best != nullptr)
  {
    output = { best->Width, best->Height };
    { LOG(LogDebug) << "[ResolutionAdapter] Adjusted to " << output.Width << 'x' << output.Height; }
    return true;
  }

  // Get nearest resolution
  best = nullptr;
  int area = SDL_MAX_SINT32;
  for(const Resolution& r : list)
  {
    int dw = Math::absi(r.Width - sr.Width);
    int dh = Math::absi(r.Height - sr.Height);
    int delta = dw * dw + dh * dh;
    if (r.Display == display && delta < area)
    {
      area = delta;
      best = &r;
    }
  }
  if (best != nullptr)
  {
    output = { best->Width, best->Height };
    { LOG(LogDebug) << "[ResolutionAdapter] Adjusted to " << output.Width << 'x' << output.Height; }
    return true;
  }

  // Something goes wrong...
  { LOG(LogDebug) << "[ResolutionAdapter] Not adjusting " << value << " because something goes wrong!"; }
  return false;
}

ResolutionAdapter::Resolution ResolutionAdapter::GetDefaultResolution(bool filtered)
{
  ResolutionList allResolutions = GetResolutionDetailedList(filtered);
  for(const Resolution& rawResolution : allResolutions)
    if (rawResolution.IsDefault)
      return rawResolution;
  return Resolution(0, -1, -1, 32, 50, false, true);
}

const ResolutionAdapter::ResolutionList& ResolutionAdapter::GetResolutionList(bool filterHighResolutions)
{
  if(mResolutions.empty())
  {
    ResolutionList allResolutions = GetResolutionDetailedList(filterHighResolutions);
    for(const Resolution& rawResolution : allResolutions)
    {
      bool alreadyPresent = false;
      // if mResolutions.contains(rawResolution)
      for(Resolution& filteredResolution : mResolutions)
      {
        if(filteredResolution.Width == rawResolution.Width && filteredResolution.Height == rawResolution.Height)
        {
          // The resolution already exists, we should update the frequency value if it's higher
          if(filteredResolution.Frequency < rawResolution.Frequency)
          {
            filteredResolution.Frequency = rawResolution.Frequency;
            if (filteredResolution.Bpp < rawResolution.Bpp)
              filteredResolution.Bpp = rawResolution.Bpp;
            if (filteredResolution.Interlaced && !rawResolution.Interlaced)
              filteredResolution.Interlaced = false;
          }
          alreadyPresent = true;
          break;
        }
      }
      if(!alreadyPresent)
        mResolutions.push_back(rawResolution);
    }
  }
  return mResolutions;
}

void ResolutionAdapter::GetMaximumResolution(int& w, int& h, bool& strict)
{
  switch(Board::Instance().GetBoardType())
  {
    case BoardType::Pi0:
    case BoardType::Pi02:
    case BoardType::Pi1:
    case BoardType::Pi2:
    case BoardType::Pi3:
    case BoardType::Pi3plus: w = 0; h = 800; strict = false; break;
    case BoardType::Pi4:
    case BoardType::Pi400:
    case BoardType::Pi5: w = 0; h = 1200; strict = true; break;
    case BoardType::RG351V:
    case BoardType::RG351P:
    case BoardType::OdroidAdvanceGo:
    case BoardType::OdroidAdvanceGoSuper:
    case BoardType::UnknownPi:
    case BoardType::PCx86:
    case BoardType::PCx64:
    case BoardType::RG353P:
    case BoardType::RG353V:
    case BoardType::RG353M:
    case BoardType::RG503:
    case BoardType::UndetectedYet:
    case BoardType::Unknown:
    default: w = h = 0; strict =false; break;
  }
}

bool ResolutionAdapter::FilterHighResolution(int w, int h)
{
  int maxw = 0, maxh = 0;
  bool strict = false;
  GetMaximumResolution(maxw, maxh, strict);

  if (maxh != 0 && h > maxh)
  {
    { LOG(LogDebug) << "[DRM]     Skip resolution: " << w << 'x' << h << ". Height higher than " << maxh; }
    return true;
  }
  if (maxw != 0 && w > maxw)
  {
    { LOG(LogDebug) << "[DRM]     Skip resolution: " << w << 'x' << h << ". Width higher than " << maxw; }
    return true;
  }
  return false;
}

bool ResolutionAdapter::GetCRTResolution(int& w, int& h)
{
  ICrtInterface& crt = Board::Instance().CrtBoard();
  if (!crt.IsCrtAdapterAttached()) return false;

  // Es will choose its own resolution. The desktop mode cannot be trusted.
  if (crt.GetHorizontalFrequency() == ICrtInterface::HorizontalFrequency::KHz31)
  {
    w = 1920; h = 240;
    if (CrtConf::Instance().GetSystemCRT31kHzResolution() == "480")
    {
      w = 640; h = 480;
    }
  }
  else if (crt.GetHorizontalFrequency() == ICrtInterface::HorizontalFrequency::KHzMulti)
  {
    w = 1920; h = 240;
    if (CrtConf::Instance().GetSystemCRTResolution() == "480")
    {
      w = 640; h = 480;
    }
  }
  else if (CrtConf::Instance().GetSystemCRTResolution() == "480" && crt.HasInterlacedSupport())
  {
    w = 640; h = 480;
    if (Board::Instance().CrtBoard().MustForce50Hz())
    {
      w = 768; h = 576;
    }
  }
  else
  {
    w = 1920; h = 240;
    if (Board::Instance().CrtBoard().MustForce50Hz())
    {
      w = 1920; h = 288;
    }
  }

  return true;
}

