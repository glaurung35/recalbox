#pragma once

#include <utils/storage/Array.h>
#include <utils/storage/HashMap.h>
#include <utils/String.h>
#include <utils/os/fs/Path.h>
#include "MetadataDescriptor.h"
#include "ItemType.h"
#include "utils/cplusplus/Bitflags.h"

// Forward declarations
class SystemData;
class FolderData;
class RootFolderData;

// A tree node that holds information for a file.
class FileData
{
  public:
    typedef HashMap<String, FileData*> StringMap;
    typedef Array<FileData*> List;
    typedef Array<const FileData*> ConstList;

    enum class TopLevelFilter
    {
      None              = 0x00000000,
      Favorites         = 0x00000001,
      Hidden            = 0x00000002,
      Yoko              = 0x00000004,
      LatestVersion     = 0x00000008,
      Adult             = 0x00000010,
      Preinstalled      = 0x00000020,
      NotAGame          = 0x00000040,
      Board             = 0x00000080,
      OneAndTwoPlayers  = 0x00000100,
      Tate              = 0x00000200,

    };

    //! Game filters
    enum class Filter
    {
      None             = 0,  //!< Include nothing
      Normal           = 1,  //!< Include normal files (not hidden, not favorite)
      Favorite         = 2,  //!< Include favorites
      Hidden           = 4,  //!< Include hidden
      Adult            = 8,
      NotLatest        = 16,
      NoGame           = 32,
      PreInstalled     = 64,
      Board            = 128,
      Yoko             = 256,
      OneAndTwoPlayers = 512,
      Tate             = 1024,
      All              = 2047, //!< Include all
    };

    //! Search attribute enumeration
    enum class SearchAttributes
    {
      ByName        = 1, //!< Search by name, excluding extension
      ByNameWithExt = 2, //!< Search by name, including extension
      ByHash        = 4, //!< Search by hash
      All           = 7, //!< All attributes
    };

    //! Precalc properties
    enum class InternalProperties : char
    {
       None         = 0, //!< No properties
       Preinstalled = 1, //!< It's a preinstalled game if path contains 'share_init'
       NotAGame     = 2, //!< It's not a game if filename contains '[BIOS]' or 'ZZZ'
    };

  protected:
    //! Top ancestor (link to system)
    RootFolderData& mTopAncestor;
    //! Parent folder
    FolderData* mParent;
    //! Item type - Const ensure mType cannot be modified after being set by the constructor, so that it's alays safe to use c-style cast for FolderData sub-class.
    const ItemType mType;
    const InternalProperties mProperties;

    /*!
     * Constructor for subclasses only
     * @param type
     * @param path Item path
     * @param system Parent system
     */
    FileData(ItemType type, const Path& path, RootFolderData& ancestor);

  private:
    //! Metadata
    MetadataDescriptor mMetadata;

    //! Get properties from the given path
    static InternalProperties BuildProperties(const Path& path);

  public:
    /*!
     * Constructor
     * @param path Item path on filesystem
     * @param system system to attach to
     */
    FileData(const Path& path, RootFolderData& ancestor);

    //! Destructor
    virtual ~FileData() = default;

    /*
     * Getters
     */

    [[nodiscard]] virtual String Name() const { return mMetadata.Name(); }
    [[nodiscard]] inline String Hash() const { return mMetadata.RomCrc32AsString(); }
    [[nodiscard]] inline ItemType Type() const { return mType; }
    [[nodiscard]] inline Path RomPath() const { return mMetadata.Rom(); }
    [[nodiscard]] inline FolderData* Parent() const { return mParent; }
    [[nodiscard]] inline RootFolderData& TopAncestor() const { return mTopAncestor; }
    [[nodiscard]] SystemData& System() const;

    /*
     * Booleans
     */

    [[nodiscard]] inline bool IsEmpty() const { return mType == ItemType::Empty; }
    [[nodiscard]] inline bool IsHeader() const { return mType == ItemType::Header; }
    [[nodiscard]] inline bool IsGame() const { return mType == ItemType::Game; }
    [[nodiscard]] inline bool IsFolder() const { return mType == ItemType::Folder || mType == ItemType::Root; }
    [[nodiscard]] inline bool IsRoot() const { return mType == ItemType::Root; }
    [[nodiscard]] inline bool IsTopMostRoot() const { return mType == ItemType::Root && mParent == nullptr; }

    /*
     * Setters
     */

    inline void SetParent(FolderData* parent) { mParent = parent; }

    /*!
     * Get Thumbnail path if there is one, or Image path.
     * @return file path (may be empty)
     */
    [[nodiscard]] Path ThumbnailOrImagePath() const { return mMetadata.HasThumnnail() ? mMetadata.Thumbnail() : mMetadata.Image(); }

    /*!
     * Return true if at least one image is available (thumbnail or regular image)
     * @return Boolean result
     */
    [[nodiscard]] bool HasThumbnailOrImage() const { return (mMetadata.HasThumnnail() || mMetadata.HasImage()); }

    /*!
     * const Metadata accessor for Read operations
     * @return const Metadata object
     */
    [[nodiscard]] const MetadataDescriptor& Metadata() const { return mMetadata; }

    /*!
     * Metadata accessor for Write operations only
     * @return Writable Metadata object
     */
    MetadataDescriptor& Metadata() { return mMetadata; }

    /*!
     * @brief Get Pad2Keyboard configuration file path
     * @return Pad2Keyboard configuration file path
     */
    [[nodiscard]] Path P2KPath() const { Path p(RomPath()); return p.ChangeExtension(p.Extension() + ".p2k.cfg"); }

    /*!
     * @brief Get recalbox.conf configuration file path
     * @return recalbox.conf configuration file path
     */
    [[nodiscard]] Path RecalboxConfPath() const { Path p(RomPath()); return p.ChangeExtension(p.Extension() + ".recalbox.conf"); }

      /*!
       * @brief Check if Pad2Keyboard configuration file exists
       * @return Trie if the Pad2Keyboard configuration file exists
       */
    [[nodiscard]] bool HasP2K() const;

    /*!
     * @brief Calculate rom CRC32 and store it in metadata
     * @return This
     */
    FileData& CalculateHash();

    /*!
     * @brief Get region string
     * @return
     */
    String Regions();

    /*!
     * @brief Check if file data can be displayable
     * @return displayable state
     */
    [[nodiscard]] bool IsDisplayable(TopLevelFilter topfilter) const;

    /*!
     * @brief Check if rom path equals the rom path of the given filedata
     * @param other Other filedata to compare rom path
     * @return True if rom path are equal, false otherwise
     */
    bool AreRomEqual(const FileData& other) { return mMetadata.AreRomEqual(other.mMetadata); }

    /*!
     * @brief Build a top level filter to use in visible file get/count methods
     * @return Top level filter
     */
    static TopLevelFilter BuildTopLevelFilter();
};

DEFINE_BITFLAG_ENUM(FileData::InternalProperties, int)
DEFINE_BITFLAG_ENUM(FileData::TopLevelFilter, int)
DEFINE_BITFLAG_ENUM(FileData::Filter, int)
DEFINE_BITFLAG_ENUM(FileData::SearchAttributes, int)
