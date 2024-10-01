//
// Created by bkg2k on 28/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <views/gamelist/ISimpleGameListView.h>
#include "guis/GuiScraperSingleGameRun.h"

// Forward declaration
class SystemManager;
class SystemData;

class MenuGamelistGameOptions : public Menu
                              , private ISingleSelectorChanged<String>
                              , private ISingleSelectorChanged<Path>
                              , private ISingleSelectorChanged<GameGenres>
                              , private ISwitchChanged
                              , private IEditableChanged
                              , private IRatingChanged
                              , private IActionTriggered
                              , private GuiScraperSingleGameRun::IScrapingComplete
{
  public:
    MenuGamelistGameOptions(WindowManager& window, ISimpleGameListView& view, SystemManager& systemManager, SystemData& system, FileData& game);

    //! Build menu items
    void BuildMenuItems() final;

    ~MenuGamelistGameOptions() override;
  private:
    enum class Components
    {
      Emulator,
      Patch,
      Ratio,
      Favorite,
      Hidden,
      Adult,
      Name,
      Description,
      Rating,
      Genre,
      Scrape,
      Rotation,
    };

    //! View reference
    ISimpleGameListView& mView;
    //! System Manager reference
    SystemManager& mSystemManager;
    //! System reference
    SystemData& mSystem;
    //! Game reference
    FileData& mGame;

    //! Default emulator
    String mDefaultEmulator;
    //! Default core
    String mDefaultCore;

    //! Emulator/Core
    ItemSelector<String>* mEmulator;
    //! sotpatching path
    ItemSelector<Path>* mPath;
    //! Ratio
    ItemSelector<String>* mRatio;
    //! Name
    ItemEditable* mName;
    //! Rating
    ItemRating* mRating;
    //! Genre
    ItemSelector<GameGenres>* mGenre;
    //! Description
    ItemEditable* mDescription;
    //! Favorite
    ItemSwitch* mFavorite;
    //! Hidden
    ItemSwitch* mHidden;
    //! Adult
    ItemSwitch* mAdult;
    //! Rotation
    ItemSwitch* mRotation;

    //! Emulator/Core list
    ItemSelector<String>* mEmulators;

    //! Get emulator list
    SelectorEntry<String>::List GetEmulatorEntries(String& emulatorAndCore);
    //! Get available patch List
    SelectorEntry<Path>::List GetPatchEntries();
    //! Get ratio list
    SelectorEntry<String>::List GetRatioEntries();
    //! Get genre list
    SelectorEntry<GameGenres>::List GetGenreEntries();

    String GetFooter(FileData& file);

    /*
     * ISingleSelectorChanged<String> implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;

    void MenuSingleChanged(int id, int index, const Path& value) override;

    /*
     * ISingleSelectorChanged<GameGenres> implementation
     */

    void MenuSingleChanged(int id, int index, const GameGenres& value) override;

    /*
     * ISwitchChanged implementation
     */

    void MenuEditableChanged(int id, const String& text) override;

    /*
     * IEditableChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;

    /*
     * IRatingChanged implementation
     */

    void MenuRatingMoved(int id, float value) override;

    /*
     * ISubMenuSelected implementation
     */

    void MenuActionTriggered(int id) override;

    /*
     * GuiScraperSingleGameRun::IScrapingComplete implementation
     */

    void ScrapingComplete(FileData& game, MetadataType changedMetadata) override;
};



