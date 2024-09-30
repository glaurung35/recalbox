//
// Created by bkg2k on 03/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>

class MenuRetroAchievements : public Menu
                            , private IEditableChanged
                            , private ISwitchChanged
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuRetroAchievements(WindowManager& window);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
        Enabled,
        Hardcore,
        Login,
        Password,
    };

    /*
     * IEditableChanged implementation
     */

    void MenuEditableChanged(int id, const String& text) override;

    /*
     * ISwitchChanged implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;
};



