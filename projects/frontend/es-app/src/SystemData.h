#pragma once

#include <vector>
#include <string>
#include "RootFolderData.h"
#include "Window.h"
#include "PlatformId.h"
#include "themes/ThemeData.h"
#include "FileSorts.h"

#include <boost/property_tree/ptree.hpp>
#include <RootFolders.h>

class SystemData
{
	public:
		//! convenient ptree type access
		typedef boost::property_tree::ptree Tree;
		typedef std::pair<std::string, Tree> TreeNode;

		//! Convenient alias for system collision map
		typedef std::map<std::string, int> XmlNodeCollisionMap;
		//! Convenient alias for XML node list
		typedef std::vector<Tree> XmlNodeList;

	private:
		std::string mName;
		std::string mFullName;
		std::string mStartPath;
		std::string mSearchExtensions;
		std::string mLaunchCommand;
		std::vector<PlatformIds::PlatformId> mPlatformIds;
		std::string mThemeFolder;
		std::shared_ptr<ThemeData> mTheme;

		bool mIsFavorite;
		unsigned int mSortId;

		void populateFolder(FolderData* folder, FileData::StringMap& doppelgangerWatcher);

		RootFolderData mRootFolder;
		std::map<std::string, std::vector<std::string> *> mEmulators;

		/*!
     * Run though the system list and store system nodes into the given store.
     * If a system already exists in the store, the new system node is ignored
     * @param collisionMap Collision map to keep track of nodes in the store
     * @param nodeStore Node store to fill with unique system nodes
     * @param document Source document
     * @return true if the system list contains one or more system. False if the systemList node does not exist or is empty.
     */
		static bool loadSystemNodes(XmlNodeCollisionMap& collisionMap, XmlNodeList& nodeStore, const Tree& document);

		/*!
     */

		/*!
     * Load systemList file into the given XML Document, then parse and store systems into the given node store
     * Perform all file/xml checks.
     * @param document Xml Document to store content to
     * @param collisionMap Collision map to keep track of nodes in the store
     * @param nodeStore System node store
     * @return true if the operation is successful and at least one system has been processed. False otherwise
     */
		static bool loadSystemList(Tree& document, XmlNodeCollisionMap& collisionMap, XmlNodeList& nodeStore, const std::string& filepath);

		/*!
     * Load and parse the given file to populate a property tree
     * @param document Document to populate
     * @param filepath Filepath to load & parse
     * @return false if the file does not exist or if the file is not parsable.
     */
		static bool loadXmlFile(Tree& document, const std::string& filepath);

		static bool AddFavoriteSystem(const XmlNodeList& systemList);

		static bool AddArcadeMetaSystem();

    static SystemData* CreateRegularSystem(const std::string& name, const std::string& fullName, const std::string& startPath,
                                           const std::string& filteredExtensions, const std::string& command,
                                           const std::vector<PlatformIds::PlatformId>& platformIds, const std::string& themeFolder,
                                           const Tree& emuNodes);

    static SystemData* CreateFavoriteSystem(const std::string& name, const std::string& fullName,
                                            const std::string& themeFolder, const std::vector<SystemData*>& systems);

    static SystemData* CreateMetaSystem(const std::string& name, const std::string& fullName,
                                        const std::string& themeFolder, const std::vector<SystemData*>& systems);

    SystemData(const std::string& name, const std::string& fullName, const std::string& startPath,
               const std::string& filteredExtensions, const std::string& command,
               const std::vector<PlatformIds::PlatformId>& platformIds, const std::string& themeFolder,
               const Tree* emuNodes, bool childOwnership);

    /*!
     * @brief Called from loading threads
     * @param system XML system descriptor
     * @return New SystemData instance
     */
    static SystemData* createSystem(const SystemData::Tree &system);

    //! Visible system, including virtual system (Arcade)
    static std::vector<SystemData*> sSystemVector;
    //! Hidden system, just here to hold their own children
    static std::vector<SystemData*> sHiddenSystemVector;
    //! ALL systems, visible and hidden
    static std::vector<SystemData*> sAllSystemVector;

  public:

    ~SystemData();

    inline RootFolderData* getRootFolder() { return &mRootFolder; };
    inline const std::string& getName() const { return mName; }
    inline const std::string& getFullName() const { return mFullName; }
    inline const std::string& getStartPath() const { return mStartPath; }
    //inline const std::vector<std::string>& getExtensions() const { return mSearchExtensions; }
    inline const std::string& getThemeFolder() const { return mThemeFolder; }
    inline bool getHasFavoritesInTheme() const { return mTheme->getHasFavoritesInTheme(); }
    inline bool isFavorite() const { return mIsFavorite; }
    inline FileData::List getFavorites() const { return mRootFolder.getAllFavoritesRecursively(false); }
    inline unsigned int getSortId() const { return mSortId; };
    inline FileSorts::SortType getSortType(bool forFavorites) const { return forFavorites ? FileSorts::SortTypesForFavorites.at(mSortId) : FileSorts::SortTypes.at(mSortId); };
    inline void setSortId(const unsigned int sortId = 0) { mSortId = sortId; };

    inline const std::vector<PlatformIds::PlatformId>& getPlatformIds() const { return mPlatformIds; }
    inline bool hasPlatformId(PlatformIds::PlatformId id) { return std::find(mPlatformIds.begin(), mPlatformIds.end(), id) != mPlatformIds.end(); }

    inline const std::shared_ptr<ThemeData>& getTheme() const { return mTheme; }

    std::string getGamelistPath(bool forWrite) const;
    //bool hasGamelist() const;
    std::string getThemePath() const;

    bool hasGame() const { return mRootFolder.hasGame(); }
    int getGameCount() const { return mRootFolder.countAll(false); };
    int getFavoritesCount() const{ return mRootFolder.countAllFavorites(false); };
    int getHiddenCount() const{ return mRootFolder.countAllHidden(false); };

    void launchGame(Window* window, FileData* game, const std::string& netplay, const std::string& core, const std::string& ip, const std::string& port);

    static void deleteSystems();
    static bool loadConfig(); //Load the system config file at getConfigPath(). Returns true if no errors were encountered. An example will be written if the file doesn't exist.
    static void writeExampleConfig(const std::string& path);

    static SystemData* getFavoriteSystem();
    static SystemData* getSystem(std::string& name);
    static SystemData* getFirstSystemWithGame();
    static int getVisibleSystemIndex(const std::string& name);

    static const std::vector<SystemData*>& getAllSystems() { return sAllSystemVector; }
    static const std::vector<SystemData*>& getVisibleSystems() { return sSystemVector; }
    static const std::vector<SystemData*>& getHiddenSystems() { return sHiddenSystemVector; }

    inline std::vector<SystemData*>::const_iterator getIterator() const { return std::find(sSystemVector.begin(), sSystemVector.end(), this); };
    inline std::vector<SystemData*>::const_reverse_iterator getRevIterator() const { return std::find(sSystemVector.rbegin(), sSystemVector.rend(), this); };

    inline SystemData* getNext() const
    {
      auto it = getIterator();
      it++;
      if(it == sSystemVector.end()) it = sSystemVector.begin();
      return *it;
    }

    inline SystemData* getPrev() const
    {
      auto it = getRevIterator();
      it++;
      if(it == sSystemVector.rend()) it = sSystemVector.rbegin();
      return *it;
    }

    // Load or re-load theme.
    void loadTheme();

    std::map<std::string, std::vector<std::string> *> * getEmulators() { return &mEmulators; }

    std::vector<std::string> getCores(const std::string& emulatorName);

    static std::string getUserConfigurationAbsolutePath()     { return RootFolders::DataRootFolder     + "/system/.emulationstation/es_systems.cfg"; }
    static std::string getTemplateConfigurationAbsolutePath() { return RootFolders::TemplateRootFolder + "/system/.emulationstation/es_systems.cfg"; }

    static std::string demoInitialize(Window& window);
    static void demoFinalize(Window& window);
    bool demoLaunchGame(FileData* game, int duration, int infoscreenduration, const std::string& controlersConfig);
};
