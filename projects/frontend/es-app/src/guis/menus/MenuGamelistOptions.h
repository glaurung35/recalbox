#include <systems/SystemManager.h>
#include "guis/menus/base/Menu.h"
#include "guis/GuiMetaDataEd.h"

class ISimpleGameListView;

class MenuGamelistOptions : public Menu
                          , private GuiMetaDataEd::IMetaDataAction
                          , private ISingleSelectorChanged<String::Unicode>
                          , private ISingleSelectorChanged<Regions::GameRegions>
                          , private ISingleSelectorChanged<FileSorts::Sorts>
                          , private ISubMenuSelected
                          , private ISwitchChanged
                             , private IOptionListMultiComponent<RecalboxConf::GamelistDecoration>
{
  public:
    /*!
     * @brief Constructor
     * @param window Main window
     * @param system Target system
     * @param systemManager System manager
     */
    MenuGamelistOptions(WindowManager&window, SystemData& system, SystemManager& systemManager, IArcadeGamelistInterface* arcadeInterface, const IGlobalVariableResolver& resolver);

    //! Destructor
    ~MenuGamelistOptions() override;

  private:
    enum class Components
    {
      Download,
      JumpToLetter,
      Sorts,
      Regions,
      FlatFolders,
      FavoritesOnly,
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
    //! Game submenu item
    ItemSubMenu* mGame;

    ItemSelector<FileSorts::Sorts>* mListSort;

    //! Refresh first menu entry
    void RefreshGameMenuContext();

    //! Get available letter List
    SelectorEntry<String::Unicode>::List GetLetterEntries();
    //! Get available sort List
    SelectorEntry<FileSorts::Sorts>::List GetSortEntries();
    //! Get available region List
    SelectorEntry<Regions::GameRegions>::List GetRegionEntries();
    //! Get available manufacturers
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

    void MenuSingleChanged(int id, int index, const unsigned int& value) override;

    /*
     * IOptionListComponent<FileSorts::Sorts> implementation
     */
    void MenuSingleChanged(int id, int index, const Regions::GameRegions& value) override;

    /*
     * IOptionListComponent<FileSorts::Sorts> implementation
     */
    void MenuSingleChanged(int id, int index, const FileSorts::Sorts& value) override;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;

    /*
     * ISwitchComponent implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;
};
