#include <RecalboxConf.h>
#include <VideoEngine.h>
#include "views/gamelist/DetailedGameListView.h"
#include "animations/LambdaAnimation.h"
#include "scraping/ScraperSeamless.h"
#include "recalbox/RecalboxStorageWatcher.h"
#include "guis/GuiScraperRun.h"

DetailedGameListView::DetailedGameListView(WindowManager&window, SystemManager& systemManager, SystemData& system, const IGlobalVariableResolver& resolver, PictogramCaches& pictogramCache)
  : ISimpleGameListView(window, systemManager, system, resolver, pictogramCache)
  , mEmptyListItem(_("YOUR LIST IS EMPTY. PRESS START TO CHANGE GAME FILTERS."), system)
  , mPopulatedFolder(nullptr)
  , mList(window)
  , mElapsedTimeOnGame(0)
  , mIsScraping(false)
  , mHeaderStars(window, 0.f)
  , mImage(window)
  , mNoImage(window)
  , mVideo(window, this)
  , mRegions { ImageComponent(window), ImageComponent(window), ImageComponent(window), ImageComponent(window) }
  , mLblRating(window)
  , mLblReleaseDate(window)
  , mLblDeveloper(window)
  , mLblPublisher(window)
  , mLblGenre(window)
  , mLblPlayers(window)
  , mLblLastPlayed(window)
  , mLblPlayCount(window)
  , mLblFavorite(window)
  , mFolderName(window)
  , mRating(window, 0.f)
  , mReleaseDate(window)
  , mDeveloper(window)
  , mPublisher(window)
  , mGenre(window)
  , mPlayers(window)
  , mLastPlayed(window)
  , mPlayCount(window)
  , mFavorite(window)
  , mDecorationFlagsTemplate(window)
  , mDecorationGenreTemplate(window)
  , mDecorationPlayersTemplate(window)
  , mDescContainer(window)
  , mDescription(window)
  , mBusy(window)
  , mSettings(RecalboxConf::Instance())
  , mFlagWidth(0)
  , mFlagMargin(Renderer::Instance().Is480pOrLower() ? 1 : 2)
  , mSort(FileSorts::Sorts::FileNameAscending)
  , mDecorations(mSettings.GetSystemGamelistDecoration(system))
  , mLastCursorItem(nullptr)
  , mLastCursorItemHasP2K(false)
{
}

void DetailedGameListView::Initialize()
{
  addChild(&mList);

  mList.setCursorChangedCallback([this](const CursorState& state)
                                 {
                                   (void) state;
                                   updateInfoPanel();
                                 });
  mList.SetOverlayInterface(this);

  const float padding = 0.01f;

  mList.setDefaultZIndex(20);
  mList.setPosition(mSize.x() * (0.50f + padding), mList.getPosition().y());
  mList.setSize(mSize.x() * (0.50f - padding), mList.getSize().y());
  mList.setAlignment(HorizontalAlignment::Left);

  // folder components
  for (int i = sFoldersMaxGameImageCount; --i >= 0; )
  {
    auto* img = new ImageComponent(mWindow);
    addChild(img); // normalised functions required to be added first
    img->setOrigin(0.5f, 0.5f);
    img->setNormalisedSize(0.4f, 0.4f);
    img->setKeepRatio(true),
    img->setDefaultZIndex(30);
    img->setZIndex(30);
    img->setOpacity(0);
    mFolderContent.push_back(img);
  }

  addChild(&mFolderName);
  mFolderName.setDefaultZIndex(40);

  // image
  mImage.setOrigin(0.5f, 0.5f);
  mImage.setPosition(mSize.x() * 0.25f, mList.getPosition().y() + mSize.y() * 0.2125f);
  mImage.setResize(mSize.x() * (0.50f - 2 * padding), mSize.y() * 0.4f);
  mImage.setKeepRatio(true);
  mImage.setDefaultZIndex(30);
  mImage.setOpacity(0);

  // no image
  mNoImage.setOrigin(mImage.getOrigin());
  mNoImage.setPosition(mImage.getPosition());
  mNoImage.setResize(mImage.getSize());
  mNoImage.setKeepRatio(true);
  mNoImage.setDefaultZIndex(30);
  mNoImage.setOpacity(0);

  addChild(&mNoImage);
  addChild(&mImage);

  // video
  mVideo.setOrigin(0.5f, 0.5f);
  mVideo.setPosition(mSize.x() * 0.25f, mList.getPosition().y() + mSize.y() * 0.2125f);
  mVideo.setResize(mSize.x() * (0.50f - 2 * padding), mSize.y() * 0.4f);
  mVideo.setKeepRatio(true);
  mVideo.setDefaultZIndex(30);
  addChild(&mVideo);

  // Busy
  mBusy.setPosition(mImage.getPosition());
  mBusy.setSize(mImage.getSize());
  mBusy.setText(_("UPDATING..."));

  // metadata labels + values
  addChild(&mLblRating);
  addChild(&mRating);
  addChild(&mLblReleaseDate);
  addChild(&mReleaseDate);
  addChild(&mLblDeveloper);
  addChild(&mDeveloper);
  addChild(&mLblPublisher);
  addChild(&mPublisher);
  addChild(&mLblGenre);
  addChild(&mGenre);
  addChild(&mLblPlayers);
  addChild(&mPlayers);
  addChild(&mLblLastPlayed);
  mLastPlayed.setDisplayMode(DateTimeComponent::Display::RelativeToNow);
  addChild(&mLastPlayed);
  addChild(&mLblPlayCount);
  addChild(&mPlayCount);
  addChild(&mLblFavorite);
  addChild(&mFavorite);

  for (int i = Regions::RegionPack::sMaxRegions; --i >= 0; )
  {
    addChild(&mRegions[i]); // normalised functions required to be added first
    mRegions[i].setDefaultZIndex(40);
    //mRegions[i].setThemeDisabled(true);
  }

  mDescContainer.setPosition(mSize.x() * padding, mSize.y() * 0.65f);
  mDescContainer.setSize(mSize.x() * (0.50f - 2 * padding), mSize.y() - mDescContainer.getPosition().y());
  mDescContainer.setDefaultZIndex(40);
  addChild(&mDescContainer);

  mDescription.setFont(Font::get(FONT_SIZE_SMALL));
  mDescription.setSize(mDescContainer.getSize().x(), 0);
  mDescContainer.addChild(&mDescription);

  initMDLabels();
  initMDValues();
  updateInfoPanel();
}

