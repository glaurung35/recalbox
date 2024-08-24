//
// Created by bkg2k on 12/07/23.
//
#pragma once

#include "IFastMenuListCallback.h"
#include <guis/menus/base/Menu.h>
#include <guis/menus/base/ItemAction.h>

class FastMenuList : public Menu
                      , public IActionTriggered
{
  public:
    //! Simple entry structure
    struct EntryParam
    {
      String Name; //!< Display text
      String Help; //!< Auto help message
      EntryParam(const String& name, const String& help) : Name(name), Help(help) {}
      EntryParam(const String& name) : Name(name), Help(name) {}
    };

    //! Parameter list
    typedef std::vector<EntryParam> EntryParamList;

    /*!
     * @brief Constructor
     * @param window Window manager
     * @param callbackInterface Callback interface
     * @param title Menu title
     * @param footer Menu footer
     * @param menuIndex Global menu index
     * @param params Line params
     * @param defaultIndex Default selected line index
     */
    FastMenuList(WindowManager& window, IFastMenuListCallback* callbackInterface,
                 const String& title, const String& footer, int menuIndex,
                 const EntryParamList& params, int defaultIndex)
       : Menu(window, title, footer)
       , mMenuIndex(menuIndex)
       , mInterface(callbackInterface)
    {
      // Add lines
      int entryIndex = 0;
      for(const EntryParam& param : params)
        AddAction(param.Name, _("SELECT"), entryIndex++, true, this, param.Help);

      // Add cancel button
      ItemAction* cancel = AddAction(_("CANCEL SELECTION"), _("BACK"), -1, false, this);

      // Set default line or button
      if (defaultIndex>= 0) SetSelectedItem(defaultIndex);
      else SetSelectedItem(cancel);
    }

  private:
    //! Menu index
    int mMenuIndex;
    //! Interface
    IFastMenuListCallback* mInterface;

    /*!
     * @brief Line selected, call interface back
     * @param id
     */
    void MenuActionTriggered(int id) override
    {
      if (mInterface != nullptr)
        mInterface->FastMenuLineSelected(mMenuIndex, id);
      Close();
    }
};
