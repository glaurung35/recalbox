//
// Created by bkg2k on 15/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <vector>
#include <utils/String.h>
#include <input/Input.h>
#include <input/IInputChange.h>
#include <utils/math/Misc.h>
#include <utils/os/fs/Path.h>

class InputMapper : private IInputChange
{
  public:
    //! Pad structure
    struct Pad
    {
      String mName;  //!< Real pad name
      String mUUID;  //!< Pad uuid
      Path   mPath;  //! /dev/input/eventX
      int    mIndex; //!< Pad index in InputManager list

      Pad() : mIndex(-1) {}

      Pad(const String& name, const String& uuid, const Path& path, int index)
        : mName(name)
        , mUUID(uuid)
        , mPath(path)
        , mIndex(index)
      {
      }

      void Set(const String& name, const String& uuid, const Path& path, int index)
      {
        mName = name;
        mUUID = uuid;
        mPath = path;
        mIndex = index;
      }

      void Reset()
      {
        mName.clear();
        mUUID.clear();
        mPath = Path();
        mIndex = -1;
      }

      [[nodiscard]] String LookupPowerLevel() const;

      [[nodiscard]] bool IsValid() const { return !mName.empty() && !mUUID.empty(); }

      [[nodiscard]] bool IsConnected() const { return !mName.empty() && !mUUID.empty() && mIndex >= 0; }

      [[nodiscard]] bool Equals(const Pad& to) const
      {
        return mName == to.mName &&
               mUUID == to.mUUID &&
               mIndex == to.mIndex;
      }

      [[nodiscard]] bool Same(const Pad& than) const
      {
        return mName == than.mName &&
               mUUID == than.mUUID;
      }

      [[nodiscard]] String AsString() const { return String(mName).Append('.').Append(mUUID).Append('.').Append(mIndex); }

      void Copy(const Pad& source)
      {
        mName = source.mName;
        mUUID = source.mUUID;
        mPath = source.mPath;
        mIndex = source.mIndex;
      }
    };

    //! Pad array
    typedef Pad PadArray[Input::sMaxInputDevices];
    //! Pad list
    typedef std::vector<Pad> PadList;

    //! Constructor
    explicit InputMapper(IInputChange* interface);

    //! Destructor
    virtual ~InputMapper();

    /*!
     * @brief Compose numbered name at the given index, by counting same pads' name/uuid with lower indexes
     * @param padArray Pad array
     * @param index Index of the pad to get name from
     * @return Name or numbered name
     */
    String GetDecoratedName(int index);

    /*!
     * @brief Get pat at the given index
     * @param index Index to retrieve the pad at
     * @return Pad
     */
    [[nodiscard]] const Pad& PadAt(int index) const { return mPads[Math::clampi(index, 0, Input::sMaxInputDevices - 1)]; }

    /*!
     * @brief Swap pads at the given positions
     * @param index1 First pad index
     * @param index2 Second pad index
     */
    void Swap(int index1, int index2);

  private:
    //! Pad array
    PadArray mPads;
    //! Connected pad list
    PadList mConnected;
    //! Unconnected pad list
    PadList mUnconnected;

    //! Interface
    IInputChange* mInterface;

    //! Rebuid the pad array, ready to be used
    void Build();

    /*!
     * @brief Load configuration from recalbox.conf
     */
    void LoadConfiguration();

    /*!
     * @brief Save configuration into recalbox.conf
     */
    void SaveConfiguration();

    //! Get available pad list
    static PadList AvailablePads();

    /*!
     * @brief Sort pads, pushing active ones first
     * This sort does not change the order of active pads
     * @param list List to sort
     */
    static void SortActiveFirst(PadArray& padArray);

    /*
     * IInputChange implementation
     */

    //! Refresh pad list
    void PadsAddedOrRemoved(bool removed) override;
};