void DetailedGameListView::SwitchToTheme(const ThemeData& theme, bool refreshOnly, IThemeSwitchTick* interface)
{
  ISimpleGameListView::SwitchToTheme(theme, refreshOnly, interface);

  mList.DoApplyThemeElement(theme, getName(), "gamelist", ThemePropertyCategory::All);
  // Set color 2/3 50% transparent of color 0/1
  mList.setColor(GameHighlightColor, (mList.Color(GameColor) & 0xFFFFFF00) | ((mList.Color(GameColor) & 0xFF) >> 1));
  mList.setColor(FolderHighlightColor, (mList.Color(FolderColor) & 0xFFFFFF00) | ((mList.Color(FolderColor) & 0xFF) >> 1));
  mList.setColor(HeaderColor, /*(mList.Color(GameColor) & 0xFFFFFF00) |*/ 0x40);
  sortChildren();

  // Compute flag width
  mFlagWidth = (int)(mList.getFont()->getHeight(1.f) * (71.f / 48.f));

  for (int i = 0; i < (int)Regions::RegionPack::sMaxRegions; i++)
    mRegions[i].DoApplyThemeElement(theme, getName(), String("md_region").Append(i + 1).c_str(),
                                    ThemePropertyCategory::Position | ThemePropertyCategory::Size | ThemePropertyCategory::ZIndex | ThemePropertyCategory::Path);

  mImage.DoApplyThemeElement(theme, getName(), "md_image", ThemePropertyCategory::All ^ ThemePropertyCategory::Path);
  mNoImage.DoApplyThemeElement(theme, getName(), "md_image", ThemePropertyCategory::All ^ ThemePropertyCategory::Path);
  mNoImage.setImage(Path(":/no_image.png"));
  mNoImage.DoApplyThemeElement(theme, getName(), "default_image_path", ThemePropertyCategory::Path);
  mNoImage.setThemeDisabled(false);
  mVideo.DoApplyThemeElement(theme, getName(), "md_video", ThemePropertyCategory::All ^ ThemePropertyCategory::Path);

  mDecorationFlagsTemplate.DoApplyThemeElement(theme, getName(), "template_flag", ThemePropertyCategory::Color);
  mDecorationGenreTemplate.DoApplyThemeElement(theme, getName(), "template_genre", ThemePropertyCategory::Color);
  mDecorationPlayersTemplate.DoApplyThemeElement(theme, getName(), "template_players", ThemePropertyCategory::Color);

  BuildVideoLinks(theme);

  initMDLabels();
  Array<TextComponent*> labels = getMDLabels();
  std::vector<String> names({
                                   "md_lbl_rating",
                                   "md_lbl_releasedate",
                                   "md_lbl_developer",
                                   "md_lbl_publisher",
                                   "md_lbl_genre",
                                   "md_lbl_players",
                                   "md_lbl_lastplayed",
                                   "md_lbl_playcount"
                                 });

  names.push_back("md_lbl_favorite");

  assert((int)names.size() == labels.Count());
  for (int i = 0; i < labels.Count(); i++)
  {
    labels[i]->DoApplyThemeElement(theme, getName(), names[i], ThemePropertyCategory::All);
  }

  initMDValues();
  Array<ThemableComponent*> values = getMDValues();
  names = {
    "md_rating",
    "md_releasedate",
    "md_developer",
    "md_publisher",
    "md_genre",
    "md_players",
    "md_lastplayed",
    "md_playcount"
  };

  names.push_back("md_favorite");

  names.push_back("md_folder_name");
  values.Add(&mFolderName);

  assert((int)names.size() == values.Count());
  for (int i = 0; i < values.Count(); i++)
  {
    values[i]->DoApplyThemeElement(theme, getName(), names[i], ThemePropertyCategory::All ^ ThemePropertyCategory::Text);
  }

  //mHeaderStars.DoApplyThemeElement(theme, getName(), "md_rating", ThemePropertyCategory::All);
  mDescContainer.DoApplyThemeElementPolymorphic(theme, getName(), "md_description", ThemePropertyCategory::Position | ThemePropertyCategory::Size | ThemePropertyCategory::ZIndex);
  mDescription.setSize(mDescContainer.getSize().x(), 0);
  mDescription.DoApplyThemeElement(theme, getName(), "md_description",
                                   ThemePropertyCategory::All ^ (ThemePropertyCategory::Position | ThemePropertyCategory::Size | ThemePropertyCategory::Origin | ThemePropertyCategory::Text));
  mBusy.SetFont(mDescription.getFont());

  mLblRating.setText(_("Rating") + ": ");
  mLblReleaseDate.setText(_("Released") + ": ");
  mLblDeveloper.setText(_("Developer") + ": ");
  mLblPublisher.setText(_("Publisher") + ": ");
  mLblGenre.setText(_("Genre") + ": ");
  mLblPlayers.setText(_("Players") + ": ");
  mLblLastPlayed.setText(_("Last played") + ": ");
  mLblPlayCount.setText(_("Times played") + ": ");
  mLblFavorite.setText(_("Favorite") + ": ");

  if (theme.isFolderHandled())
  {
    mFolderName.DoApplyThemeElement(theme, getName(), "md_folder_name", ThemePropertyCategory::All);
    for (int i = 0; i < (int) mFolderContent.size(); i++)
    {
      String folderImage("md_folder_image_"); folderImage.Append(i);
      mFolderContent[i]->DoApplyThemeElement(theme, getName(), folderImage,
                                             ThemePropertyCategory::Position | ThemePropertyCategory::Size | ThemePropertyCategory::ZIndex | ThemePropertyCategory::Rotation);
    }
  }
  else
  {
    // backward compatibility
    auto size = mImage.getSize().isZero() ? mImage.getTargetSize() : mImage.getSize();
    float minSize = Math::min(size.x(), size.y());
    float left = mImage.getPosition().x() - mImage.getOrigin().x() * minSize;
    float top = mImage.getPosition().y() - mImage.getOrigin().y() * minSize;

    mFolderName.setPosition(left, top);
    mFolderName.setZIndex(40);
    mFolderName.setFont(Font::get(FONT_SIZE_EXTRASMALL));
    mFolderName.setColor(0xFFFFFFFF);

    const unsigned int grid = 3; // 3 x 3
    const float relativeMarge = 0.1f;

    float labelHeight = 2.5f * Font::get(FONT_SIZE_EXTRASMALL)->getLetterHeight();
    top += labelHeight;

    minSize = Math::min(size.x(), size.y() - labelHeight);

    const float imgSize = minSize / (grid + 2.0f * relativeMarge);

    // centering with unused space
    left += 0.5f * (minSize - grid * imgSize - 2.0f * relativeMarge);

    for (unsigned int x = 0; x < grid; x++)
    {
      for (unsigned int y = 0; y < grid; y++)
      {
        ImageComponent* img = mFolderContent[x + y * grid];
        img->setResize(imgSize, imgSize);
        img->setKeepRatio(true);
        img->setPosition(left + imgSize * img->getOrigin().x() + (float)x * (1 + relativeMarge) * imgSize,
                         top + imgSize * img->getOrigin().y() + (float)y * (1 + relativeMarge) * imgSize);
        img->setZIndex(30);
      }
    }
  }

  sortChildren();
  DoUpdateGameInformation(false);
  (void)interface;
}

void DetailedGameListView::initMDLabels()
{
  Array<TextComponent*> components = getMDLabels();

  //const unsigned int colCount = 2;
  const unsigned int rowCount = (unsigned int) (components.Count() / 2);

  Vector3f start(mSize.x() * 0.01f, mSize.y() * 0.625f, 0.0f);

  //const float colSize = (mSize.x() * 0.48f) / colCount;
  const float rowPadding = 0.01f * mSize.y();

  Vector3f pos = start + Vector3f(0, 0, 0);
  for (int i = 0; i < components.Count(); i++)
  {
    const unsigned int row = i % rowCount;
    if (row != 0)
    {
      // work from the last component
      Component* lc = components[i - 1];
      pos = lc->getPosition() + Vector3f(0, lc->getSize().y() + rowPadding, 0);
    }

    components[i]->setFont(Font::get(FONT_SIZE_SMALL));
    components[i]->setPosition(pos);
    components[i]->setDefaultZIndex(40);
  }
}

void DetailedGameListView::initMDValues()
{
  Array<TextComponent*> labels = getMDLabels();
  Array<ThemableComponent*> values = getMDValues();

  std::shared_ptr<Font> defaultFont = Font::get(FONT_SIZE_SMALL);
  mRating.setSize(defaultFont->getHeight() * 5.0f, defaultFont->getHeight());
  mReleaseDate.setFont(defaultFont);
  mDeveloper.setFont(defaultFont);
  mPublisher.setFont(defaultFont);
  mGenre.setFont(defaultFont);
  mPlayers.setFont(defaultFont);
  mLastPlayed.setFont(defaultFont);
  mPlayCount.setFont(defaultFont);
  mFavorite.setFont(defaultFont);

  float bottom = 0.0f;

  const float colSize = (mSize.x() * 0.48f) / 2;
  for (int i = 0; i < labels.Count(); i++)
  {
    const float heightDiff = (labels[i]->getSize().y() - values[i]->getSize().y()) / 2;
    values[i]->setPosition(labels[i]->getPosition() + Vector3f(labels[i]->getSize().x(), heightDiff, 0));
    values[i]->setSize(colSize - labels[i]->getSize().x(), values[i]->getSize().y());
    values[i]->setDefaultZIndex(40);

    float testBot = values[i]->getPosition().y() + values[i]->getSize().y();
    if (testBot > bottom)
      bottom = testBot;
  }

  mDescContainer.setPosition(mDescContainer.getPosition().x(), bottom + mSize.y() * 0.01f);
  mDescContainer.setSize(mDescContainer.getSize().x(), mSize.y() - mDescContainer.getPosition().y());
}

