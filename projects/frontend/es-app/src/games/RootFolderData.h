#pragma once

#include "FolderData.h"

class RootFolderData : public FolderData
{
  private:
    bool mChildOwnership;

  public:
    /*!
     * Constructor
     */
    RootFolderData(bool childownership, const Path& startpath, SystemData* system)
      : FolderData(startpath, system),
        mChildOwnership(childownership)
    {
    }

    /*!
     * Since this destructor is called before ~FileData
     * this is the opportuniy to clear child list before they are destroyed
     */
    virtual ~RootFolderData()
    {
      if (!mChildOwnership)
        ClearChildList();
    }

    bool hasChildrenOwnership() const { return mChildOwnership; }
};


