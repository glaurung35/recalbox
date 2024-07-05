#pragma once

#include <guis/Gui.h>
#include <components/ImageComponent.h>
#include <components/TextComponent.h>
#include "themes/ThemeExtras.h"
#include "IArcadeGamelistInterface.h"
#include "SlowDataInformation.h"
#include "views/PictogramCaches.h"
#include <systems/SystemData.h>

class SystemManager;

enum class FileChangeType
{
  Added,
  Run,
  MetadataChanged,
  Removed,
  Sorted,
  DisplayUpdated,
};

class ISimpleGameListView : public Gui
                          , private IThemeSwitchable
{
  public:
    enum class Change
    {
      Resort, //!< New sorting required
      Update, //!< Update lists
    };

    ISimpleGameListView(WindowManager& window, SystemManager& systemManager, SystemData& system, const IGlobalVariableResolver& resolver, PictogramCaches& flagCache);

    ~ISimpleGameListView() override = default;

    /*!
     * @brief Get Arcade interface
     * @return Arcade interface or nullptr
     */
    virtual IArcadeGamelistInterface* getArcadeInterface()
    { return nullptr; }

    /*!
     * @brief Called when a major change occurs on the system
     * @param change Change type
     */
    virtual void onChanged(Change change);

    /*!
     * @brief Refresh name & properties of the given item
     */
    virtual void RefreshItem(FileData* game) = 0;

    bool ProcessInput(const InputCompactEvent& event) override;

    bool CollectHelpItems(Help& help) override;

    /*!
     * @brief Get available initials of games from the current gamelist
     * @return Unicode character array
     */
    virtual Array<String::Unicode> GetAvailableLetters();

    /*!
     * @brief Move to the given letter in the gamelist
     * @param unicode Unicode letter to move to
     */
    virtual void JumpToLetter(unsigned int unicode);

    /*!
     * @brief Move to next or previous letter in the gamelist
     * @param forward True to move forward, false to move back
     */
    virtual void JumpToNextLetter(bool forward);

    [[nodiscard]] const SystemData& System() const { return mSystem; }

    void updateInfoPanel();

    [[nodiscard]] virtual int Count() const = 0;

    [[nodiscard]] virtual bool IsEmpty() const = 0;

    virtual FileData* getCursor() = 0;

    virtual FileData* getDataAt(int i) = 0;

    virtual const String& getCursorText() = 0;

    virtual const String& getCursorTextAt(int i) = 0;

    virtual void setCursorStack(FileData*) = 0;

    virtual void setCursor(FileData*) = 0;

    virtual int getCursorIndex() = 0;

    virtual int getCursorIndexMax() = 0;

    virtual void setCursorIndex(int) = 0;

    virtual void OnGameSelected() = 0;

    virtual void removeEntry(FileData* fileData) = 0;

    [[nodiscard]] virtual const char* getName() const = 0;

    virtual void DoUpdateGameInformation(bool update) = 0;

    virtual void populateList(const FolderData& folder) = 0;

    virtual void refreshList() = 0;

    /*!
     * @brief Called back from view manager when a game exited and the user is back to the gamelist
     * @param game Game ran
     */
    virtual void ReturnedFromGame(FileData* game) { (void)game; }

    virtual FileData::List getFileDataList() = 0;

    /*!
     * @brief Must be called right after the constructor
     */
    void DoInitialize()
    {
      Initialize();
      SwitchToTheme(mSystem.Theme(), false, nullptr);
      populateList(mSystem.MasterRoot());
    }

    /*!
     * @brief Get available regions from the current game list
     * @return Region list (may be empty)
     */
    virtual Regions::List AvailableRegionsInGames() = 0;

    /*!
     * @brief Check if the current game (under cursor) has p2k file available
     * @return True if at least one p2k file has been found
     */
    [[nodiscard]] virtual bool HasCurrentGameP2K() const = 0;

    /*!
     * @brief Gamelist may update thos information if required
     * @param info
     */
    virtual void UpdateSlowData(const SlowDataInformation& info) = 0;

  protected:
    /*!
     * @brief Called right after the constructor
     */
    virtual void Initialize() = 0;

    virtual void launch(FileData* game) = 0;

    virtual void clean() = 0;

    virtual FileData* getEmptyListItem() = 0;

    //! Flag texture cache
    PictogramCaches& mPictogramCaches;

    //! Global variabel resolver
    const IGlobalVariableResolver& mResolver;

    SystemData& mSystem;

    //! SystemManager instance
    SystemManager& mSystemManager;

    TextComponent mHeaderText;
    ImageComponent mHeaderImage;
    ImageComponent mBackground;

    ThemeExtras mThemeExtras;

    std::stack<FolderData*> mCursorStack;

    /*
     * IThemeSwitchable
     */

    /*!
     * @brief Gamelist view process their own attached system
     * @return SystemData
     */
    [[nodiscard]] SystemData* SystemTheme() const override { return &mSystem; };

    /*!
     * @brief Called when theme switch.
     * @param theme New Theme
     * @param refreshOnly True if we need to refresh the current view only
     */
    void SwitchToTheme(const ThemeData& theme, bool refreshOnly, IThemeSwitchTick* interface) override;

  private:

    bool mVerticalMove;

    bool IsFavoriteSystem() { return mSystem.IsFavorite(); }
};