void DetailedGameListView::DoUpdateGameInformation(bool update)
{
  FileData* file = (mList.size() == 0 || mList.isScrolling()) ? nullptr : mList.getSelected();

  // Reset null game
  if (file == nullptr)
  {
    VideoEngine::Instance().StopVideo(false);
    MoveToFadeOut(getFolderComponents());
    MoveToFadeOut(getGameComponents());
  }
  else
  {
    const bool isFolder = file->IsFolder();
    const bool hasImage = !file->Metadata().Image().IsEmpty();

    if (hasImage && isFolder)
    {
      MoveToFadeOut(getFolderComponents());
      MoveToFadeOut(getGameComponents(false));
      MoveToFadeIn(getScrapedFolderComponents());

      setScrapedFolderInfo(file);
    }
    else
    {
       if (isFolder)
       {
         MoveToFadeIn(getFolderComponents());
         MoveToFadeOut(getGameComponents());

         if (file != mLastCursorItem) ViewController::Instance().FetchSlowDataFor(file);
       }
       else
       {
         MoveToFadeOut(getFolderComponents());
         MoveToFadeIn(getGameComponents());

         setGameInfo(file, update);
       }
    }
  }

  // Reset p2k status (except if the last item is the same
  if (file != mLastCursorItem) mLastCursorItemHasP2K = false;
  mWindow.UpdateHelpSystem();
  // Update p2k status
  if (file != nullptr && file != mLastCursorItem)
  {
    if (file->IsGame())
      ViewController::Instance().FetchSlowDataFor(file);
    else if (file->IsFolder()) // Kill video on multi-thumbnail folder
      mVideo.setVideo(Path::Empty);
  }

  // Update last procesed item
  mLastCursorItem = file;
}

Array<ThemableComponent*> DetailedGameListView::getFolderComponents()
{
  Array<ThemableComponent*> comps;
  for (auto* img: mFolderContent)
  {
    comps.Add(img);
  }
  comps.Add(&mFolderName);
  return comps;
}

Array<ThemableComponent*> DetailedGameListView::getGameComponents(bool includeMainComponents)
{
  Array<ThemableComponent*> comps = getMDValues();
  if (includeMainComponents)
  {
    comps.Add(&mNoImage);
    comps.Add(&mImage);
    comps.Add(&mVideo);
    comps.Add(&mDescription);
  }
  for(TextComponent* component : getMDLabels())
    comps.Add(component);
  return comps;
}

Array<ThemableComponent*> DetailedGameListView::getScrapedFolderComponents()
{
  Array<ThemableComponent*> comps;
  comps.Add(&mNoImage);
  comps.Add(&mImage);
  comps.Add(&mVideo);
  comps.Add(&mDescription);
  return comps;
}

void DetailedGameListView::SetFolderInfo(FolderData* folder, int count, const FolderImagesPath& path)
{
  String gameCount(_N("%i GAME AVAILABLE", "%i GAMES AVAILABLE", count));
  gameCount.Replace("%i", String(count));
  mFolderName.setText(folder->Name() + " - " + gameCount);

  for (int i = sFoldersMaxGameImageCount; --i >= 0;)
    mFolderContent[i]->setImage(path[i]);
}

void DetailedGameListView::SetImageFading(FileData* game, bool videoStillRunning)
{
  if (videoStillRunning)
  {
    MoveToFadeOut(&mImage);
    MoveToFadeOut(&mNoImage);
  }
  else if (game->Metadata().Image().Exists())
  {
    mImage.setImage(game->Metadata().Image());
    MoveToFadeIn(&mImage);
    MoveToFadeOut(&mNoImage);
  }
  else
  {
    MoveToFadeOut(&mImage);
    MoveToFadeIn(&mNoImage);
  }
}

void DetailedGameListView::setGameInfo(FileData* file, bool update)
{
  (void)update;
  setRegions(file);

  MetadataDescriptor& meta = file->Metadata();
  mRating.setValue(meta.RatingAsString());
  if (meta.ReleaseDateEpoc() != 0) mReleaseDate.setValue(meta.ReleaseDate());
  else mReleaseDate.setValue(_("UNKNOWN"));
  mDeveloper.setValue(meta.Developer().empty() ? _("UNKNOWN") : meta.Developer());
  mPublisher.setValue(meta.Publisher().empty() ? _("UNKNOWN") : meta.Publisher());
  mGenre.setValue(meta.GenreId() != GameGenres::None ? Genres::GetFullNameStandalone(meta.GenreId()) :
                  !meta.Genre().empty() ? meta.Genre().Replace(',', ", ", 2).Replace("  ", 2, ' ') :
                  _("NONE"));
  mPlayers.setValue(meta.PlayersAsString());
  mLastPlayed.setValue(meta.LastPlayed());
  mPlayCount.setValue(meta.PlayCountAsString());
  mFavorite.setValue(meta.Favorite() ? _("YES") : _("NO"));

  mBusy.setPosition(mImage.getPosition());
  mBusy.setSize(mImage.getSize());
  mBusy.setOrigin(mImage.getOrigin());

  if (!mSettings.AsBool("system.secondminitft.enabled", false) ||
      !mSettings.AsBool("system.secondminitft.disablevideoines", false))
  {
    mVideo.setVideo(meta.Video(), AudioModeTools::CanDecodeVideoSound());
    { LOG(LogDebug) << "[GamelistView] Set video " << meta.Video().ToString() << " for " << meta.Name() << " => " << file->RomPath().ToString(); }
  }

  SetImageFading(file, mVideo.isDiplayed());

  mDescription.setText(GetDescription(*file));
  mDescContainer.reset();
}

void DetailedGameListView::setScrapedFolderInfo(FileData* file)
{
  SetImageFading(file, false);
  mVideo.setVideo(Path::Empty);
  mDescription.setText(GetDescription(*file));
  mDescContainer.reset();
}

void DetailedGameListView::launch(FileData* game)
{
  VideoEngine::Instance().StopVideo(true);
  mVideo.setVideo(Path::Empty);

  Vector3f target(Renderer::Instance().DisplayWidthAsFloat() / 2.0f, Renderer::Instance().DisplayHeightAsFloat() / 2.0f, 0);
  if (mImage.hasImage())
    target.Set(mImage.getCenter().x(), mImage.getCenter().y(), 0);

  ViewController::Instance().Launch(game, GameLinkedData(), target, false);
}

// element order need to follow the one in onThemeChanged
Array<TextComponent*> DetailedGameListView::getMDLabels()
{
  Array<TextComponent*> ret;
  ret.Add(&mLblRating);
  ret.Add(&mLblReleaseDate);
  ret.Add(&mLblDeveloper);
  ret.Add(&mLblPublisher);
  ret.Add(&mLblGenre);
  ret.Add(&mLblPlayers);
  ret.Add(&mLblLastPlayed);
  ret.Add(&mLblPlayCount);
  ret.Add(&mLblFavorite);
  return ret;
}

// element order need to follow the one in onThemeChanged
Array<ThemableComponent*> DetailedGameListView::getMDValues()
{
  Array<ThemableComponent*> ret;
  ret.Add(&mRating);
  ret.Add(&mReleaseDate);
  ret.Add(&mDeveloper);
  ret.Add(&mPublisher);
  ret.Add(&mGenre);
  ret.Add(&mPlayers);
  ret.Add(&mLastPlayed);
  ret.Add(&mPlayCount);
  ret.Add(&mFavorite);
  return ret;
}

