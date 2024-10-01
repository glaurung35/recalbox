
#pragma once

#include <guis/menus/base/Menu.h>
#include "views/ISoftPatchingNotifier.h"

class MenuSoftpatchingLauncher : public Menu
                               , private ISingleSelectorChanged<Path>
                               , private IActionTriggered
{
  public:
    MenuSoftpatchingLauncher(WindowManager& window,
                             FileData& game,
                             std::vector<Path>&& patches,
                             int lastChoice,
                             ISoftPatchingNotifier* notifier);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
      Patch,
      Original,
      Patched,
    };

    //! Notification interface
    ISoftPatchingNotifier* mNotifier;
    //! Game reference
    FileData& mGame;
    //! Patch list
    std::vector<Path> mPatches;
    //! Path item
    ItemSelector<Path>* mPatchList;
    //! Previous choice
    int mPreviousChoice;

    SelectorEntry<Path>::List GetPatchesEntries();

    //! Build footer string
    static String GetFooter(FileData& game);

    /*
     * ISingleSelectorChanged<String> implementation
     */

    void MenuSingleChanged(int id, int index, const Path& value) final;

    /*
     * IActionTriggered imlplementation
     */

    void MenuActionTriggered(int id) final;
};



