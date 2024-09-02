//
// Created by bkg2k on 30/10/2020.
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <audio/AudioMode.h>
#include <guis/GuiWaitLongExecution.h>
#include <audio/IAudioNotification.h>
#include "guis/menus/base/ISliderChanged.h"
#include "guis/menus/base/ISubMenuSelected.h"
#include "guis/menus/base/ISelectorChanged.h"

class MenuSound : public Menu
                   , public ILongExecution<bool, String::List>
                   , public ISubMenuSelected
                   , public IAudioNotification
                   , private ISliderChanged
                   , private ISingleSelectorChanged<String>
                   , private ISingleSelectorChanged<AudioMode>
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuSound(WindowManager& window);

    ~MenuSound();

    /*!
     * @brief Called once per frame. Override to implement your own drawings.
     * Call your base::Update() to ensure animation and childrens are updated properly
     * @param deltaTime Elapsed time from the previous frame, in millisecond
     */
    void Update(int deltaTime) override;

    /*!
     * @brief Refresh audio output menu list
     */
    void Refresh();

  private:
    enum class Components
    {
      Volume,
      AudioMode,
      Output,
      Pair,
      MusicVolume,
    };

    //! Volume slider
    ItemSlider* mVolume;
    //! Music Volume slider
    ItemSlider* mMusicVolume;
    //! Output device index
    ItemSelector<String>* mOutputDevice;

    //! Get Output List
    static SelectorEntry<String>::List GetOutputEntries();

    static SelectorEntry<AudioMode>::List GetAudioModeEntries();

    // Start scaning bluetooth devices
    void StartScanningDevices();

    /*!
     * @brief Scan bluetooth devices in background
     * @return
     */
    String::List Execute(GuiWaitLongExecution<bool, String::List>&, const bool&) override;

    /*!
     * @brief Called when the device scan is complete
     * @param parameter Useless "in" parameter
     * @param result List of available devices
     */
    void Completed(const bool& parameter, const String::List& result) override;

    /*!
     * @brief Called from PulseAudioController whenever a sink is added or removed
     */
    void NotifyAudioChange() final;

    /*
     * ISingleSelectorChanged<String>
     */

    void MenuSingleChanged(int id, int index, const String& value) override;

    /*
     * ISingleSelectorChanged<AudioMode>
     */

    void MenuSingleChanged(int id, int index, const AudioMode& value) override;

    /*
     * ISliderComponent implementation
     */

    void MenuSliderMoved(int id, float value) override;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;
};
