//
// Created by bkg2k on 22/06/22.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include "games/FileData.h"

/*!
 * @brief Adapt some methods from FileData to handle special cases
 * as for EasyRPG
 */
class GameAdapter
{
  public:
    explicit GameAdapter(const FileData& filedata)
      : mGame(filedata)
    {
    }

    //! Direct accessor
    [[nodiscard]] const class FileData& FileData() const { return mGame; }

    //! Get scraping name
    [[nodiscard]] std::string ScrapingName() const;

    //! Get displayable name if the game has no name
    [[nodiscard]] std::string DisplayName() const;

    //! Get rom size
    long long RomSize();

    /*!
     * @brief Get the raw display filename if a rom from the given system
     * @param system System the rom belongs to
     * @param rompath Rom path
     * @return Name or empty string
     */
    static std::string RawDisplayName(SystemData& system, const Path& rompath) ;

  private:
    //! Easy RPG system name
    static const String sEasyRPGSystemName;
    //! Easy RPG special name uppercase
    static const String sEasyRPGGameNameUpper;
    //! Easy RPG special name lowercase
    static const String sEasyRPGGameNameLower;

    //! Underlying FileData structure
    const class FileData& mGame;
};
