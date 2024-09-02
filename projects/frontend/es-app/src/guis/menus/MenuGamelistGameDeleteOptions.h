
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declaration
class SystemManager;
class SystemData;

class MenuGamelistGameDeleteOptions : public Menu
                                       , private ISubMenuSelected
                                       , private IActionTriggered
{
  public:
    explicit MenuGamelistGameDeleteOptions(WindowManager& window, ISimpleGameListView& view, FileData& game);

  private:
    enum class Components
    {
      Delete,
      Advanced,
      DoDelete,
      DoNotDelete,
    };

    //! View reference
    ISimpleGameListView& mView;
    //! Game reference
    FileData& mGame;

    HashSet<String> mGameFiles;
    HashSet<String> mExtraFiles;
    HashSet<String> mMediaFiles;
    HashSet<String> mSaveFiles;

    /*!
     * @brief Build footer according to the game name
     * @param game Game
     * @return Footer string
     */
    String GetFooter(FileData& game);

    //! Build popup message
    String ComputeMessage();

    // Do delete
    void DeleteAllFiles();

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;

    /*
     * IActionTriggered implementation
     */

    void MenuActionTriggered(int id) override;
};



