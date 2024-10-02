//
// Created by bkg2k on 04/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include "ResolutionAdapter.h"

// Forward declaration
class SystemManager;

class GuiMenuPinballSettings : public Menu
                             , private ISingleSelectorChanged<String>
                             , private ISwitchChanged
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit GuiMenuPinballSettings(WindowManager& window);

  private:
    enum class Components
    {
      Cabinet,
      Trail,
      BackglassScreen,
      BackglassResolution,
      Profile,
    };

    SelectorEntry<String>::List GetPinballBackglassResolutionEntries();
    SelectorEntry<String>::List GetPinballBackglassScreenEntries();
    SelectorEntry<String>::List GetPinballProfileEntries();

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) final;

    /*
     * ISingleSelectorChanged implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) final;
};



