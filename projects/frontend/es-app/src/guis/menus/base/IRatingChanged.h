//
// Created by bkg2k on 30/08/24.
//
#pragma once

class IRatingChanged
{
  public:
    //! Default destructor
    virtual ~IRatingChanged() = default;

    /*!
     * @brief Called when a rating component has been updated
     * @param id Component id
     * @param value New value
     */
    virtual void MenuRatingMoved(int id, float value) = 0;
};
