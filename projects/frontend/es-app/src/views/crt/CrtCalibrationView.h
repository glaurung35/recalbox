//
// Created by bkg2k on 21/11/2019.
//
#include <WindowManager.h>
#include <components/TextComponent.h>
#include <utils/sync/SyncMessageSender.h>
#include <views/crt/CrtResolutions.h>

#pragma once

class CrtCalibrationView : public Gui
              , private ISyncMessageReceiver<void>
{
  public:
    enum CalibrationType {
      kHz15_60plus50Hz,
      kHz15_60Hz,
      kHz15_50Hz,
      kHz15_60Hz_plus_kHz31,
      kHz31_no_120,
      kHz31,
    };

    /*!
     * @brief Constructor
     * @param window Main Window instance
     */
    explicit CrtCalibrationView(WindowManager& window);

    //! Initialize
    void Initialize(CalibrationType calibrationType);

    /*
     * Gui implementation
     */

    [[nodiscard]] bool DoNotDisturb() const override { return true; }

    /*
     * IComponent implementation
     */

    /*!
     * @brief Called once per frame, after Update.
     * @param parentTrans Transformation
     */
    void Render(const Transform4x4f& parentTrans) override;

    //! Process input events
    bool ProcessInput(const InputCompactEvent& event) override;

    //! Help bar
    bool CollectHelpItems(Help& help) override;

  private:
    static constexpr CrtResolution sForced31khz[] =
    {
      CrtResolution::r480p,
      CrtResolution::r1920x480p,
      CrtResolution::r240p120Hz,
      CrtResolution::rNone
    };

    static constexpr CrtResolution sPALOnly[] =
    {
      CrtResolution::r288p,
      CrtResolution::r384x288p,
      CrtResolution::r576i,
      CrtResolution::rNone
    };

    static constexpr CrtResolution sNTSCOnly[] =
    {
      CrtResolution::r240p,
      CrtResolution::r320x240p,
      CrtResolution::r224p,
      CrtResolution::r480i,
      CrtResolution::rNone
    };

    static constexpr CrtResolution sPALNTSC[] =
    {
      CrtResolution::r240p,
      CrtResolution::r320x240p,
      CrtResolution::r224p,
      CrtResolution::r480i,
      CrtResolution::r288p,
      CrtResolution::r384x288p,
      CrtResolution::r576i,
      CrtResolution::rNone
    };

    static constexpr CrtResolution sMultiSync[] =
    {
      CrtResolution::r240p,
      CrtResolution::r224p,
      CrtResolution::r480p,
      CrtResolution::r1920x480p,
      CrtResolution::rNone
    };

    static constexpr CrtResolution s31khzNo120[] =
      {
        CrtResolution::r480p,
        CrtResolution::r1920x480p,
        CrtResolution::rNone
      };

    //! Pattern image
    ImageComponent mPattern;

    //! Inner grid
    ComponentGrid mGrid;
    //! Horizontal offset text
    std::shared_ptr<TextComponent> mHorizontalOffsetText;
    //! Vertical offset text
    std::shared_ptr<TextComponent> mVerticalOffsetText;
    //! Viewport text
    std::shared_ptr<TextComponent> mViewportText;

    //! Synchronous event
    SyncMessageSender<void> mEvent;

    //! Configuration sequence
    const CrtResolution* mSequence;
    //! Sequence index
    int mSequenceIndex;

    //! Original config
    int mOriginalVOffset;
    int mOriginalHOffset;
    int mOriginalViewportWidth;

    //! Original resolution width
    int mOriginalWidth;
    //! Original resolution height
    int mOriginalHeight;

    //! Update viewport
    void UpdateViewport();

    //! Change resolution
    void SetResolution(CrtResolution resolution);

    //! Initialize all the view
    void Initialize();

    /*
     * Synchronous event
     */

    /*!
     * @brief Receive message from the CRT thread
     */
    void ReceiveSyncMessage() override;
};


