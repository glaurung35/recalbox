//
// Created by digi on 11/27/23.
//

#include "RPiEepromUpdater.h"
#include "recalbox/RecalboxSystem.h"


RPiEepromUpdater::RPiEepromUpdater()
  : mError(false), mCurrentVersion(""), mLastversion(""), mUpdateAvailable(false)
{
  auto res = Run(false);
  if (res.second != 0)
  {
    if (res.first.Contains("*** UPDATE AVAILABLE ***"))
    {
      mCurrentVersion = ExtractVersion(res.first, "CURRENT: ");
      mLastversion = ExtractVersion(res.first, "LATEST: ");
      mUpdateAvailable = true;
    }
    else
    {
      mError = true;
    }
  }
  else
  {
    mCurrentVersion = ExtractVersion(res.first, "CURRENT: ");
    mLastversion = ExtractVersion(res.first, "LATEST: ");
  }
}

bool RPiEepromUpdater::Update() const
{
  return Run(true).second == 0;
}

std::pair<String, int> RPiEepromUpdater::Run(bool autoupdate) const
{
  return RecalboxSystem::execute(String("rpi-eeprom-update").Append(autoupdate ? " -a" : ""));
}

String RPiEepromUpdater::ExtractVersion(String cmdResult, String updateType)
{
  if (cmdResult.Contains(updateType))
  {
    int currentIndex = cmdResult.Find(updateType) + (int) updateType.length();
    int currentLineEnd = cmdResult.Find('\n', currentIndex);
    return String(cmdResult, currentIndex, currentLineEnd - currentIndex);
  }
  return "";
}
