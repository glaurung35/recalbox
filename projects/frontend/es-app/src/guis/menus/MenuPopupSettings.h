//
// Created by bkg2k on 31/10/2020.
//
#pragma once

#include <guis/menus/base/Menu.h>

class MenuPopupSettings : public Menu
                           , private ISliderChanged
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuPopupSettings(WindowManager& window);

    //! Build menu items
    void BuildMenuItems() final;

  private:
    enum class Components
    {
      Help,
      Music,
      Netplay,
    };

    /*
     * ISliderChanged implementation
     */

    void MenuSliderMoved(int id, float value) override;
};

