#pragma once

#include "FolderData.h"

class RootFolderData : public FolderData
{
  public:
    //! Ownership
    enum class Ownership : char
    {
      None      , //!< This rootfolder has no ownership at all
      FolderOnly, //!< This rootfolder owns all sub folders recursively, but no game
      All       , //!< This rootfolder owns all folder and game structures
    };

    enum class Types : char
    {
      None    , //!< Nothing particular
      ReadOnly, //!< Read only tree
      Virtual , //!< Virtual tree
    };

    /*!
     * Constructor
     */
    RootFolderData(RootFolderData& topAncestor, Ownership childownership, Types type, const Path& startpath, SystemData& system)
      : FolderData(topAncestor, startpath)
      , mSystem(system)
      , mChildOwnership(childownership)
      , mType(type)
      , mPreinstalled(startpath.ToString().Contains("/share_init"))
    {
    }

    /*!
     * Since this destructor is called before ~FileData
     * this is the opportuniy to clear child list before they are destroyed
     */
    ~RootFolderData() override
    {
      // Destroy any subroot first
      for(RootFolderData* root : SubRoots())
      {
        RemoveChild(root);
        delete root;
      }

      // Then remaining items
      switch(mChildOwnership)
      {
        // Clear child list so that none are dstroyed
        case Ownership::None: ClearChildList(); break;
        // Clear only game, keep the folder structure and let them being destroyed
        case Ownership::FolderOnly: ClearSubChildList(); break;
        // Let everything be detroyed & destroy deleted games
        case Ownership::All:
        {
          for(auto* FileData : mDeletedChildren)
            delete FileData;
          break;
        }
      }
    }

    /*!
     * @brief Delete virtual sub tree
     */
    void DeleteVirtualSubTree()
    {
      for(FileData* child : mChildren)
        if (child->IsRoot())
          if (((RootFolderData*)child)->Virtual())
          {
            RemoveChild(child);
            delete child;
          }
    }

    //! System
    [[nodiscard]] SystemData& RootSystem() const { return mSystem; }

    //! Get type
    [[nodiscard]] Types RootType() const { return mType; }

    //! Normal folder?
    [[nodiscard]] bool Normal() const { return (mType == Types::None); }

    //! Is read only?
    [[nodiscard]] bool ReadOnly() const { return (mType == Types::ReadOnly); }

    //! Virtual folder?
    [[nodiscard]] bool Virtual() const { return (mType == Types::Virtual); }

    //! Preinstalled folder?
    [[nodiscard]] bool PreInstalled() const { return mPreinstalled; }

    //! Has sub root?
    [[nodiscard]] bool HasSubRoots() const
    {
      for(FileData* child : mChildren)
        if (child->IsRoot())
          return true;
      return false;
    }

    //! Get sub-root folders
    [[nodiscard]] std::vector<RootFolderData*> SubRoots() const
    {
      std::vector<RootFolderData*> roots;
      for(FileData* child : mChildren)
        if (child->IsRoot())
          roots.push_back((RootFolderData*)child);
      return roots;
    }

    /*!
     * @brief Add Sub-root
     * @param subroot Sub-root to add
     */
    void AddSubRoot(RootFolderData* subroot) { AddChild(subroot, true); }

    /*!
     * @brief Get deleted children
     * @return Deleted children
     */
    [[nodiscard]] const HashSet<FileData*>& GetDeletedChildren() const { return mDeletedChildren; }

    /*!
     * @brief Check if this root has deleted children
     * @return True if it has deleted children, false otherwise
     */
    [[nodiscard]] bool HasDeletedChildren() const { return !mDeletedChildren.empty(); }

    /*!
     * @brief Delete child or grand-child
     * @param file File to be inserted
     */
    static void DeleteChild(FileData* file)
    {
      file->TopAncestor().mDeletedChildren.insert(file);
      file->Parent()->RemoveChild(file);
    }

  private:
    //! Deleted children list
    HashSet<FileData*> mDeletedChildren;
    //! Parent system
    SystemData& mSystem;
    //! Type of child ownership
    Ownership mChildOwnership;
    //! This folder and all its subtree is readonly
    Types mType;
    //! Preinstalled folder?
    bool mPreinstalled;

    // Hide addChild
    using FolderData::AddChild;
};
