//
// Created by bkg2k on 24/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

class ISubMenuSelected
{
  public:
    //! Default destructor
    virtual ~ISubMenuSelected() = default;

    //! Called when a submenu is selected
    virtual void SubMenuSelected(int id) = 0;
};