void DetailedGameListView::Update(int deltatime)
{
  // Cache gamelist decoration - TODO: replace by dynamic observable recalboxconf when available
  mDecorations = mSettings.GetSystemGamelistDecoration(mSystem);

  ISimpleGameListView::Update(deltatime);

  mBusy.Enable(mIsScraping);
  mBusy.Update(deltatime);

  // Process fade-in list
  for(Component* component : mFadeInList)
    if (component->getOpacity() != 255)
      component->setOpacity(Math::clampi(component->getOpacity() + deltatime, 0, 255));

  // Process fade-out list
  for(Component* component : mFadeOutList)
    if (component->getOpacity() != 0)
      component->setOpacity(Math::clampi(component->getOpacity() - deltatime, 0, 255));

  // Cancel video
  if (mList.isScrolling())
    mVideo.setVideo(Path::Empty);

  if (!mSystem.IsScreenshots() && !GuiScraperRun::IsRunning())
  {
    // Need busy animation?
    ScraperSeamless& scraper = ScraperSeamless::Instance();
    FileData* game = getCursor();
    mIsScraping = false;
    if (game != nullptr)
      if (game->IsGame())
      {
        // Currently scraping?
        mIsScraping = (scraper.HowLong(game) > sMaxScrapingTimeBeforeBusyAnim);
        // Or start scraping?
        if (mElapsedTimeOnGame >= 0) // Valid timer?
          if (mElapsedTimeOnGame += deltatime; mElapsedTimeOnGame > sMaxHoveringTimeBeforeScraping) // Enough time on game?
          {
            // Shutdown timer for the current game
            mElapsedTimeOnGame = -1;
            // Push game into the seamless scraper
            scraper.Push(game, this);
          }
      }
  }
}

void DetailedGameListView::Render(const Transform4x4f& parentTrans)
{
  Transform4x4f trans = parentTrans * getTransform();

  renderChildren(trans);

  Renderer::SetMatrix(trans);
  //Renderer::DrawRectangle(mBusy.getPosition().x(), mBusy.getPosition().y(), mBusy.getSize().x(), mBusy.getSize().y(), 0x00000080);
  mBusy.Render(trans);
}

void DetailedGameListView::OverlayApply(const Transform4x4f& parentTrans, const Vector2f& position, const Vector2f& size, FileData* const& data, unsigned int& color)
{
  (void)parentTrans;
  (void)color;

  // Right part
  int w = Math::roundi(DetailedGameListView::OverlayGetRightOffset(data));
  if (w != 0 && data->IsGame())
  {
    int drawn = 1;
    int flagHeight = Math::roundi(mList.getFont()->getHeight(1.f));
    int y = (int) (position.y() + ((size.y() - (float) flagHeight) / 2.f));

    // Color shift
    bool fade = data != getCursor();

    // Genre
    if (hasFlag(mDecorations, RecalboxConf::GamelistDecoration::Genre))
    {
      if (data->Metadata().GenreId() != GameGenres::None)
      {
        std::shared_ptr<TextureResource>& genreTexture = mPictogramCaches.GetGenre(data->Metadata().GenreId());
        // Draw
        int x = (int) (position.x() + size.x()) - (mFlagMargin + mFlagWidth) * drawn + mFlagMargin;
        Renderer::DrawTexture(*genreTexture, x, y, mFlagWidth, flagHeight, true,
                              HalfColor(mDecorationGenreTemplate.TopLeftColor(), fade),
                              HalfColor(mDecorationGenreTemplate.TopRightColor(), fade),
                              HalfColor(mDecorationGenreTemplate.BottomRightColor(), fade),
                              HalfColor(mDecorationGenreTemplate.BottomLeftColor(), fade));
      }
      drawn++;
    }

    // Players
    if (hasFlag(mDecorations, RecalboxConf::GamelistDecoration::Players))
    {
      std::shared_ptr<TextureResource>& playerTexture = mPictogramCaches.GetPlayers(data->Metadata().PlayerMin(), data->Metadata().PlayerMax());
      // Draw
      int x = (int) (position.x() + size.x()) - (mFlagMargin + mFlagWidth) * drawn + mFlagMargin;
      Renderer::DrawTexture(*playerTexture, x, y, mFlagWidth, flagHeight, true,
                            HalfColor(mDecorationPlayersTemplate.TopLeftColor(), fade),
                            HalfColor(mDecorationPlayersTemplate.TopRightColor(), fade),
                            HalfColor(mDecorationPlayersTemplate.BottomRightColor(), fade),
                            HalfColor(mDecorationPlayersTemplate.BottomLeftColor(), fade));
      drawn++;
    }

    // Regions
    if (hasFlag(mDecorations, RecalboxConf::GamelistDecoration::Regions))
    {
      for (int r = Regions::RegionPack::sMaxRegions; --r >= 0;)
        if (Regions::GameRegions region = data->Metadata().Region().Regions[r]; region != Regions::GameRegions::Unknown)
        {
          std::shared_ptr<TextureResource>& flagTexture = mPictogramCaches.GetFlag(region);
          // Draw
          int x = (int) (position.x() + size.x()) - (mFlagMargin + mFlagWidth) * drawn + mFlagMargin;
          Renderer::DrawTexture(*flagTexture, x, y, mFlagWidth, flagHeight, true,
                                HalfColor(mDecorationFlagsTemplate.TopLeftColor(), fade),
                                HalfColor(mDecorationFlagsTemplate.TopRightColor(), fade),
                                HalfColor(mDecorationFlagsTemplate.BottomRightColor(), fade),
                                HalfColor(mDecorationFlagsTemplate.BottomLeftColor(), fade));
          drawn++;
        }
    }
  }

  // Left part
  w = Math::roundi(DetailedGameListView::OverlayGetLeftOffset(data));
  if (w != 0 && data->IsHeader())
  {
    switch(mSort)
    {
      case FileSorts::Sorts::FileNameAscending:
      case FileSorts::Sorts::FileNameDescending:
      case FileSorts::Sorts::SystemAscending:
      case FileSorts::Sorts::SystemDescending: break;
      case FileSorts::Sorts::RatingAscending:
      case FileSorts::Sorts::RatingDescending:
      {
        int h = Math::roundi(mList.getFont()->getHeight(1.f));
        int y = (int)(position.y() + ((size.y() - (float) h) / 2.f));
        mHeaderStars.setPosition(0, (float)y);
        mHeaderStars.setValue(1.f);
        mHeaderStars.setColor(0xFF);
        mHeaderStars.Render(parentTrans);
        mHeaderStars.setValue(((HeaderData*)data)->Float());
        mHeaderStars.setColor(mRating.getOriginColor());
        mHeaderStars.Render(parentTrans);
        break;
      }
      case FileSorts::Sorts::TimesPlayedAscending:
      case FileSorts::Sorts::TimesPlayedDescending:
      case FileSorts::Sorts::TotalTimeAscending:
      case FileSorts::Sorts::TotalTimeDescending:
      case FileSorts::Sorts::LastPlayedAscending:
      case FileSorts::Sorts::LastPlayedDescending: break;
      case FileSorts::Sorts::PlayersAscending:
      case FileSorts::Sorts::PlayersDescending:
      {
        int playerMinMax = ((HeaderData*)data)->Int();
        int playerMax = playerMinMax >> 16;
        int playerMin = playerMinMax & 0xFFFF;
        std::shared_ptr<TextureResource>& playersTexture = mPictogramCaches.GetPlayers(playerMin, playerMax);
        // Draw
        int h = Math::roundi(mList.getFont()->getHeight(1.f));
        int y = (int)(position.y() + ((size.y() - (float) h) / 2.f));
        Renderer::DrawTexture(*playersTexture, 0, y, mFlagWidth, h, true, 0xFFFFFFFF);
        break;
      }
      case FileSorts::Sorts::DeveloperAscending:
      case FileSorts::Sorts::DeveloperDescending:
      case FileSorts::Sorts::PublisherAscending:
      case FileSorts::Sorts::PublisherDescending: break;
      case FileSorts::Sorts::GenreAscending:
      case FileSorts::Sorts::GenreDescending:
      {
        GameGenres genre = (GameGenres)((HeaderData*)data)->Int();
        if (genre != GameGenres::None)
        {
          std::shared_ptr<TextureResource>& genreTexture = mPictogramCaches.GetGenre(genre);
          // Draw
          int h = Math::roundi(mList.getFont()->getHeight(1.f));
          int y = (int) (position.y() + ((size.y() - (float) h) / 2.f));
          Renderer::DrawTexture(*genreTexture, 0, y, mFlagWidth, h, true, 0xFFFFFFFF);
        }
        break;
      }
      case FileSorts::Sorts::ReleaseDateAscending:
      case FileSorts::Sorts::ReleaseDateDescending: break;
      case FileSorts::Sorts::RegionAscending:
      case FileSorts::Sorts::RegionDescending:
      {
        Regions::RegionPack pack;
        pack.Pack = ((HeaderData*)data)->Int();
        int drawn = 0;
        int flagHeight = Math::roundi(mList.getFont()->getHeight(1.f));
        int y = (int) (position.y() + ((size.y() - (float) flagHeight) / 2.f));
        for (int r = Regions::RegionPack::sMaxRegions; --r >= 0;)
          if (Regions::GameRegions region = pack.Regions[r]; region != Regions::GameRegions::Unknown)
          {
            std::shared_ptr<TextureResource>& flagTexture = mPictogramCaches.GetFlag(region);
            // Draw
            int x = (int)position.x() + (mFlagMargin + mFlagWidth) * drawn;
            Renderer::DrawTexture(*flagTexture, x, y, mFlagWidth, flagHeight, true,
                                  mDecorationFlagsTemplate.TopLeftColor(),
                                  mDecorationFlagsTemplate.TopRightColor(),
                                  mDecorationFlagsTemplate.BottomRightColor(),
                                  mDecorationFlagsTemplate.BottomLeftColor());
            drawn++;
          }
        break;
      }
      default: break;
    }
  }
}

