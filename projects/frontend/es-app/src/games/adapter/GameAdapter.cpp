//
// Created by bkg2k on 22/06/22.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include "GameAdapter.h"
#include "GameNameMapManager.h"
#include <systems/SystemData.h>

const String GameAdapter::sEasyRPGSystemName(LEGACY_STRING("easyrpg"));
const String GameAdapter::sEasyRPGGameNameUpper(LEGACY_STRING("RPG_RT.INI"));
const String GameAdapter::sEasyRPGGameNameLower(LEGACY_STRING("RPG_RT.ini"));

std::string GameAdapter::ScrapingName() const
{
  if (mGame.System().Name() == (std::string)sEasyRPGSystemName) // TODO: move to String & remove cast
  {
    if (mGame.RomPath().Filename() == sEasyRPGGameNameUpper ||
        mGame.RomPath().Filename() == sEasyRPGGameNameLower)
    {
      IniFile ini(mGame.RomPath(), false);
      std::string gameName = Strings::RemoveParenthesis(ini.AsString("GameTitle"));
      if (!gameName.empty()) return gameName;
    }
  }

  return mGame.RomPath().Filename();
}

std::string GameAdapter::DisplayName() const
{
  if (!mGame.Name().empty()) return mGame.Name();

  return RawDisplayName(mGame.System(), mGame.RomPath());
}

std::string GameAdapter::RawDisplayName(SystemData& system, const Path& rompath)
{
  if (system.Name() == (std::string)sEasyRPGSystemName) // TODO: move to String & remove cast
  {
    if (rompath.Filename() == sEasyRPGGameNameUpper ||
        rompath.Filename() == sEasyRPGGameNameLower)
    {
      IniFile ini(rompath, false);
      std::string gameName = Strings::RemoveParenthesis(ini.AsString("GameTitle"));
      if (!gameName.empty()) return gameName;
    }
  }
  else if (GameNameMapManager::HasRenaming(system))
  {
    std::string gameName(GameNameMapManager::Rename(system, rompath.FilenameWithoutExtension()));
    if (!gameName.empty()) return gameName;
  }

  return rompath.FilenameWithoutExtension();
}

long long GameAdapter::RomSize()
{
  Path romPath(mGame.RomPath());
  return romPath.IsFile() ? romPath.Size() : 0L;
}
