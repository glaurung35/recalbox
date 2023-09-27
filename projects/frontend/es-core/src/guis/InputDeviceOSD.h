//
// Created by bkg2k on 26/09/23.
//
#pragma once

#include "Gui.h"
#include <components/TextComponent.h>
#include <input/InputMapper.h>
#include <input/Input.h>

class InputDeviceOSD : public Gui
{
  public:
    //! Constructor
    explicit InputDeviceOSD(WindowManager& window);

    /*!
     * @brief Check & Refresh battery statuses
     */
    void UpdatePadIcon();

    /*!
     * @brief Force OSD activation whatever user configuration says
     * @param forced True to force activation
     */
    void ForcedActivation(bool forced) { mForcedActive = forced; }

    /*
     * Component override
     */

    //! Process input
    bool ProcessInput(const InputCompactEvent& event) override;

    //! Update fadings
    void Update(int deltaTime) override;

    //! Draw
    void Render(const Transform4x4f& parentTrans) override;

  private:
    //! Pad unicode character
    static constexpr String::Unicode sPadIcon = 0xf2f0;
    //! Pad color
    static constexpr unsigned int sColor = 0xFFFFFF00;
    //! Maximum Alpha when a pad is "activated"
    static constexpr int sMaxAlpha = 255;
    //! Minimum alpha when a pad is idle
    static constexpr int sMinAlpha = 64;

    //! Mapper reference
    InputMapper& mMapper;

    //! Font
    std::shared_ptr<Font> mFont;
    //! Pad char glyph
    Font::Glyph mPadGlyph;
    //! Component Alpha
    int mAlpha[Input::sMaxInputDevices];
    //! Pad unicode char
    String::Unicode mPadChar;
    //! Pad count
    int mPadCount;
    //! Last time battery icons have been refreshed
    int mLastTimeBatteryRefreshed;
    //! Active?
    bool mActive;
    //! Forced Active?
    bool mForcedActive;
};