float DetailedGameListView::OverlayGetLeftOffset(FileData* const& data)
{
  if (data->IsHeader())
    switch(mSort)
    {
      case FileSorts::Sorts::RatingAscending:
      case FileSorts::Sorts::RatingDescending:
      {
        mHeaderStars.setSize(0, mList.getFont()->getHeight(1.f));
        return mHeaderStars.getSize().x() + (float)mFlagMargin;
      }
      case FileSorts::Sorts::PlayersAscending:
      case FileSorts::Sorts::PlayersDescending:
      case FileSorts::Sorts::GenreAscending:
      case FileSorts::Sorts::GenreDescending: return (mFlagWidth + mFlagMargin);
      case FileSorts::Sorts::FileNameAscending:
      case FileSorts::Sorts::FileNameDescending:
      case FileSorts::Sorts::SystemAscending:
      case FileSorts::Sorts::SystemDescending:
      case FileSorts::Sorts::DeveloperAscending:
      case FileSorts::Sorts::DeveloperDescending:
      case FileSorts::Sorts::PublisherAscending:
      case FileSorts::Sorts::PublisherDescending:
      case FileSorts::Sorts::TimesPlayedAscending:
      case FileSorts::Sorts::TimesPlayedDescending:
      case FileSorts::Sorts::TotalTimeAscending:
      case FileSorts::Sorts::TotalTimeDescending:
      case FileSorts::Sorts::LastPlayedAscending:
      case FileSorts::Sorts::LastPlayedDescending:
      case FileSorts::Sorts::ReleaseDateAscending:
      case FileSorts::Sorts::ReleaseDateDescending: break;
      case FileSorts::Sorts::RegionAscending:
      case FileSorts::Sorts::RegionDescending:
      {
        Regions::RegionPack pack;
        pack.Pack = ((HeaderData*)data)->Int();
        int regionCount = pack.Count();
        return (mFlagWidth + mFlagMargin) * regionCount;
      }
      default: break;
    }
  return 0.f;
}

float DetailedGameListView::OverlayGetRightOffset(FileData* const& data)
{
  if (data->IsHeader()) return 0.f;

  int result = 0;
  if (hasFlag(mDecorations, RecalboxConf::GamelistDecoration::Regions))
  {
    int regionCount = data->Metadata().Region().Count();
    result += (mFlagWidth + mFlagMargin) * regionCount;
  }
  if (hasFlag(mDecorations, RecalboxConf::GamelistDecoration::Players))
    result += mFlagWidth + mFlagMargin;
  if (hasFlag(mDecorations, RecalboxConf::GamelistDecoration::Genre))
    result += mFlagWidth + mFlagMargin;
  return (float)result;
}

DetailedGameListView::~DetailedGameListView()
{
  for(int i = (int)mFolderContent.size(); --i >= 0; )
    delete mFolderContent[i];
  mFolderContent.clear();
}

void DetailedGameListView::setRegions(FileData* file)
{
  for (int r = Regions::RegionPack::sMaxRegions; --r >= 0;)
    if (Regions::GameRegions region = file->Metadata().Region().Regions[r]; region != Regions::GameRegions::Unknown)
    {
      std::shared_ptr<TextureResource>& flagTexture = mPictogramCaches.GetFlag(region);
      mRegions[r].setImage(flagTexture);
    }
    else mRegions[r].setImage(Path::Empty);
}

void DetailedGameListView::ScrapingStageCompleted(FileData* game, Stage stage, MetadataType changes)
{
  // Got result, from the seamless scraper, update game data!
  if (game == getCursor())
    switch(stage)
    {
      case Stage::Text:
      {
        DoUpdateGameInformation(false);
        // Game name
        if ((changes & MetadataType::Name) != 0)
          mList.changeTextAt(mList.getCursorIndex(), GetDisplayName(*game));
        break;
      }
      case Stage::Images:
      {
        if ((changes & (MetadataType::Image | MetadataType::Thumbnail)) != 0)
          DoUpdateGameInformation(true);
        break;
      }
      case Stage::Video:
      {
        if ((changes & MetadataType::Video) != 0)
          DoUpdateGameInformation(false);
        break;
      }
      case Stage::Extra: break; // Nothing to do with extra data for now
      case Stage::Completed: RecalboxStorageWatcher::CheckStorageFreeSpace(mWindow, mSystemManager.GetMountMonitor(), game->RomPath()); break;
      default: break;
    }
  else
    if (stage == Stage::Text)
      if ((changes & MetadataType::Name) != 0)
        for(int i = mList.Count(); -- i>= 0; )
          if (mList.getObjectAt(i) == game)
            mList.changeTextAt(i, GetDisplayName(*game));

  { LOG(LogDebug) << "[Scraper] Scraper stage: " << (int)stage; }
}

// Called when a game is selected in the list whatever how
void DetailedGameListView::OnGameSelected()
{
  // Reset seamless scraping timer
  FileData* game = getCursor();
  if (game != nullptr && game->IsGame()) mElapsedTimeOnGame = 0;

  // Update current game information
  DoUpdateGameInformation(false);
}

String DetailedGameListView::getItemIcon(const FileData& item)
{
  // Crossed out eye for hidden things
  if (item.Metadata().Hidden()) return "\uF070 ";
  // System icon, for Favorite games
  if ((item.IsGame()) && (mSystem.IsVirtual() || item.Metadata().Favorite()))
    return item.System().Descriptor().IconPrefix();
  // Open folder for folders
  if (item.IsFolder()) return "\uF07C ";

  return String();
}

String DetailedGameListView::GetDisplayName(FileData& game)
{
  // Select Icon
  String result = getItemIcon(game);
  // Get name
  result.Append(RecalboxConf::Instance().GetDisplayByFileName() ? game.Metadata().RomFileOnly().ToString() : game.Name());
  return result;
}

