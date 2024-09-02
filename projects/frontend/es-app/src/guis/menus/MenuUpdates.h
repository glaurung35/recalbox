//
// Created by bkg2k on 08/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>

// Forward declaration
class SystemManager;

class MenuUpdates : public Menu
                  , private ISwitchChanged
                  , private ISubMenuSelected
                  , private ISingleSelectorChanged<RecalboxConf::UpdateType>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuUpdates(WindowManager& window);

  private:
    enum class Components
    {
      Enable,
      Version,
      Changelog,
      StartUpdate,
      UpdateType,
    };

    //! Get Update type List
    static SelectorEntry<RecalboxConf::UpdateType>::List GetUpdateTypeEntries();

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;

    /*
     * IOptionListComponent<String> implementation
     */

    void MenuSingleChanged(int id, int index, const RecalboxConf::UpdateType& value) override;
};



