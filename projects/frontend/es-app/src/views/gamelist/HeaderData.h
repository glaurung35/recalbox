//
// Created by bkg2k on 02/03/24.
//
#pragma once

#include <games/FileData.h>
#include <games/MetadataDescriptor.h>
#include <systems/SystemData.h>

/*!
 * @brief This class is used as a placeholder in gamelists
 * Name is brought into local space to avoid conflicts w/ optimized names in regular items
 */
class HeaderData : public FileData
{
  public:
    //!  Constructor
    HeaderData(const String& name, SystemData& system)
      : FileData(ItemType::Header, Path::Empty, system.MasterRoot())
      , mName(name)
      , mInt(0)
      , mFloat(0.f)
      , mFolded(false)
    {
    }

    //!  Constructor
    HeaderData(const String& name, SystemData& system, bool folded, bool isAlias)
      : FileData(ItemType::Header, Path::Empty, system.MasterRoot())
      , mName(name)
      , mInt(0)
      , mFloat(0.f)
      , mFolded(folded)
      , mIsAlias(isAlias)
    {
    }
    //! Constructor
    HeaderData(const String& name, SystemData& system, int idata, float fdata)
      : FileData(ItemType::Header, Path::Empty, system.MasterRoot())
      , mName(name)
      , mInt(idata)
      , mFloat(fdata)
      , mFolded(false)
    {
    }

    //! Get local name undecorated
    [[nodiscard]] String Name() const final { return mName; }

    //! Get decorated local name
    [[nodiscard]] String Name(bool lefticon) const
    {
      if (lefticon)
      {
        String result(mFolded ? "▶ " : "▼ ");
        result.Append(mName);
        return result;
      }
      String result(mName);
      result.Append(mFolded ? " ◀" : " ▼");
      return result;
    }

    //! Fold status
    [[nodiscard]] bool IsFolded() const { return mFolded; }

    //! IsAlias status
    [[nodiscard]] bool IsAlias() const { return mIsAlias; }

    //! Set fold/unfold
    void SetFolded(bool folded) { mFolded = folded; }
    //! Toggle folded state
    void ToogleFolded() { mFolded = !mFolded; }

    //! Integer data
    [[nodiscard]] int Int() const { return mInt; }
    //! Float data
    [[nodiscard]] float Float() const { return mFloat; }

  private:
    //! Direct name as string
    String mName;
    //! Integer data
    int mInt;
    //! float data
    float mFloat;
    //! Fold status
    bool mFolded;

    //! is alias
    bool mIsAlias = false;

    using FileData::Metadata;
};
