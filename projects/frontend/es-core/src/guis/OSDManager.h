//
// Created by bkg2k on 26/09/23.
//
#pragma once

#include "Gui.h"
#include <components/TextComponent.h>
#include <input/InputMapper.h>
#include <input/Input.h>

class OSDManager : public Gui
{
  public:
    //! Constructor
    explicit OSDManager(WindowManager& window);

    /*
     * Pads
     */

    /*!
     * @brief Check & Refresh battery statuses
     */
    void UpdatePadIcon();

    /*!
     * @brief Force OSD activation whatever user configuration says
     * @param forced True to force activation
     */
    void ForcedPadOSDActivation(bool forced) { mForcedActive = forced; }

    /*
     * FPS
     */

    /*!
     * @brief Record start of frame
     */
    void RecordStartFrame();

    /*!
     * @brief Record end of frame
     */
    void RecordStopFrame();

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
    //! Number of recorded frame timing (in pow of 2)s
    static constexpr int sMaxFrameTimingPow2 = 6;
    //! Number of recorded frame timings (mask)
    static constexpr int sMaxFrameTimingMask = (1 << sMaxFrameTimingPow2) - 1;
    //! Number of recorded frame timings
    static constexpr int sMaxFrameTiming = 1 << sMaxFrameTimingPow2;
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

    //! Pad Font
    std::shared_ptr<Font> mFont;
    //! FPS Font
    std::shared_ptr<Font> mFPSFont;
    //! FPS rectangle
    Rectangle mFPSArea;
    //! Pad char glyph
    Font::Glyph mPadGlyph;
    //! Frame timings start
    int mFrameStart[sMaxFrameTiming];
    //! Frame timings (computations)
    int mFrameTimingComputations[sMaxFrameTiming];
    //! Frame timings (computations)
    int mFrameTimingTotal[sMaxFrameTiming];
    //! Component Alpha
    int mAlpha[Input::sMaxInputDevices];
    //! Pad unicode char
    String::Unicode mPadChar;
    //! Timing current index
    int mTimingIndex;
    //! Recorded timings
    int mRecordedTimings;
    //! Pad count
    int mPadCount;
    //! Active?
    bool mActive;
    //! Forced Active?
    bool mForcedActive;

    /*!
     * @brief Calculate FPS using recorded operations
     * @return FPS average calculated on last sMaxFrameTiming frames
     */
    float CalculateFPS();

    /*!
     * @brief Calculate Frame percentage using recorded operations
     * @return Frame percentage average calculated on last sMaxFrameTiming frames
     */
    float CalculateFramePercentage();

    /*!
     * @brief Render FPS and frame percentage
     */
    void RenderFPS();

    /*!
     * @brief Render pad & battery icons
     */
    void RenderPads();
};