void DetailedGameListView::populateList(const FolderData& folder)
{
  mPopulatedFolder = &folder;
  mList.clear();
  mHeaderText.setText(mSystem.FullName());

  // Default filter
  FileData::Filter includesFilter = FileData::Filter::Normal | FileData::Filter::Favorite;
  // Favorites only?
  if (RecalboxConf::Instance().GetFavoritesOnly()) includesFilter = FileData::Filter::Favorite;

  // Get items
  bool flatfolders = mSystem.IsAlwaysFlat() || (RecalboxConf::Instance().GetSystemFlatFolders(mSystem));
  FileData::List items;
  if (flatfolders) folder.GetItemsRecursivelyTo(items, includesFilter, mSystem.Excludes(), false);
  else folder.GetItemsTo(items, includesFilter, mSystem.Excludes(), true);

  // Check emptyness
  if (items.empty()) items.push_back(&mEmptyListItem); // Insert "EMPTY SYSTEM" item

  // Sort
  FileSorts::SortSets set = mSystem.IsVirtual() ? FileSorts::SortSets::MultiSystem :
                            mSystem.Descriptor().IsArcade() ? FileSorts::SortSets::Arcade :
                            FileSorts::SortSets::SingleSystem;
  FileSorts::Sorts sort = mSystem.IsSelfSorted() ? mSystem.FixedSort() :
                          FileSorts::Clamp(RecalboxConf::Instance().GetSystemSort(mSystem), set);
  FileSorts& sorts = FileSorts::Instance();
  FolderData::Sort(items, sorts.ComparerFromSort(sort), sorts.IsAscending(sort));
  mSort = sort;

  // Region filtering?
  Regions::GameRegions currentRegion = Regions::Clamp((Regions::GameRegions)RecalboxConf::Instance().GetSystemRegionFilter(mSystem));
  bool activeRegionFiltering = false;
  if (currentRegion != Regions::GameRegions::Unknown)
  {
    Regions::List availableRegion = AvailableRegionsInGames(items);
    // Check if our region is in the available ones
    for(Regions::GameRegions region : availableRegion)
    {
      activeRegionFiltering = (region == currentRegion);
      if (activeRegionFiltering) break;
    }
  }

  // Tate flag
  bool onlyTate = RecalboxConf::Instance().GetShowOnlyTateGames();
  bool onlyYoko = RecalboxConf::Instance().GetShowOnlyYokoGames();

  // Precalculate header alignment
  HorizontalAlignment headerAlignment = mList.Alignment() == HorizontalAlignment::Left
                                      ? HorizontalAlignment::Right : HorizontalAlignment::Left;
  bool leftIcon = headerAlignment == HorizontalAlignment::Left;

  // Add to list
  mList.clear(items.size());
  FileData* previous = nullptr;
  HeaderData* lastHeader = nullptr;

  bool hasTopFavorites = false;
  if (RecalboxConf::Instance().GetFavoritesFirst())
    for (FileData* item : items)
      if (item->Metadata().Favorite()) { hasTopFavorites = true; break; }

  for (FileData* item : items)
  {
    // Region filtering?
    int colorIndexOffset = BaseColor;
    if (activeRegionFiltering)
      if (!Regions::IsIn4Regions(item->Metadata().Region().Pack, currentRegion))
        colorIndexOffset = HighlightColor;
    // Tate filtering
    if (onlyTate && !RotationUtils::IsTate(item->Metadata().Rotation())) continue;
    // Yoko filtering
    if (onlyYoko && RotationUtils::IsTate(item->Metadata().Rotation())) continue;
    // Header?
    if (item->IsGame())
      if (HeaderData* header = NeedHeader(previous, item, hasTopFavorites); header != nullptr)
      {
        mList.add(header->Name(leftIcon), header, GameColor, HeaderColor, headerAlignment);
        lastHeader = header;
      }
    // Store
    if (lastHeader == nullptr || !lastHeader->IsFolded())
      mList.add(GetDisplayName(*item), item, colorIndexOffset + (item->IsFolder() ? FolderColor : GameColor), false);
    if (item->IsGame())
      previous = item;
  }
}

void DetailedGameListView::setCursorIndex(int index)
{
  if (index >= mList.size()) index = mList.size() - 1;
  if (index < 0) index = 0;

  mList.setCursorIndex(index);
}
void DetailedGameListView::setCursorStack(FileData* cursor)
{
  std::stack<FolderData*> reverseCursorStack;

  Path systemTopFolderPath = cursor->TopAncestor().RomPath();
  FolderData* parent = cursor->Parent();

  if (systemTopFolderPath == parent->RomPath())
    return;

  while(systemTopFolderPath != parent->RomPath())
  {
    reverseCursorStack.push(parent);
    parent = parent->Parent();
  }

  while(!reverseCursorStack.empty())
  {
    mCursorStack.push(reverseCursorStack.top());
    reverseCursorStack.pop();

    FolderData& tmp = !mCursorStack.empty() ? *mCursorStack.top() : mSystem.MasterRoot();
    populateList(tmp);
  }
}

void DetailedGameListView::setCursor(FileData* cursor)
{
  if(!mList.setCursor(cursor, 0))
  {
    populateList(mSystem.MasterRoot());
    mList.setCursor(cursor);

    // update our cursor stack in case our cursor just got set to some folder we weren't in before
    if(mCursorStack.empty() || mCursorStack.top() != cursor->Parent())
    {
      std::stack<FolderData*> tmp;
      FolderData* ptr = cursor->Parent();
      while((ptr != nullptr) && !ptr->IsRoot())
      {
        tmp.push(ptr);
        ptr = ptr->Parent();
      }

      // flip the stack and put it in mCursorStack
      mCursorStack = std::stack<FolderData*>();
      while(!tmp.empty())
      {
        mCursorStack.push(tmp.top());
        tmp.pop();
      }
    }
  }
}

void DetailedGameListView::removeEntry(FileData* fileData)
{
  FileData::TopLevelFilter filter = FileData::BuildTopLevelFilter();
  if (!mCursorStack.empty() && !fileData->Parent()->HasVisibleGame(filter))
  {
    // remove current folder from stack
    mCursorStack.pop();

    FolderData& cursor = !mCursorStack.empty() ? *mCursorStack.top() : mSystem.MasterRoot();
    populateList(cursor);
  }

  int cursorIndex = getCursorIndex();
  refreshList();
  if(cursorIndex > 0) setCursorIndex(cursorIndex - 1);
}

Regions::List DetailedGameListView::AvailableRegionsInGames()
{
  bool regionIndexes[256];
  memset(regionIndexes, 0, sizeof(regionIndexes));
  // Run through all games
  for(int i = (int)mList.size(); --i >= 0; )
  {
    const FileData& fd = *mList.getObjectAt(i);
    unsigned int fourRegions = fd.Metadata().Region().Pack;
    // Set the 4 indexes corresponding to all 4 regions (Unknown regions will all point to index 0)
    regionIndexes[(fourRegions >>  0) & 0xFF] = true;
    regionIndexes[(fourRegions >>  8) & 0xFF] = true;
    regionIndexes[(fourRegions >> 16) & 0xFF] = true;
    regionIndexes[(fourRegions >> 24) & 0xFF] = true;
  }
  // Rebuild final list
  Regions::List list;
  for(int i = 0; i < (int)sizeof(regionIndexes); ++i )
    if (regionIndexes[i])
      list.push_back((Regions::GameRegions)i);
  // Only unknown region?
  if (list.size() == 1 && regionIndexes[0])
    list.clear();
  return list;
}

Regions::List DetailedGameListView::AvailableRegionsInGames(FileData::List& fdList)
{
  bool regionIndexes[256];
  memset(regionIndexes, 0, sizeof(regionIndexes));
  // Run through all games
  for(const FileData* fd : fdList)
  {
    unsigned int fourRegions = fd->Metadata().Region().Pack;
    // Set the 4 indexes corresponding to all 4 regions (Unknown regions will all point to index 0)
    regionIndexes[(fourRegions >>  0) & 0xFF] = true;
    regionIndexes[(fourRegions >>  8) & 0xFF] = true;
    regionIndexes[(fourRegions >> 16) & 0xFF] = true;
    regionIndexes[(fourRegions >> 24) & 0xFF] = true;
  }
  // Rebuild final list
  Regions::List list;
  for(int i = (int)sizeof(regionIndexes); --i >= 0; )
    if (regionIndexes[i])
      list.push_back((Regions::GameRegions)i);
  // Only unknown region?
  if (list.size() == 1 && regionIndexes[0])
    list.clear();
  return list;
}

void DetailedGameListView::RefreshItem(FileData* game)
{
  if (game == nullptr || !game->IsGame()) { LOG(LogError) << "[DetailedGameListView] Trying to refresh null or empty item"; return; }

  int index = mList.Lookup(game);
  if (index < 0) { LOG(LogError) << "[DetailedGameListView] Trying to refresh a not found item"; return; }
  mList.changeTextAt(index, GetDisplayName(*game));
  if (mList.getCursorIndex() == index) DoUpdateGameInformation(true);
}

void DetailedGameListView::UpdateSlowData(const SlowDataInformation& info)
{
  if (info.mItem == getCursor())
  {
    // Game? update p2k status
    if (info.mItem->IsGame())
    {
      mLastCursorItemHasP2K = info.mHasP2k;
      mWindow.UpdateHelpSystem();
    }
    else if (info.mItem->IsFolder())
      SetFolderInfo((FolderData*)info.mItem, info.mCount, *info.mPathList);
  }
}

