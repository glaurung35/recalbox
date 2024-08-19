#pragma once

#include <utility>

#include "games/FileData.h"

class AliasData : public FileData
{
  public:


    explicit AliasData(ItemType type, const Path& path, RootFolderData& ancestor)
      : FileData(type, path, ancestor)
    {

    }

     AliasData(SystemData* system, String  name):
    FileData(ItemType::Alias, Path::Root(), system->MasterRoot()),
    mName(std::move(name))
    {}

    AliasData(RootFolderData& root, String  name):
    FileData(ItemType::Alias, Path::Root(), root),
    mName(std::move(name))
    {}

    [[nodiscard]] inline String AliasName() const { return mName; }
    inline FileData::List Games() const { return mGames; }

    inline void SetMainGame(FileData* game) {mMainGame = game; }


  private:
    String mName;
    FileData::List mGames;
    FileData* mMainGame;
};
