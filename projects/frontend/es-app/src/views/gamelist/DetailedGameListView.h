#pragma once

#include <views/gamelist/ISimpleGameListView.h>
#include "components/RatingComponent.h"
#include "components/DateTimeComponent.h"
#include "components/VideoComponent.h"
#include "components/BusyComponent.h"
#include "components/VerticalScrollableContainer.h"
#include "components/TextScrollComponent.h"
#include <views/ViewController.h>
#include <components/TextListComponent.h>
#include <games/EmptyData.h>
#include <scraping/scrapers/IScraperEngineStage.h>

class DetailedGameListView : public ISimpleGameListView
                           , public ITextListComponentOverlay<FileData*>
                           , private IScraperEngineStage
                           , private IVideoComponentAction
{
  public:
    DetailedGameListView(WindowManager& window, SystemManager& systemManager, SystemData& system, const IGlobalVariableResolver& resolver);

    ~DetailedGameListView() override;

    /*!
     * @brief Must be called right after the constructor
     */
    void Initialize() override;

    void SwitchToTheme(const ThemeData& theme, bool refreshOnly, IThemeSwitchTick* interface) override;

    [[nodiscard]] const char* getName() const override { return "detailed"; }

    void Update(int deltatime) override;
    void Render(const Transform4x4f& parentTrans) override;

    void DoUpdateGameInformation(bool update) final;

    [[nodiscard]] int Count() const override { return mList.Count(); };
    [[nodiscard]] bool IsEmpty() const override { return mList.IsEmpty(); };

    void populateList(const FolderData& folder) override;

    void refreshList() override { populateList(*mPopulatedFolder); }

    FileData::List getFileDataList() override { return mList.getObjects(); }

    /*!
     * @brief Get available regions from the current game list
     * @return Region list (may be empty)
     */
    Regions::List AvailableRegionsInGames() override;

  protected:
    //! Empty item
    EmptyData mEmptyListItem;

    //! Current folder
    const FolderData *mPopulatedFolder;

    //! Game list
    TextListComponent<FileData*> mList;

    void launch(FileData* game) override;
    void clean() override { mVideo.setVideo(Path::Empty, 0, 0); }

    /*!
     * @brief Get display name of the given game
     * @param game Game
     * @return Final display name
     */
    virtual String GetDisplayName(FileData& game);

    /*!
     * @brief Get description of the given game
     * @param game Game
     * @return Description
     */
    virtual String GetDescription(FileData& game) { return game.Metadata().Description(); }

    FileData* getCursor() override { return mList.getSelected(); }
    int getCursorIndex() override { return mList.getCursorIndex(); }
    int getCursorIndexMax() override {	return mList.size() - 1; }
    FileData* getDataAt(int i) override
    {
      if (i >= 0 && i < getCursorIndexMax()) return mList.getObjectAt(i);
      return mList.getObjectAt(0);
    }

    const String& getCursorText() override { return mList.getSelectedName(); }
    const String& getCursorTextAt(int i) override
    {
      if (i >= 0 && i < getCursorIndexMax()) return mList.getNameAt(i);
      static const String __nulltext;
      return __nulltext;
    }

    void setCursorStack(FileData* file) override;
    void setCursor(FileData* file) override;
    void setCursorIndex(int index) override;
    void removeEntry(FileData* fileData) override;

  private:
    //! Hovering time in ms before starting a seamless scrape
    static constexpr int sMaxHoveringTimeBeforeScraping = 200; // 200ms
    //! Elapsed time since starting seamless scraping before displaying busy animation
    static constexpr int sMaxScrapingTimeBeforeBusyAnim = 200; // 200ms

    //! Elapsed time on the selected game (hovering time)
    int mElapsedTimeOnGame;
    //! Scraping state (for visual feedback)
    bool mIsScraping;

    void initMDLabels();
    void initMDValues();

    ImageComponent mImage;
    ImageComponent mNoImage;
    VideoComponent mVideo;
    std::vector<ImageComponent *> mFolderContent;
    ImageComponent mRegions[Regions::RegionPack::sMaxRegions];

    TextComponent mLblRating, mLblReleaseDate, mLblDeveloper, mLblPublisher, mLblGenre, mLblPlayers, mLblLastPlayed, mLblPlayCount, mLblFavorite;
    TextComponent mFolderName;

    RatingComponent mRating;
    DateTimeComponent mReleaseDate;
    TextScrollComponent mDeveloper;
    TextScrollComponent mPublisher;
    TextScrollComponent mGenre;
    TextComponent mPlayers;
    DateTimeComponent mLastPlayed;
    TextComponent mPlayCount;
    TextComponent mFavorite;

    std::vector<TextComponent*> getMDLabels();
    std::vector<ThemableComponent*> getMDValues();

    VerticalScrollableContainer mDescContainer;
    TextComponent mDescription;

    BusyComponent mBusy;

    RecalboxConf& mSettings;

    //! Components linked by video component
    Array<Component*> mVideoLinks;

    //! region to flag texture
    HashMap<Regions::GameRegions, std::shared_ptr<TextureResource>> mRegionToTextures;

    //! Array of fade-in components (from transparent to opaque)
    Array<Component*> mFadeInList;
    //! Array of fade-out components (from opaque to transparent)
    Array<Component*> mFadeOutList;

    //! Flag width not included any margin
    int mFlagWidth;
    //! Flag margin
    int mFlagMargin;

    //! Last processed cursor item
    FileData* mLastCursorItem;
    //! Current game P2K status
    bool mLastCursorItemHasP2K;

    /*!
     * @brief Move a component into the fade-in list, removing it from the fade-out list if it was in.
     * @param component Component to move to the fade-in list
     */
    void MoveToFadeIn(Component* component)
    {
      mFadeOutList.Remove(component);
      if (!mFadeInList.Contains(component))
        mFadeInList.Add(component);
    }
    /*!
     * @brief Move a component into the fade-out list, removing it from the fade-in list if it was in.
     * @param component Component to move to the fade-out list
     */
    void MoveToFadeOut(Component* component)
    {
      mFadeInList.Remove(component);
      if (!mFadeOutList.Contains(component))
        mFadeOutList.Add(component);
    }

    /*!
     * @brief Move a component into the fade-in list, removing it from the fade-out list if it was in.
     * @param component Component to move to the fade-in list
     */
    void MoveToFadeIn(const Array<Component*>& components)
    {
      for(Component* component : components)
      {
        mFadeOutList.Remove(component);
        if (!mFadeInList.Contains(component))
          mFadeInList.Add(component);
      }
    }
    /*!
     * @brief Move a component into the fade-out list, removing it from the fade-in list if it was in.
     * @param component Component to move to the fade-out list
     */
    void MoveToFadeOut(const Array<Component*>& components)
    {
      for(Component* component : components)
      {
        mFadeInList.Remove(component);
        if (!mFadeOutList.Contains(component))
          mFadeOutList.Add(component);
      }
    }

    /*!
     * @brief Move a component into the fade-in list, removing it from the fade-out list if it was in.
     * Set opacity to 255 instantly
     * @param component Component to move to the fade-in list
     */
    void MoveToFadeInMax(Component* component)
    {
      mFadeOutList.Remove(component);
      if (!mFadeInList.Contains(component))
        mFadeInList.Add(component);
      component->setOpacity(255);
    }
    /*!
     * @brief Move a component into the fade-out list, removing it from the fade-in list if it was in.
     * Set opacity to 0 instantly
     * @param component Component to move to the fade-out list
     */
    void MoveToFadeOutMax(Component* component)
    {
      mFadeInList.Remove(component);
      if (!mFadeOutList.Contains(component))
        mFadeOutList.Add(component);
      component->setOpacity(255);
    }

    FileData* getEmptyListItem() override { return &mEmptyListItem; }

    std::vector<ThemableComponent*> getFolderComponents();
    std::vector<ThemableComponent*> getGameComponents(bool includeMainComponents = true);
    std::vector<ThemableComponent*> getScrapedFolderComponents();
    void setGameInfo(FileData* file, bool update);
    void setRegions(FileData* file);
    void setScrapedFolderInfo(FileData* file);

    /*!
     * @brief Build component list linked by the video component
     * @param theme Current theme
     */
    void BuildVideoLinks(const ThemeData& theme);

    /*!
     * @brief Gamelist may update those information if required
     * @param info
     */
    void UpdateSlowData(const SlowDataInformation& info) override;

    /*!
     * @brief Display folder info
     * @param folder Folder
     * @param count Game count in folder
     * @param path Path of folder images
     */
    void SetFolderInfo(FolderData* folder, int count, const FolderImagesPath& path);

    /*!
     * @brief Check if the game has P2k
     * @return
     */
    [[nodiscard]] bool HasCurrentGameP2K() const override { return mLastCursorItemHasP2K; };

    /*!
     * @brief Refresh name & properties of the given item
     */
    void RefreshItem(FileData* game) override;

    /*!
     * @brief Get generic game icons
     * @param item Item to get icon for
     * @return Icon
     */
    String getItemIcon(const FileData& item);

    /*!
     * @brief Set fading (or not) between NoImage & Image, regarding game data
     * @param game Game data
     * @param update True if the game video is still running
     */
    void SetImageFading(FileData* game, bool videoStillRunning);

    /*!
     * @brief Called when a game is selected
     */
    void OnGameSelected() final;

    /*!
     * @brief Get available regions from the given listt
     * @return Region list (may be empty)
     */
    static Regions::List AvailableRegionsInGames(FileData::List& list);

    /*
     * ITextListComponentOverlay<FileData*> implementation
     */

    /*!
     * @brief Apply (draw) an overlay in the given item rectangle and adjust rectangle position/size
     * so that the text won't draw over the overlay if required
     * @param position Top/Left of the item rectangle
     * @param size  Width/Height of the item rectangle
     * @param data Linked data
     */
    void OverlayApply(const Vector2f& position, const Vector2f& size, FileData* const& data, unsigned int& color) override;

    /*!
     * @brief Get the left offset (margin to the text) if any
     * @return left offset
     */
    float OverlayGetLeftOffset(FileData* const& data) override { (void)data; return 0.0f; }

    /*!
     * @brief Get the right offset (margin from text to right limit) if any
     * @return right offset
     */
    float OverlayGetRightOffset(FileData* const& data) override;

    /*
     * IScrapeEngineStage implementation
     */

    /*!
     * @brief Report scraping stage completion
     * @param game Target game
     * @param stage Last stage completed
     * @param changes Metadata that have been actually changed
     */
    void ScrapingStageCompleted(FileData* game, Stage stage, MetadataType changes) override;

    /*
     * IVideoComponentAction
     */

    /*!
     * @brief Called when a video component require action
     * @param source Source video component
     * @param action Required action
     */
    void VideoComponentRequireAction(const VideoComponent* source, Action action) final;
};
