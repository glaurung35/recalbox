//
// Created by bkg2k on 30/11/23.
//
#pragma once

#include <guis/Gui.h>

class IView
{
  public:
    //! View type
    enum class ViewType
    {
      None,           //!< Unitialized
      SplashScreen,   //!< Splash screen (startup or stop)
      SystemList,     //!< System list
      GameList,       //!< Game list
      GameClip,       //!< Game clip
      CrtCalibration, //!< CRT Calibration screen
    };

    //! Default destructor
    virtual ~IView() = default;

    /*!
     * @brief Constructor
     * @param type View type
     */
    IView(ViewType type, Gui& underlayingGui)
      : mViewType(type)
      , mGui(underlayingGui)
    {
    }

    //! Type getter
    [[nodiscard]] ViewType GetViewType() const { return mViewType; }

    //! Type getter
    [[nodiscard]] Gui& GetGui() const { return mGui; }

  private:
    //! ViewType
    ViewType mViewType;
    //! Underlaying Gui reference
    Gui& mGui;
};