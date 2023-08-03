//
// Created by bkg2k on 08/07/23.
//
#pragma once

#include "ISlowSystemOperation.h"

// Forward declaration
class SystemData;
class FileData;

class ISystemChangeNotifier
{
  public:
    //! Destructor
    virtual ~ISystemChangeNotifier() = default;

    //! System must show
    virtual void ShowSystem(SystemData* system) = 0;

    //! System must hide
    virtual void HideSystem(SystemData* system) = 0;

    //! System must be updated (games have been updated inside)
    virtual void UpdateSystem(SystemData* system) = 0;

    //! Move to selected systems if possible
    virtual void SelectSystem(SystemData* system) = 0;

    /*!
     * @brief Slow operation requested to high level UI class
     * @param interface Threaded operations methods
     * @param systems System list to work on
     */
    virtual void RequestSlowOperation(ISlowSystemOperation* interface, ISlowSystemOperation::List systems) = 0;
};