void DetailedGameListView::VideoComponentRequireAction(const VideoComponent* source, IVideoComponentAction::Action action)
{
  if (source == &mVideo)
    switch(action)
    {
      case Action::FadeIn:
      {
        MoveToFadeIn(mVideoLinks);
        SetImageFading(mLastCursorItem, false);
        break;
      }
      case Action::FadeOut:
      {
        MoveToFadeOut(mVideoLinks);
        break;
      }
      default: break;
    }
}

void DetailedGameListView::BuildVideoLinks(const ThemeData& theme)
{
  mVideoLinks.Clear();
  const ThemeElement* elem = theme.Element(getName(), "md_video", ThemeElementType::Video, ThemeElementType::None);
  if (elem != nullptr)
    if (elem->HasProperty(ThemePropertyName::Link))
      for(String& link : elem->AsString(ThemePropertyName::Link).Split(','))
      {
        link.Trim();
        if (link == "md_image") { mVideoLinks.Add(&mImage); mVideoLinks.Add(&mNoImage); }
        else if (Component* comp = mThemeExtras.Lookup(link); comp != nullptr) mVideoLinks.Add(comp);
      }
}

HeaderData* DetailedGameListView::NeedHeader(FileData* previous, FileData* next, bool hasTopFavorites)
{
  // Favorites
  if (hasTopFavorites)
  {
    if (next->Metadata().Favorite())
    {
      if (previous == nullptr || (!previous->Metadata().Favorite() && next->Metadata().Favorite()))
        return GetHeader(_("Favorites"));
      return nullptr;
    }
    // Leaving favorite zone: reset prevous has if it is the first encountered game
    else if (previous != nullptr && previous->Metadata().Favorite())
      previous = nullptr;
  }

  // Normal processing
  switch(mSort)
  {
    case FileSorts::Sorts::FileNameAscending:
    case FileSorts::Sorts::FileNameDescending:
    {
      if (hasTopFavorites)
      {
        // Leavinf favorite area
        if (previous != nullptr && previous->Metadata().Favorite() && !next->Metadata().Favorite())
          return GetHeader(_("In alphabetical order"));
        // List has favorites but not at top
        else if (previous == nullptr)
          return GetHeader(_("In alphabetical order"));
      }
      break;
    }
    case FileSorts::Sorts::SystemAscending:
    case FileSorts::Sorts::SystemDescending:
    {
      if (previous == nullptr || &previous->System() != &next->System())
        if (previous == nullptr || previous->System().SortingName() != next->System().SortingName())
        return GetHeader(next->System().SortingFullName());
      break;
    }
    case FileSorts::Sorts::RatingAscending:
    case FileSorts::Sorts::RatingDescending:
    {
      int previousRating = previous == nullptr ? -1 : Math::round(previous->Metadata().Rating() * 10.f);
      int nextRating = Math::round(next->Metadata().Rating() * 10.f);
      if (previousRating != nextRating)
        return GetHeader(nextRating != 0 ? (_F(_("RATED {0} / 10")) / nextRating)() : _("NOT RATED"), next->Metadata().Rating());
      break;
    }
    case FileSorts::Sorts::TotalTimeAscending:
    case FileSorts::Sorts::TotalTimeDescending:
    {
      int previousRange = -1;
      int nextRange = 0;
      // Get previous range
      if (previous == nullptr)
      {
        if (previous->Metadata().TimePlayed() == 0) previousRange = 0;
        else if (TimeSpan span(previous->Metadata().TimePlayed(), 0); span.TotalMinutes() < 30) previousRange = 1;
        else { int hours = (int)span.TotalHours(); previousRange = hours == 0 ? 2 : hours + 2; }
      }
      // Get next range
      if (next->Metadata().TimePlayed() == 0) nextRange = 0;
      else if (TimeSpan span(next->Metadata().TimePlayed(), 0); span.TotalMinutes() < 30) nextRange = 1;
      else { int hours = (int)span.TotalHours(); nextRange = hours == 0 ? 2 : hours + 2; }

      // Get header
      if (previousRange != nextRange)
      {
        String text;
        if (nextRange == 0) text = _("Never played");
        else if (nextRange == 1) text = _("Just a few minutes");
        else if (nextRange == 2) text = _("Less than an hour");
        else text = (_F(_("{0} hours")) / (nextRange - 2)).ToString();
        return GetHeader(text);
      }
      break;
    }
    case FileSorts::Sorts::TimesPlayedAscending:
    case FileSorts::Sorts::TimesPlayedDescending:
    {
      if (previous == nullptr || previous->Metadata().TimePlayed() != next->Metadata().TimePlayed())
      {
        String text;
        if (next->Metadata().TimePlayed() == 0) text = _("Never played");
        else text = (_F(_("{0} times")) / next->Metadata().TimePlayed())();
        return GetHeader(text);
      }
      break;
    }
    case FileSorts::Sorts::LastPlayedAscending:
    case FileSorts::Sorts::LastPlayedDescending:
    {
      DateTime now;
      String previousTime = previous != nullptr ? GetSimplifiedElapsedTime(now - previous->Metadata().LastPlayed()) : String::Empty;
      String nextTime = GetSimplifiedElapsedTime(now - next->Metadata().LastPlayed());
      if (previousTime != nextTime)
        return GetHeader(nextTime);
      break;
    }
    case FileSorts::Sorts::PlayersAscending:
    case FileSorts::Sorts::PlayersDescending:
    {
      if (previous == nullptr || previous->Metadata().PlayerRange() != next->Metadata().PlayerRange())
      {
        bool monoPlayer = next->Metadata().PlayerMin() == next->Metadata().PlayerMax() &&
                          next->Metadata().PlayerMin() == 1;
        return GetHeader(monoPlayer ? _("One player") : (_F(_("{0} Players")) / next->Metadata().PlayersAsString())(),
                         (next->Metadata().PlayerMax() << 16) | next->Metadata().PlayerMin());
      }
      break;
    }
    case FileSorts::Sorts::DeveloperAscending:
    case FileSorts::Sorts::DeveloperDescending:
    {
      String prevString = previous != nullptr ? previous->Metadata().Developer().Trim().ToLowerCase() : "\t";
      String nextString = next->Metadata().Developer().Trim().ToLowerCase();
      if (prevString != nextString)
      {
        String developer(next->Metadata().Developer());
        if (developer.empty()) developer = _("Unknown developer");
        return GetHeader(developer);
      }
      break;
    }
    case FileSorts::Sorts::PublisherAscending:
    case FileSorts::Sorts::PublisherDescending:
    {
      String prevString = previous != nullptr ? previous->Metadata().Publisher().Trim().ToLowerCase() : "\t";
      String nextString = next->Metadata().Publisher().Trim().ToLowerCase();
      if (prevString != nextString)
      {
        String publisher(next->Metadata().Publisher());
        if (publisher.empty()) publisher = _("Unknown publisher");
        return GetHeader(publisher);
      }
      break;
    }
    case FileSorts::Sorts::GenreAscending:
    case FileSorts::Sorts::GenreDescending:
    {
      if (previous == nullptr || previous->Metadata().GenreId() != next->Metadata().GenreId())
        return GetHeader(Genres::GetFullNameStandalone(next->Metadata().GenreId()), (int)next->Metadata().GenreId());
      break;
    }
    case FileSorts::Sorts::ReleaseDateAscending:
    case FileSorts::Sorts::ReleaseDateDescending:
    {
      int year = next->Metadata().ReleaseDate().Year();
      if (previous == nullptr || previous->Metadata().ReleaseDate().Year() != year)
        return GetHeader(year <= 1970 ? _("Release date unknown") : (_F(_("Year {0}")) / year)());
      break;
    }
    case FileSorts::Sorts::RegionAscending:
    case FileSorts::Sorts::RegionDescending:
    {
      Regions::RegionPack regions = next->Metadata().Region();
      if (previous == nullptr || previous->Metadata().Region() != regions)
      {
        String header;
        for(int i = Regions::RegionPack::sMaxRegions; --i >= 0; )
          if (Regions::GameRegions region = regions.Regions[i]; region != Regions::GameRegions::Unknown)
          {
            if (!header.empty()) header.Append(", ", 2);
            header.Append(Regions::RegionFullName(region));
          }
        if (header.empty()) header = _("NO REGION");
        return GetHeader(header, regions.Pack);
      }
      break;
    }
  }
  return nullptr;
}

