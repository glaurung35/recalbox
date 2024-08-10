#include <systems/SystemManager.h>
#include "guis/menus/GuiMenuBase.h"
#include "guis/GuiMetaDataEd.h"
#include "views/gamelist/IArcadeGamelistInterface.h"

class ISimpleGameListView;

class GuiMenuGamelistOptions : public GuiMenuBase
                             , private GuiMetaDataEd::IMetaDataAction
                             , private IOptionListComponent<unsigned int>
                             , private IOptionListComponent<Regions::GameRegions>
                             , private IOptionListComponent<FileSorts::Sorts>
                             , private IOptionListMultiComponent<RecalboxConf::GamelistDecoration>
                             , private IGuiMenuBase
                             , private ISwitchComponent
{
  public:
    /*!
     * @brief Constructor
     * @param window Main window
     * @param system Target system
     * @param systemManager System manager
     */
    GuiMenuGamelistOptions(WindowManager&window, SystemData& system, SystemManager& systemManager, IArcadeGamelistInterface* arcadeInterface, const IGlobalVariableResolver& resolver);

    //! Destructor
    ~GuiMenuGamelistOptions() override;

  private:
    enum class Components
    {
      Download,
      JumpToLetter,
      Sorts,
      Regions,
      FlatFolders,
      FavoritesOnly,
      FavoritesFirst,
      MetaData,
      UpdateGamelist,
      Delete,
      DeleteScreeshot,
      SaveStates,
      MainMenu,
      Quit,
      Search,
      ArcadeOptions,
      AutorunGame,
      Decorations,
    };

    //! Global variable resolver
    const IGlobalVariableResolver& mResolver;
    //! System reference
    SystemData& mSystem;
    //! System manager reference
    SystemManager& mSystemManager;
    //! Gamelist UI reference
    ISimpleGameListView& mGamelist;
    //! Arcade interface
    IArcadeGamelistInterface* mArcade;

    std::shared_ptr<OptionListComponent<FileSorts::Sorts>> mListSort;
    std::shared_ptr<TextComponent> mGame;

    //! Refresh first menu entry
    void RefreshGameMenuContext();

    //! Get available letter List
    std::vector<ListEntry<unsigned int>> GetLetterEntries();
    //! Get available sort List
    std::vector<ListEntry<FileSorts::Sorts>> GetSortEntries();
    //! Get available region List
    std::vector<ListEntry<Regions::GameRegions>> GetRegionEntries();
    //! Get available manufacturers
    std::vector<ListEntry<int>> GetManufacturerEntries();
    //! Get gamelist decorations
    std::vector<ListEntry<RecalboxConf::GamelistDecoration>> GetDecorationEntries();

    /*!
     * @brief Refresh gamelist
     */
    static void ManageSystems();

    /*
     * GuiMetaDataEd::IMetaDataAction implementation
     */

    void Delete(FileData& game) override;
    void Modified(ISimpleGameListView* gamelistview, FileData& game) override;

    /*
     * IOptionListComponent<unsigned int> implementation
     */

    void OptionListComponentChanged(int id, int index, const unsigned int& value, bool quickChange) override;

    /*
     * IOptionListComponent<FileSorts::Sorts> implementation
     */
    void OptionListComponentChanged(int id, int index, const Regions::GameRegions& value, bool quickChange) override;

    /*
     * IOptionListComponent<FileSorts::Sorts> implementation
     */
    void OptionListComponentChanged(int id, int index, const FileSorts::Sorts& value, bool quickChange) override;

    /*
     * IGuiMenuBase implementation
     */

    void SubMenuSelected(int id) override;

    /*
     * ISwitchComponent implementation
     */

    void SwitchComponentChanged(int id, bool& status) override;

    /*
     * IOptionListMultiComponent<RecalboxConf::GamelistDecorations> implementation
     */

    void OptionListMultiComponentChanged(int id, const std::vector<RecalboxConf::GamelistDecoration>& value) override;
};
