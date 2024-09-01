//
// Created by bkg2k on 30/08/24.
//
#pragma once

class IActionTriggered
{
  public:
    //! Default destructor
    virtual ~IActionTriggered() = default;

    /*!
     * @brief Called when an action button has been clicked
     * @param id Component id
     */
    virtual void MenuActionTriggered(int id) = 0;
};