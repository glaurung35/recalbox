//
// Created by bkg2k on 15/12/23.
//
#pragma once

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