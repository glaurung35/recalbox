
#pragma once

#include <guis/menus/base/Menu.h>
#include <views/gamelist/ISimpleGameListView.h>

// Forward declaration
class SystemManager;
class SystemData;

class MenuGamelistGameDeleteSelectionOptions : public Menu
                                             , public IActionTriggered
{
  public:
    explicit MenuGamelistGameDeleteSelectionOptions(WindowManager& window, ISimpleGameListView& view, FileData& game);

  private:
    enum class Components
    {
      Path,
      Delete,
      Cancel,
    };

    //! View reference
    ISimpleGameListView& mView;
    //! Game reference
    FileData& mGame;

    ItemSelector<Path>* mGameFiles;
    ItemSelector<Path>* mMedias;
    ItemSelector<Path>* mExtras;
    ItemSelector<Path>* mSaves;

    SelectorEntry<Path>::List GetGameFileEntries();
    SelectorEntry<Path>::List GetMediaEntries();
    SelectorEntry<Path>::List GetExtraEntries();
    SelectorEntry<Path>::List GetSaveEntries();

    void DeleteSelectedFiles();

    String ComputeMessage();

    //! Get footer string from the given game
    String GetFooter(FileData& game);

    /*
     * IActionTriggered implementation
     */

    void MenuActionTriggered(int id);
};



