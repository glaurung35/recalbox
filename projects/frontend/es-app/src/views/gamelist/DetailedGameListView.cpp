#include <RecalboxConf.h>
#include <VideoEngine.h>
#include "views/gamelist/DetailedGameListView.h"
#include "animations/LambdaAnimation.h"
#include "scraping/ScraperSeamless.h"
#include "recalbox/RecalboxStorageWatcher.h"

DetailedGameListView::DetailedGameListView(WindowManager&window, SystemManager& systemManager, SystemData& system, const IGlobalVariableResolver& resolver)
  : ISimpleGameListView(window, systemManager, system, resolver)
  , mEmptyListItem(&system)
  , mPopulatedFolder(nullptr)
  , mList(window)
  , mElapsedTimeOnGame(0)
  , mIsScraping(false)
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
  , mDescContainer(window)
  , mDescription(window)
  , mBusy(window)
  , mSettings(RecalboxConf::Instance())
  , mLastCursorItem(nullptr)
  , mLastCursorItemHasP2K(false)
{
}

void DetailedGameListView::Initialize()
{
  addChild(&mList);

  mEmptyListItem.Metadata().SetName(_("YOUR LIST IS EMPTY. PRESS START TO CHANGE GAME FILTERS."));
  populateList(mSystem.MasterRoot());

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

  // no image
  mNoImage.setOrigin(mImage.getOrigin());
  mNoImage.setPosition(mImage.getPosition());
  mNoImage.setResize(mImage.getSize());
  mNoImage.setKeepRatio(true);
  mNoImage.setDefaultZIndex(30);
  //mNoImage.setImage(Path(":/no_image.png"));

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
    mRegions[i].setThemeDisabled(true);
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
  mList.setColor(2, (mList.Color(0) & 0xFFFFFF00) | ((mList.Color(0) & 0xFF) >> 1));
  mList.setColor(3, (mList.Color(1) & 0xFFFFFF00) | ((mList.Color(1) & 0xFF) >> 1));
  sortChildren();

  for (int i = 0; i < (int)Regions::RegionPack::sMaxRegions; i++)
    mRegions[i].DoApplyThemeElement(theme, getName(), String("md_region").Append(i + 1).c_str(),
                                    ThemePropertyCategory::Position | ThemePropertyCategory::Size | ThemePropertyCategory::ZIndex | ThemePropertyCategory::Path);

  mImage.DoApplyThemeElement(theme, getName(), "md_image", ThemePropertyCategory::All ^ ThemePropertyCategory::Path);
  mNoImage.DoApplyThemeElement(theme, getName(), "md_image", ThemePropertyCategory::All ^ ThemePropertyCategory::Path);
  mNoImage.setImage(Path(":/no_image.png"));
  mNoImage.DoApplyThemeElement(theme, getName(), "default_image_path", ThemePropertyCategory::Path);
  mNoImage.setThemeDisabled(false);
  mVideo.DoApplyThemeElement(theme, getName(), "md_video", ThemePropertyCategory::All ^ ThemePropertyCategory::Path);

  BuildVideoLinks(theme);

  initMDLabels();
  std::vector<TextComponent*> labels = getMDLabels();
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

  assert(names.size() == labels.size());
  for (unsigned int i = 0; i < (unsigned int)labels.size(); i++)
  {
    labels[i]->DoApplyThemeElement(theme, getName(), names[i], ThemePropertyCategory::All);
  }

  initMDValues();
  std::vector<ThemableComponent*> values = getMDValues();
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
  values.push_back(&mFolderName);

  assert(names.size() == values.size());
  for (unsigned int i = 0; i < (unsigned int)values.size(); i++)
  {
    values[i]->DoApplyThemeElement(theme, getName(), names[i], ThemePropertyCategory::All ^ ThemePropertyCategory::Text);
  }

  mDescContainer.DoApplyThemeElement(theme, getName(), "md_description", ThemePropertyCategory::Position | ThemePropertyCategory::Size | ThemePropertyCategory::ZIndex);
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
  std::vector<TextComponent*> components = getMDLabels();

  //const unsigned int colCount = 2;
  const unsigned int rowCount = (unsigned int) (components.size() / 2);

  Vector3f start(mSize.x() * 0.01f, mSize.y() * 0.625f, 0.0f);

  //const float colSize = (mSize.x() * 0.48f) / colCount;
  const float rowPadding = 0.01f * mSize.y();

  Vector3f pos = start + Vector3f(0, 0, 0);
  for (unsigned int i = 0; i < (unsigned int)components.size(); i++)
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
  std::vector<TextComponent*> labels = getMDLabels();
  std::vector<ThemableComponent*> values = getMDValues();

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
  for (unsigned int i = 0; i < (unsigned int)labels.size(); i++)
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
    for(Component* component : getFolderComponents()) MoveToFadeOut(component);
    for(Component* component : getGameComponents()) MoveToFadeOut(component);
  }
  else
  {
    const bool isFolder = file->IsFolder();
    const bool hasImage = !file->Metadata().Image().IsEmpty();

    if (hasImage && isFolder)
    {
      setScrapedFolderInfo(file);
      for(Component* component : getFolderComponents()) MoveToFadeOut(component);
      for(Component* component : getGameComponents(false)) MoveToFadeOut(component);
      for(Component* component : getScrapedFolderComponents()) MoveToFadeIn(component);
    }
    else
    {
       if (isFolder)
       {
         for(Component* component : getFolderComponents()) MoveToFadeIn(component);
         for(Component* component : getGameComponents()) MoveToFadeOut(component);

         if (file != mLastCursorItem) ViewController::Instance().FetchSlowDataFor(file);
       }
       else
       {
         for(Component* component : getFolderComponents()) MoveToFadeOut(component);
         for(Component* component : getGameComponents()) MoveToFadeIn(component);

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
      mVideo.setVideo(Path::Empty, 0, 0);
  }

  // Update last procesed item
  mLastCursorItem = file;
}

std::vector<ThemableComponent*> DetailedGameListView::getFolderComponents()
{
  std::vector<ThemableComponent*> comps;
  for (auto* img: mFolderContent)
  {
    comps.push_back(img);
  }
  comps.push_back(&mFolderName);
  return comps;
}

std::vector<ThemableComponent*> DetailedGameListView::getGameComponents(bool includeMainComponents)
{
  std::vector<ThemableComponent*> comps = getMDValues();
  if (includeMainComponents)
  {
    comps.push_back(&mNoImage);
    comps.push_back(&mImage);
    comps.push_back(&mVideo);
    comps.push_back(&mDescription);
  }
  std::vector<TextComponent*> labels = getMDLabels();
  comps.insert(comps.end(), labels.begin(), labels.end());
  return comps;
}

std::vector<ThemableComponent*> DetailedGameListView::getScrapedFolderComponents()
{
  std::vector<ThemableComponent*> comps;
  comps.push_back(&mNoImage);
  comps.push_back(&mImage);
  comps.push_back(&mVideo);
  comps.push_back(&mDescription);
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

  int videoDelay = (int) mSettings.AsUInt("emulationstation.videosnaps.delay", VideoComponent::DEFAULT_VIDEODELAY);
  int videoLoop  = (int) mSettings.AsUInt("emulationstation.videosnaps.loop", VideoComponent::DEFAULT_VIDEOLOOP);

  mBusy.setPosition(mImage.getPosition());
  mBusy.setSize(mImage.getSize());
  mBusy.setOrigin(mImage.getOrigin());

  if (!mSettings.AsBool("system.secondminitft.enabled", false) ||
      !mSettings.AsBool("system.secondminitft.disablevideoines", false))
  {
    mVideo.setVideo(meta.Video(), videoDelay, videoLoop, AudioModeTools::CanDecodeVideoSound());
    { LOG(LogDebug) << "[GamelistView] Set video " << meta.Video().ToString() << " for " << meta.Name() << " => " << file->RomPath().ToString(); }
  }

  SetImageFading(file, mVideo.isDiplayed());

  mDescription.setText(GetDescription(*file));
  mDescContainer.reset();
}

void DetailedGameListView::setScrapedFolderInfo(FileData* file)
{
  SetImageFading(file, false);
  mVideo.setVideo(Path::Empty, 0, 0);
  mDescription.setText(GetDescription(*file));
  mDescContainer.reset();
}

void DetailedGameListView::launch(FileData* game)
{
  VideoEngine::Instance().StopVideo(true);
  mVideo.setVideo(Path::Empty, 0, 0);

  Vector3f target(Renderer::Instance().DisplayWidthAsFloat() / 2.0f, Renderer::Instance().DisplayHeightAsFloat() / 2.0f, 0);
  if (mImage.hasImage())
    target.Set(mImage.getCenter().x(), mImage.getCenter().y(), 0);

  ViewController::Instance().Launch(game, GameLinkedData(), target, false);
}

// element order need to follow the one in onThemeChanged
std::vector<TextComponent*> DetailedGameListView::getMDLabels()
{
  std::vector<TextComponent*> ret;
  ret.push_back(&mLblRating);
  ret.push_back(&mLblReleaseDate);
  ret.push_back(&mLblDeveloper);
  ret.push_back(&mLblPublisher);
  ret.push_back(&mLblGenre);
  ret.push_back(&mLblPlayers);
  ret.push_back(&mLblLastPlayed);
  ret.push_back(&mLblPlayCount);
  ret.push_back(&mLblFavorite);
  return ret;
}

// element order need to follow the one in onThemeChanged
std::vector<ThemableComponent*> DetailedGameListView::getMDValues()
{
  std::vector<ThemableComponent*> ret;
  ret.push_back(&mRating);
  ret.push_back(&mReleaseDate);
  ret.push_back(&mDeveloper);
  ret.push_back(&mPublisher);
  ret.push_back(&mGenre);
  ret.push_back(&mPlayers);
  ret.push_back(&mLastPlayed);
  ret.push_back(&mPlayCount);
  ret.push_back(&mFavorite);
  return ret;
}

void DetailedGameListView::Update(int deltatime)
{
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
    mVideo.setVideo(Path::Empty, 0, 0);

  if (!mSystem.IsScreenshots())
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

void DetailedGameListView::OverlayApply(const Vector2f& position, const Vector2f& size, FileData* const& data, unsigned int& color)
{
  (void)color;
  int w = Math::roundi(DetailedGameListView::OverlayGetRightOffset(data));
  if (w != 0)
  {
    int drawn = 1;
    int flagHeight = Math::roundi(mList.getFont()->getHeight(1.f));

    for (int r = Regions::RegionPack::sMaxRegions; --r >= 0;)
      if (Regions::GameRegions region = data->Metadata().Region().Regions[r]; region != Regions::GameRegions::Unknown)
      {
        std::shared_ptr<TextureResource>* flag = mRegionToTextures.try_get(region);
        if (flag == nullptr)
        {
          // Load flag
          std::shared_ptr<TextureResource> texture = TextureResource::get(Path(":/regions/" + Regions::GameRegionsFromEnum(region) + ".svg"), false, true, true);
          mRegionToTextures.insert(region, texture);
          flag = mRegionToTextures.try_get(region);
        }
        // Draw
        int flagWidth = (int) ((float) flagHeight * (float) (*flag)->width() / (float) (*flag)->height());
        int y = Math::roundi((size.y() - (float) flagHeight) / 2.f) + (int)position.y();
        int x = ((int)size.x() - (2 + Math::roundi(mList.getHorizontalMargin()) + flagWidth) * drawn) + (int)position.x();
        Renderer::DrawTexture(**flag, x, y, flagWidth, flagHeight, data == getCursor() ? (unsigned char)255 : (unsigned char)128);
        drawn++;
      }
  }
}

float DetailedGameListView::OverlayGetRightOffset(FileData* const& data)
{
  return ((mList.getFont()->getHeight(1.f) * (71.f / 48.f) + 2.f) * (float)data->Metadata().Region().Count()) + 2.f + mList.getHorizontalMargin();
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
      std::shared_ptr<TextureResource>* flag = mRegionToTextures.try_get(region);
      if (flag == nullptr)
      {
        // Load flag
        std::shared_ptr<TextureResource> texture = TextureResource::get(Path(":/regions/" + Regions::GameRegionsFromEnum(region) + ".svg"), false, true, true);
        mRegionToTextures.insert(region, texture);
        flag = mRegionToTextures.try_get(region);
      }
      mRegions[r].setImage(*flag);
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
  FolderData::Sort(items, FileSorts::Comparer(sort), FileSorts::IsAscending(sort));

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

  // Add to list
  //mList.reserve(items.size()); // TODO: Reserve memory once
  for (FileData* fd : items)
  {
    // Region filtering?
    int colorIndexOffset = 0;
    if (activeRegionFiltering)
      if (!Regions::IsIn4Regions(fd->Metadata().Region().Pack, currentRegion))
        colorIndexOffset = 2;
    // Tate filtering
    if (onlyTate && !RotationUtils::IsTate(fd->Metadata().Rotation())) continue;
    // Yoko filtering
    if (onlyYoko && RotationUtils::IsTate(fd->Metadata().Rotation())) continue;
    // Store
    mList.add(GetDisplayName(*fd), fd, colorIndexOffset + (fd->IsFolder() ? 1 : 0), false);
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
      case Action::FadeIn: SetImageFading(mLastCursorItem, false); break;
      case Action::FadeOut: MoveToFadeOut(&mImage); MoveToFadeOut(&mNoImage); break;
      default: break;
    }
}

void DetailedGameListView::BuildVideoLinks(const ThemeData& theme)
{
  mVideoLinks.Clear();
  const ThemeElement* elem = theme.Element(getName(), "md_video", ThemeElementType::Video);
  if (elem != nullptr)
    if (elem->HasProperty(ThemePropertyName::Link))
      for(String& link : elem->AsString(ThemePropertyName::Link).Split(','))
      {
        link.Trim();
        if (link == "md_image") mVideoLinks.Add(&mImage);
        else if (Component* comp = mThemeExtras.Lookup(link); comp != nullptr) mVideoLinks.Add(comp);
      }
}
