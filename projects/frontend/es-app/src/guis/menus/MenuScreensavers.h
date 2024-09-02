//
// Created by bkg2k on 31/10/2020.
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <guis/menus/base/ISliderChanged.h>
#include <guis/menus/base/ISelectorChanged.h>

class MenuScreensavers : public Menu
                          , private ISliderChanged
                          , private ISingleSelectorChanged<RecalboxConf::Screensaver>
                          , private IMultiSelectorChanged<String>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuScreensavers(WindowManager& window, SystemManager& systemManager);

  private:
    enum class Components
    {
        Time,
        Type,
        SystemList,
    };

    //! System Manager reference
    SystemManager& mSystemManager;

    //! Get Screensaver type List
    SelectorEntry<RecalboxConf::Screensaver>::List GetTypeEntries();

    //! Get System List
    SelectorEntry<String>::List GetSystemEntries();

    /*
     * ISliderComponent implementation
     */

    void MenuSliderMoved(int id, float value) override;

    /*
     * IOptionListComponent<String> implementation
     */

    void MenuSingleChanged(int id, int index, const RecalboxConf::Screensaver& value) override;

    /*
     * IOptionListMultiComponent<String> implementation
     */

    void MenuMultiChanged(int id, int index, const String::List& value) override;
};