HeaderData* DetailedGameListView::GetHeader(const String& name)
{
  HeaderData** header = mHeaderMap.try_get(name);
  if (header != nullptr) return *header;

  return mHeaderMap[name] = new HeaderData(name, mSystem);
}

HeaderData* DetailedGameListView::GetHeader(const String& name, int data)
{
  HeaderData** header = mHeaderMap.try_get(name);
  if (header != nullptr) return *header;

  return mHeaderMap[name] = new HeaderData(name, mSystem, data, (float)data);
}

HeaderData* DetailedGameListView::GetHeader(const String& name, float data)
{
  HeaderData** header = mHeaderMap.try_get(name);
  if (header != nullptr) return *header;

  return mHeaderMap[name] = new HeaderData(name, mSystem, (int)data, data);
}

bool DetailedGameListView::ProcessInput(const InputCompactEvent& event)
{
  // Only in non alphabetic sorts
  if (mSort != FileSorts::Sorts::FileNameAscending && mSort != FileSorts::Sorts::FileNameDescending)
  {
    if (event.ValidReleased())
    {
      if (FileData* item = getCursor(); item->IsHeader())
      {
        ((HeaderData*)item)->ToogleFolded();
        populateList(*mPopulatedFolder);
        setCursor(item);
        return true;
      }
    }

    if (event.AnyHotkeyCombination())
    {
      if (event.HotkeyL1Released()) { MoveToHeader(false); return true; }
      if (event.HotkeyR1Released()) { MoveToHeader(true); return true; }
    }
  }
  // Change sort anywhere
  if (event.AnyHotkeyCombination())
  {
    if (event.HotkeyL2Released()) { ChangeSort(false); return true; }
    if (event.HotkeyR2Released()) { ChangeSort(true); return true; }
  }

  return ISimpleGameListView::ProcessInput(event);
}

/*void DetailedGameListView::FoldAll()
{
  // Lookup current header
  FileData* item = getCursor();
  for(int i = mList.getCursorIndex(); --i >= 0;)
    if (mList.getObjectAt(i)->IsHeader())
    {
      item = mList.getObjectAt(i);
      break;
    }

  // Fold all
  for(int i =  mList.Count(); --i >= 0;)
    if (mList.getObjectAt(i)->IsHeader())
      ((HeaderData*)mList.getObjectAt(i))->SetFolded(true);

  // Rebuild the UI list
  populateList(*mPopulatedFolder);

  // Set cursor
  setCursor(item);
}

void DetailedGameListView::UnfoldAll()
{
  // Get cursor position
  FileData* item = getCursor();

  // Fold all
  for(int i =  mList.Count(); --i >= 0;)
    if (mList.getObjectAt(i)->IsHeader())
      ((HeaderData*)mList.getObjectAt(i))->SetFolded(false);

  // Rebuild the UI list
  populateList(*mPopulatedFolder);

  // Set cursor
  setCursor(item);
}

void DetailedGameListView::Fold()
{
  // Lookup current header
  HeaderData* header = nullptr;
  for(int i = mList.getCursorIndex(); --i >= 0;)
    if (mList.getObjectAt(i)->IsHeader())
    {
      header = (HeaderData*)mList.getObjectAt(i);
      break;
    }

  // Fold header
  if (header != nullptr && !header->IsFolded())
  {
    header->SetFolded(true);

    // Rebuild the UI list
    populateList(*mPopulatedFolder);

    // Set cursor
    setCursor(header);
  }
}

void DetailedGameListView::Unfold()
{
  // Get cursor position
  FileData* item = getCursor();
  if (item->IsHeader())
  {
    HeaderData* header = (HeaderData*)mList.getObjectAt(getCursorIndex());
    if (header->IsFolded())
    {
      header->SetFolded(false);

      // Rebuild the UI list
      populateList(*mPopulatedFolder);

      // Set cursor
      setCursor(header);
    }
  }
}
*/

void DetailedGameListView::MoveToHeader(bool next)
{
  int direction = next ? 1 : -1;
  int baseIndex = mList.getCursorIndex();
  for(int index = baseIndex + direction; index != baseIndex; )
  {
    if (index < 0) index = mList.Count() - 1;
    if (index >= mList.Count()) index = 0;
    if (mList.getObjectAt(index)->IsHeader())
    {
      setCursorIndex(index);
      break;
    }
    index += direction;
  }
}

void DetailedGameListView::ChangeSort(bool next)
{
  FileSorts::SortSets set = mSystem.IsVirtual() ? FileSorts::SortSets::MultiSystem :
                            mSystem.Descriptor().IsArcade() ? FileSorts::SortSets::Arcade :
                            FileSorts::SortSets::SingleSystem;
  const FileSorts::SortList sorts = FileSorts::AvailableSorts(set);

  // Move sort
  int index = sorts.IndexOf(mSort);
  if (index < 0) index = 0;
  index += next ? 1 : -1;
  if (index < 0) index += sorts.Count();
  if (index >= sorts.Count()) index -= sorts.Count();
  mSort = sorts[index];

  // Save new sort
  RecalboxConf::Instance().SetSystemSort(mSystem, mSort).Save();

  // Refresh
  FileData* item = getCursor();
  populateList(*mPopulatedFolder);
  setCursor(item);

  // Notify
  String message = (_F(_("Game are now sorted\nby {0}")) / FileSorts::Instance().Name(mSort))();
  mWindow.InfoPopupAddRegular(message, 10, PopupType::Recalbox, false);
}

String DetailedGameListView::GetSimplifiedElapsedTime(const TimeSpan& span)
{
  if (span.TotalDays() > (365 * 20)) return _("Never played");
  if (span.TotalDays() > 365) return (_F(_("{0} Years ago...")) / (span.TotalDays() / 365))();
  if (span.TotalDays() >= 30) return (_F(_("{0} Month ago...")) / (span.TotalDays() / 30))();
  if (span.TotalDays() > 0) return (_F(_("{0} Days ago...")) / span.TotalDays())();
  if (span.TotalHours() > 0) return (_F(_("{0} Hours ago...")) / span.TotalHours())();
  return _("A short while ago");
}

void DetailedGameListView::ReturnedFromGame(FileData* game)
{
  (void)game;
  switch(mSort)
  {
    case FileSorts::Sorts::TimesPlayedAscending:
    case FileSorts::Sorts::TimesPlayedDescending:
    case FileSorts::Sorts::TotalTimeAscending:
    case FileSorts::Sorts::TotalTimeDescending:
    case FileSorts::Sorts::LastPlayedAscending:
    case FileSorts::Sorts::LastPlayedDescending:
    {
      // Refresh to reflect time played/last played
      FileData* item = getCursor();
      populateList(*mPopulatedFolder);
      setCursor(item);
      break;
    }
    case FileSorts::Sorts::FileNameAscending:
    case FileSorts::Sorts::FileNameDescending:
    case FileSorts::Sorts::SystemAscending:
    case FileSorts::Sorts::SystemDescending:
    case FileSorts::Sorts::RatingAscending:
    case FileSorts::Sorts::RatingDescending:
    case FileSorts::Sorts::PlayersAscending:
    case FileSorts::Sorts::PlayersDescending:
    case FileSorts::Sorts::DeveloperAscending:
    case FileSorts::Sorts::DeveloperDescending:
    case FileSorts::Sorts::PublisherAscending:
    case FileSorts::Sorts::PublisherDescending:
    case FileSorts::Sorts::GenreAscending:
    case FileSorts::Sorts::GenreDescending:
    case FileSorts::Sorts::ReleaseDateAscending:
    case FileSorts::Sorts::ReleaseDateDescending:
    case FileSorts::Sorts::RegionAscending:
    case FileSorts::Sorts::RegionDescending:
    default: break;
  }
}
