#include "views/SystemView.h"
#include "views/ViewController.h"
#include "animations/LambdaAnimation.h"
#include <guis/GuiMsgBox.h>
#include <RecalboxConf.h>
#include <guis/GuiNetPlay.h>
#include <systems/SystemManager.h>
#include <guis/menus/GuiMenuQuit.h>
#include <usernotifications/NotificationManager.h>
#include "guis/menus/GuiMenu.h"
#include "audio/AudioManager.h"
#include <guis/GuiSearch.h>
#include <guis/menus/GuiMenuSwitchKodiNetplay.h>
#include <emulators/run/GameRunner.h>
#include "MenuFilter.h"

// buffer values for scrolling velocity (left, stopped, right)
const int logoBuffersLeft[] = { -5, -2, -1 };
const int logoBuffersRight[] = { 1, 2, 5 };

SystemView::SystemView(WindowManager& window, SystemManager& systemManager)
  : IList<SystemViewData, SystemData*>(window, LIST_SCROLL_STYLE_SLOW, LoopType::Always)
  , mSystemManager(systemManager)
  , mCarousel()
  , mSystemInfo(window, "SYSTEM INFO", Font::get(FONT_SIZE_SMALL), 0x33333300, TextAlignment::Center)
  , mProgressInterface(nullptr)
  , mSender(*this)
  , mSystemFromWitchToExtractData(nullptr)
  , mCurrentSystem(nullptr)
  , mCamOffset(0)
  , mExtrasCamOffset(0)
  , mExtrasFadeOpacity(0.0f)
  , mViewNeedsReload(true)
  , mShowing(false)
  , mLaunchKodi(false)
{
  setSize(Renderer::Instance().DisplayWidthAsFloat(), Renderer::Instance().DisplayHeightAsFloat());
  Start("system-info");
}

void SystemView::AlignComponent(CarouselType carouselType, TextAlignment alignment, Component* component)
{
  if (carouselType == CarouselType::Vertical || carouselType == CarouselType::VerticalWheel)
  {
    if      (alignment == TextAlignment::Left)  component->setOrigin(0, 0.5);
    else if (alignment == TextAlignment::Right) component->setOrigin(1.0, 0.5);
    else                                         component->setOrigin(0.5, 0.5);
  }
  else
  {
    if      (alignment == TextAlignment::Top)    component->setOrigin(0.5, 0);
    else if (alignment == TextAlignment::Bottom) component->setOrigin(0.5, 1);
    else                                          component->setOrigin(0.5, 0.5);
  }
}

void SystemView::addSystem(SystemData * it)
{
  const ThemeData& theme = (it)->Theme();

  if (mViewNeedsReload)
    RefreshViewElements(theme);

  Entry e;
  e.name = (it)->Name();
  e.object = it;

  // make logo
  const ThemeElement* logoElement = theme.Element("system", "logo", ThemeElementType::Image);
  if(logoElement != nullptr && logoElement->HasProperties())
  {
    ImageComponent* logo = new ImageComponent(mWindow, false, false);
    logo->setResize(mCarousel.logoSize * mCarousel.logoScale);
    logo->setKeepRatio(true);
    logo->DoApplyThemeElement((it)->Theme(), "system", "logo", ThemePropertyCategory::Path);
    e.data.logo = std::shared_ptr<ImageComponent>(logo);
    if ((it)->ThemeFolder() == "default") // #TODO: Wrong identification of default theme. Fetch info from the theme itself
    {
      TextComponent* text = new TextComponent(mWindow, (it)->Name(), Font::get(FONT_SIZE_MEDIUM),
                                              0x1A1A1AFF, TextAlignment::Center);
      text->setSize(mCarousel.logoSize * mCarousel.logoScale);
      e.data.logotext = std::shared_ptr<TextComponent>(text);
      if (mCarousel.type == CarouselType::Vertical || mCarousel.type == CarouselType::VerticalWheel)
        text->setHorizontalAlignment(mCarousel.logoAlignment);
      else
        text->setVerticalAlignment(mCarousel.logoAlignment);
    }
  }
  else
  {
    String systemName = it->Name();
    systemName.Remove(SystemManager::sGenrePrefix);
    GameGenres genre = Genres::LookupFromName(systemName);
    if (genre == GameGenres::None)
    {
      // no logo in theme; use text
      e.data.logo = nullptr;
      TextComponent* text = new TextComponent(mWindow, (it)->FullName(), mCarousel.fontlarge, 0x000000FF,
                                              TextAlignment::Center);
      text->setSize(mCarousel.logoSize * mCarousel.logoScale);
      e.data.logotext = std::shared_ptr<TextComponent>(text);
      if (mCarousel.type == CarouselType::Vertical || mCarousel.type == CarouselType::VerticalWheel)
        text->setHorizontalAlignment(mCarousel.logoAlignment);
      else
        text->setVerticalAlignment(mCarousel.logoAlignment);
    }
    else
    {
      ImageComponent* logo = new ImageComponent(mWindow, false, false);
      logo->setResize(mCarousel.logoSize * mCarousel.logoScale);
      logo->setKeepRatio(true);
      logo->setImage(Genres::GetResourcePath(genre));
      e.data.logo = std::shared_ptr<ImageComponent>(logo);

      TextComponent* text = new TextComponent(mWindow, (it)->FullName(), mCarousel.fontmedium,
                                              0xE6E6E6FF, TextAlignment::Center);
      text->setSize(mCarousel.logoSize * mCarousel.logoScale);
      e.data.logotext = std::shared_ptr<TextComponent>(text);
      if (mCarousel.type == CarouselType::Vertical || mCarousel.type == CarouselType::VerticalWheel)
        text->setHorizontalAlignment(mCarousel.logoAlignment);
      else
        text->setVerticalAlignment(mCarousel.logoAlignment);
    }
  }

  if (e.data.logo)
  {
    AlignComponent(mCarousel.type, mCarousel.logoAlignment, e.data.logo.get());
    Vector2f denormalized = mCarousel.logoSize * e.data.logo->getOrigin();
    e.data.logo->setPosition(denormalized.x(), denormalized.y(), 0.0);
  }

  if (e.data.logotext)
  {
    AlignComponent(mCarousel.type, mCarousel.logoAlignment, e.data.logotext.get());
    Vector2f denormalized = mCarousel.logoSize * e.data.logotext->getOrigin();
    e.data.logotext->setPosition(denormalized.x(), denormalized.y(), 0.0);
  }

  e.data.backgroundExtras = std::make_shared<ThemeExtras>(mWindow);
  e.data.backgroundExtras->AssignExtras((it)->Theme(), "system", (it)->Theme().GetExtras("system", mWindow, nullptr), false);
  e.data.backgroundExtras->setSize(mSize);

  this->add(e);
}

SystemData* SystemView::Prev()
{
  SystemData* prev = mSystemManager.PreviousVisible(mCurrentSystem);
  while(!prev->HasVisibleGame())
    prev = mSystemManager.PreviousVisible(prev);

  return prev;
}

void SystemView::Sort()
{
  // Make a reference map: system => entry
  HashMap<const SystemData*, Entry*> map;
  for(Entry& entry : mEntries) map[entry.object] = &entry;

  // Sort
  std::vector<Entry> newEntries;
  for(const SystemData* system : mSystemManager.VisibleSystemList())
    if (Entry** entry = map.try_get(system); entry != nullptr)
      newEntries.push_back(**entry);
    else
    { LOG(LogError) << "[SystemView] Sort cannot lookup visible system '" << system->FullName() << "' in system entries!"; }

  // Set new sorted vector
  mEntries = newEntries;
  goToSystem(mCurrentSystem, false);
}

void SystemView::populate()
{
  mEntries.clear();

  // Count valid systems
  int count = 0;
  SystemManager::List systems;
  for (const auto& it : mSystemManager.VisibleSystemList())
    if (it->HasVisibleGame())
    {
      systems.Add(it);
      ++count;
    }

  // Initialize progress
  if (mProgressInterface != nullptr)
  {
    mProgressInterface->SetMaximum(count);
    mProgressInterface->SetProgress(0);
  }

  // Initialize systems
  count = 0;
  for (const auto& it : systems)
  {
    addSystem(it);
    if (mProgressInterface != nullptr)
      mProgressInterface->SetProgress(++count);
  }
  Sort();
}

void SystemView::goToSystem(SystemData* system, bool animate)
{
  // Systems lazy initialization
  if (mEntries.empty())
    populate();

  if (!setCursor(system)) // When deleting last favorite from favorite view, favorite system is no longer available
    setCursor(mSystemManager.FirstNonEmptySystem());

  if(!animate)
    finishAnimation(0);
  onCursorChanged(CursorState::Stopped);
}

bool SystemView::ProcessInput(const InputCompactEvent& event)
{
  if (event.AnythingPressed())
  {
    switch (mCarousel.type)
    {
      case CarouselType::Vertical:
      case CarouselType::VerticalWheel:
      {
        if (event.AnyUpPressed()) { listInput(-1); return true; }
        if (event.AnyDownPressed()) { listInput(1); return true; }
        break;
      }
      case CarouselType::Horizontal:
      default:
      {
        if (event.AnyLeftPressed()) { listInput(-1); return true; }
        if (event.AnyRightPressed()) { listInput(1); return true; }
        break;
      }
    }
    if (event.ValidPressed())
    {
      stopScrolling();
      ViewController::Instance().goToGameList(getSelected());
      return true;
    }
    if (event.YPressed() && GameClipView::IsGameClipEnabled())
    {
      mWindow.DoSleep();
      ViewController::Instance().goToGameClipView();
      return true;
    }

    if (event.XPressed())
    {
      bool kodiExists = RecalboxSystem::kodiExists();
      bool kodiEnabled = RecalboxConf::Instance().GetKodiEnabled();
      bool kodiX = RecalboxConf::Instance().GetKodiXButton();
      bool netplay = RecalboxConf::Instance().GetNetplayEnabled();

      if (kodiExists && kodiEnabled && kodiX && !mLaunchKodi && !mWindow.HasGui())
      {
        if (netplay) mWindow.pushGui(new GuiMenuSwitchKodiNetplay(mWindow, mSystemManager));
        else
        {
          mLaunchKodi = true;
          if (!GameRunner::Instance().RunKodi()) { LOG(LogWarning) << "[SystemView] Kodi terminated with non-zero result!"; }
          mLaunchKodi = false;
        }
      }
      else if (netplay && !mWindow.HasGui())
      {
        auto* netplayGui = new GuiNetPlay(mWindow, mSystemManager);
        mWindow.pushGui(netplayGui);
      }
    }

    if (event.SelectPressed() && MenuFilter::ShouldDisplayMenu(MenuFilter::Menu::Exit))
    {
      GuiMenuQuit::PushQuitGui(mWindow);
    }

    if (event.StartPressed() && MenuFilter::ShouldDisplayMenu(MenuFilter::Menu::Main))
    {
      mWindow.pushGui(new GuiMenu(mWindow, mSystemManager));
      return true;
    }

    if (event.R1Pressed() && MenuFilter::ShouldDisplayMenu(MenuFilter::Menu::Search))
    {
      mWindow.pushGui(new GuiSearch(mWindow, mSystemManager));
      return true;
    }
  }
  else if (event.AnyLeftReleased() || event.AnyRightReleased() || event.AnyUpReleased() || event.AnyDownReleased())
    listInput(0);

  return Component::ProcessInput(event);
}

void SystemView::Update(int deltaTime)
{
  UpdateExtra(deltaTime);
  listUpdate(deltaTime);
  Component::Update(deltaTime);
}

void SystemView::onCursorChanged(const CursorState& state)
{
  (void)state;

  if(mCurrentSystem != getSelected()){
    mCurrentSystem = getSelected();
    AudioManager::Instance().StartPlaying(getSelected()->Theme());
  }
  // update help style
  UpdateHelpBar();

  // update externs
  NotificationManager::Instance().Notify(*getSelected(), Notification::SystemBrowsing);

  float startPos = mCamOffset;

  float posMax = (float)mEntries.size();
  float target = (float)mCursor;

  // what's the shortest way to get to our target?
  // it's one of these...

  float endPos = target; // directly
  float dist = std::abs(endPos - startPos);

  if (std::abs(target + posMax - startPos) < dist) endPos = target + posMax; // loop around the end (0 -> max)
  if (std::abs(target - posMax - startPos) < dist) endPos = target - posMax; // loop around the start (max - 1 -> -1)

  // Set next system from witch extract game information
  SetNextSystem(getSelected());
  cancelAnimation(1);
  cancelAnimation(2);
  mSystemInfo.setOpacity(0);

  // no need to animate transition, we're not going anywhere (probably mEntries.size() == 1)
  if(endPos == mCamOffset && endPos == mExtrasCamOffset)
    return;

  Animation* anim = nullptr;
  bool move_carousel = RecalboxConf::Instance().GetThemeCarousel();
  String transitionTheme = ThemeManager::Instance().Main().getTransition();
  if (transitionTheme.empty()) transitionTheme = RecalboxConf::Instance().GetThemeTransition();
  if(transitionTheme == "fade")
  {
    float startExtrasFade = mExtrasFadeOpacity;
    anim = new LambdaAnimation([startExtrasFade, startPos, endPos, posMax, this, move_carousel](float t)
    {
      t -= 1;
      float f = lerp<float>(startPos, endPos, t*t*t + 1);
      if(f < 0) f += posMax;
      if(f >= posMax) f -= posMax;
      mCamOffset = move_carousel ? f : endPos;

      t += 1;
      if(t < 0.3f) mExtrasFadeOpacity = lerp<float>(0.0f, 1.0f, t / 0.3f + startExtrasFade);
      else if(t < 0.7f) mExtrasFadeOpacity = 1.0f;
      else mExtrasFadeOpacity = lerp<float>(1.0f, 0.0f, (t - 0.7f) / 0.3f);

      if(t > 0.5f) mExtrasCamOffset = endPos;
    }, 500);
  }
  else if (transitionTheme == "slide")
  { // slide
    anim = new LambdaAnimation([startPos, endPos, posMax, this, move_carousel](float t)
    {
      t -= 1;
      float f = lerp<float>(startPos, endPos, t*t*t + 1);
      if(f < 0) f += posMax;
      if(f >= posMax) f -= posMax;

      mCamOffset = move_carousel ? f : endPos;
      mExtrasCamOffset = f;
    }, 500);
  }
  else
  {
  // instant
    anim = new LambdaAnimation([this, startPos, endPos, posMax, move_carousel](float t)
    {
      t -= 1;
      float f = lerp<float>(startPos, endPos, t*t*t + 1);
      if (f < 0) f += posMax;
      if (f >= posMax) f -= posMax;
      mCamOffset = move_carousel ? f : endPos;
      mExtrasCamOffset = endPos;
    }, move_carousel ? 500 : 1);
  }

  setAnimation(anim, 0, nullptr, false, 0);
}

void SystemView::Render(const Transform4x4f& parentTrans)
{
  if(size() == 0) return;  // nothing to render

  // draw the list elements (titles, backgrounds, logos)
  Transform4x4f trans = getTransform() * parentTrans;

  auto systemInfoZIndex = mSystemInfo.getZIndex();
  auto minMax = std::minmax(mCarousel.zIndex, systemInfoZIndex);

  renderExtras(trans, INT16_MIN, minMax.first);
  renderFade(trans);

  if (mCarousel.zIndex > mSystemInfo.getZIndex()) renderInfoBar(trans);
  else renderCarousel(trans);

  renderExtras(trans, minMax.first, minMax.second);

  if (mCarousel.zIndex > mSystemInfo.getZIndex()) renderCarousel(trans);
  else renderInfoBar(trans);

  renderExtras(trans, minMax.second, INT16_MAX);
}

bool SystemView::CollectHelpItems(Help& help)
{
  help.Set(mCarousel.type == CarouselType::Vertical ? HelpType::UpDown : HelpType::LeftRight, _("CHOOSE"))
      .Set(Help::Valid(), _("SELECT"));

  if (RecalboxSystem::kodiExists() && RecalboxConf::Instance().GetKodiEnabled() && RecalboxConf::Instance().GetKodiXButton())
    help.Set(HelpType::X, RecalboxConf::Instance().GetNetplayEnabled() ? _("KODI/NETPLAY") : _("START KODI"));
  else if (RecalboxConf::Instance().GetNetplayEnabled())
    help.Set(HelpType::X, _("NETPLAY"));

  help.Set(HelpType::Select, _("QUIT"))
      .Set(HelpType::Start, _("MENU"))
      .Set(HelpType::R, _("SEARCH"));

  if(GameClipView::IsGameClipEnabled())
  {
    help.Set(HelpType::Y, _("gameclip"));
  }

  return true;
}

/*void SystemView::onThemeChanged(const ThemeData& theme)
{
  (void)theme; // TODO: Log theme name
  { LOG(LogDebug) << "[SystemView] Theme Changed"; }
  mViewNeedsReload = true;
  populate();
}*/

void SystemView::SwitchToTheme(const ThemeData& uselessTheme, bool refreshOnly, IThemeSwitchTick* interface)
{
  (void)uselessTheme;

  // Refrehs view elements
  //DateTime starts;
  RefreshViewElements(ThemeManager::Instance().Main());
  //{ LOG(LogWarning) << "RefreshViewElement " << (DateTime() - starts).ToMillisecondsString() << " ms"; }

  // Refresh systems
  for(auto& entry : mEntries)
  {
    //DateTime start;
    const ThemeData& theme = ThemeManager::Instance().System(entry.object);
    SystemViewData& data = entry.data;

    // Refresh logo properties
    if (data.logo != nullptr)
    {
      data.logo->setResize(mCarousel.logoSize * mCarousel.logoScale);
      data.logo->setKeepRatio(true);
      data.logo->DoApplyThemeElement(theme, "system", "logo", ThemePropertyCategory::All);
      AlignComponent(mCarousel.type, mCarousel.logoAlignment, data.logo.get());
      Vector2f denormalized = mCarousel.logoSize * data.logo->getOrigin();
      data.logo->setPosition(denormalized.x(), denormalized.y(), 0.0);
    }

    // Refresh theme properties
    if (data.logotext != nullptr)
    {
      data.logotext->setSize(mCarousel.logoSize * mCarousel.logoScale);
      if (mCarousel.type == CarouselType::Vertical || mCarousel.type == CarouselType::VerticalWheel)
        data.logotext->setHorizontalAlignment(mCarousel.logoAlignment);
      else data.logotext->setVerticalAlignment(mCarousel.logoAlignment);
      AlignComponent(mCarousel.type, mCarousel.logoAlignment, data.logotext.get());
      Vector2f denormalized = mCarousel.logoSize * data.logotext->getOrigin();
      data.logotext->setPosition(denormalized.x(), denormalized.y(), 0.0);
    }

    // Refresh extras
    if (refreshOnly)
    {
      //data.backgroundExtras->AssignExtras(theme.GetExtras("system", mWindow), true);
      theme.RefreshExtraProperties(data.backgroundExtras->Extras(), "system");
    }
    else
    {
      data.backgroundExtras = std::make_shared<ThemeExtras>(mWindow);
      data.backgroundExtras->AssignExtras(theme, "system", theme.GetExtras("system", mWindow, nullptr), false);
      data.backgroundExtras->setSize(mSize);
    }
    //{ LOG(LogWarning) << "Refresh " << entry.object->FullName() << " : " << (DateTime() - start).ToMillisecondsString() << " ms"; }
    interface->ThemeSwitchTick();
  }

  SetNextSystem(mCurrentSystem); // Force refresh of curren system info
}

//  Get the ThemeElements that make up the SystemView.
void SystemView::RefreshViewElements(const ThemeData& theme)
{
  { LOG(LogDebug) << "[SystemView] Get View Elements"; }
  getDefaultElements();

  const ThemeElement* carouselElem = theme.Element("system", "systemcarousel", ThemeElementType::Carousel);
  if (carouselElem != nullptr)
    getCarouselFromTheme(carouselElem);

  const ThemeElement* sysInfoElem = theme.Element("system", "systemInfo", ThemeElementType::Text);
  if (sysInfoElem != nullptr)
  {
    mSystemInfo.DoApplyThemeElement(theme, "system", "systemInfo", ThemePropertyCategory::All);
    if (!sysInfoElem->HasProperty(ThemePropertyName::Alignment))
      mSystemInfo.setHorizontalAlignment(TextAlignment::Center);
  }

  mViewNeedsReload = false;
}

//  Render system carousel
void SystemView::renderCarousel(const Transform4x4f& trans)
{
  // background box behind logos
  Transform4x4f carouselTrans = trans;
  carouselTrans.translate(Vector3f(mCarousel.pos.x(), mCarousel.pos.y(), 0.0));
  carouselTrans.translate(Vector3f(mCarousel.origin.x() * mCarousel.size.x() * -1, mCarousel.origin.y() * mCarousel.size.y() * -1, 0.0f));

  Vector2f clipPos(carouselTrans.translation().x(), carouselTrans.translation().y());
  Renderer::Instance().PushClippingRect(clipPos.toInt(), mCarousel.size.toInt());

  Renderer::SetMatrix(carouselTrans);
  Renderer::DrawRectangle(0.0, 0.0, mCarousel.size.x(), mCarousel.size.y(), mCarousel.color);

  // draw logos
  Vector2f logoSpacing(0.0, 0.0); // NB: logoSpacing will include the size of the logo itself as well!
  float xOff = 0;
  float yOff = 0;

  switch (mCarousel.type)
  {
    case CarouselType::VerticalWheel:
      yOff = (mCarousel.size.y() - mCarousel.logoSize.y()) / 2 - (mCamOffset * logoSpacing[1]);
      if (mCarousel.logoAlignment == TextAlignment::Left)
        xOff = mCarousel.logoSize.x() / 10;
      else if (mCarousel.logoAlignment == TextAlignment::Right)
        xOff = (float)(mCarousel.size.x() - (mCarousel.logoSize.x() * 1.1));
      else
        xOff = (mCarousel.size.x() - mCarousel.logoSize.x()) / 2;
      break;
    case CarouselType::Vertical:
      logoSpacing[1] = ((mCarousel.size.y() - (mCarousel.logoSize.y() * (float)mCarousel.maxLogoCount)) / (float)(mCarousel.maxLogoCount)) + mCarousel.logoSize.y();
      yOff = (mCarousel.size.y() - mCarousel.logoSize.y()) / 2 - (mCamOffset * logoSpacing[1]);

      if (mCarousel.logoAlignment == TextAlignment::Left)
        xOff = mCarousel.logoSize.x() / 10;
      else if (mCarousel.logoAlignment == TextAlignment::Right)
        xOff = (float)(mCarousel.size.x() - (mCarousel.logoSize.x() * 1.1));
      else
        xOff = (mCarousel.size.x() - mCarousel.logoSize.x()) / 2;
      break;
    case CarouselType::Horizontal:
      logoSpacing[0] = ((mCarousel.size.x() - (mCarousel.logoSize.x() * (float)mCarousel.maxLogoCount)) / (float)(mCarousel.maxLogoCount)) + mCarousel.logoSize.x();
      xOff = (mCarousel.size.x() - mCarousel.logoSize.x()) / 2 - (mCamOffset * logoSpacing[0]);

      if (mCarousel.logoAlignment == TextAlignment::Top)
        yOff = mCarousel.logoSize.y() / 10;
      else if (mCarousel.logoAlignment == TextAlignment::Bottom)
        yOff = (float)(mCarousel.size.y() - (mCarousel.logoSize.y() * 1.1));
      else
        yOff = (mCarousel.size.y() - mCarousel.logoSize.y()) / 2;
      break;
    default:
      break;
  }

  int center = (int)(mCamOffset);
  int logoCount = Math::min(mCarousel.maxLogoCount, (int)mEntries.size());

  // Adding texture loading buffers depending on scrolling speed and status
  int bufferIndex = getScrollingVelocity() + 1;

  int bufferLeft = logoBuffersLeft[bufferIndex];
  int bufferRight = logoBuffersRight[bufferIndex];
  if (logoCount == 1)
  {
    bufferLeft = 0;
    bufferRight = 0;
  }

  for (int i = center - logoCount / 2 + bufferLeft; i <= center + logoCount / 2 + bufferRight; i++)
  {
    int index = i;
    while (index < 0)
      index += (int)mEntries.size();
    while (index >= (int)mEntries.size())
      index -= (int)mEntries.size();

    Transform4x4f logoTrans = carouselTrans;
    logoTrans.translate(Vector3f((float)i * logoSpacing[0] + xOff, (float)i * logoSpacing[1] + yOff, 0));

    float distance = (float)i - mCamOffset;

    float scale = 1.0f + ((mCarousel.logoScale - 1.0f) * (1.0f - fabs(distance)));
    scale = Math::min(mCarousel.logoScale, Math::max(1.0f, scale));
    scale /= mCarousel.logoScale;

    int opacity = Math::roundi(0x80 + ((0xFF - 0x80) * (1 - fabs(distance))));
    opacity = Math::max((int) 0x80, opacity);

    if (mEntries[index].data.logo)
    {
      const std::shared_ptr<Component>& comp = mEntries[index].data.logo;
      if (mCarousel.type == CarouselType::VerticalWheel)
      {
        comp->setRotationDegrees(mCarousel.logoRotation * distance);
        comp->setRotationOrigin(mCarousel.logoRotationOrigin);
      }
      comp->setScale(scale);
      comp->setOpacity(opacity);
      comp->Render(logoTrans);
    }

    if (mEntries[index].data.logotext)
    {
      const std::shared_ptr<Component> &comp2 = mEntries[index].data.logotext;
      if (mCarousel.type == CarouselType::VerticalWheel)
      {
        comp2->setRotationDegrees(mCarousel.logoRotation * distance);
        comp2->setRotationOrigin(mCarousel.logoRotationOrigin);
      }
      comp2->setScale(scale);
      comp2->setOpacity(opacity);
      comp2->Render(logoTrans);
    }
  }
  Renderer::Instance().PopClippingRect();
}

void SystemView::renderInfoBar(const Transform4x4f& trans)
{
  Renderer::SetMatrix(trans);
  mSystemInfo.Render(trans);
}

void SystemView::UpdateExtra(int deltaTime)
{
  int extrasCenter = (int)mExtrasCamOffset;
  // Adding texture loading buffers depending on scrolling speed and status
  int bufferIndex = getScrollingVelocity() + 1;
  for (int i = extrasCenter + logoBuffersLeft[bufferIndex]; i <= extrasCenter + logoBuffersRight[bufferIndex]; i++)
  {
    int index = i;
    while (index < 0) index += (int)mEntries.size();
    while (index >= (int)mEntries.size()) index -= (int)mEntries.size();
    //Only render selected system when not showing
    if (mShowing || index == mCursor)
    {
      SystemViewData& data = mEntries[index].data;
      for(ThemeExtras::Extra& extra : data.backgroundExtras->Extras())
        extra.Component().Update(deltaTime);
    }
  }
}

// Draw background extras
void SystemView::renderExtras(const Transform4x4f& trans, float lower, float upper)
{
  int extrasCenter = (int)mExtrasCamOffset;

  Renderer::Instance().PushClippingRect(Vector2i::Zero(), mSize.toInt());

  // Adding texture loading buffers depending on scrolling speed and status
  int bufferIndex = getScrollingVelocity() + 1;

  for (int i = extrasCenter + logoBuffersLeft[bufferIndex]; i <= extrasCenter + logoBuffersRight[bufferIndex]; i++)
  {
    int index = i;
    while (index < 0) index += (int)mEntries.size();
    while (index >= (int)mEntries.size()) index -= (int)mEntries.size();

    //Only render selected system when not showing
    if (mShowing || index == mCursor)
    {
      Transform4x4f extrasTrans = trans;
      if (mCarousel.type == CarouselType::Horizontal)
        extrasTrans.translate(Vector3f(((float)i - mExtrasCamOffset) * mSize.x(), 0, 0));
      else
        extrasTrans.translate(Vector3f(0, ((float)i - mExtrasCamOffset) * mSize.y(), 0));

      Renderer::Instance().PushClippingRect(Vector2i((int)extrasTrans.translation()[0], (int)extrasTrans.translation()[1]), mSize.toInt());
      SystemViewData& data = mEntries[index].data;
      for(ThemeExtras::Extra& extra : data.backgroundExtras->Extras())
        if (extra.Component().getZIndex() >= lower && extra.Component().getZIndex() < upper)
          extra.Component().Render(extrasTrans);
      Renderer::Instance().PopClippingRect();
    }
  }
  Renderer::Instance().PopClippingRect();
}

void SystemView::renderFade(const Transform4x4f& trans)
{
  // fade extras if necessary
  if (mExtrasFadeOpacity != 0.0f)
  {
      Renderer::SetMatrix(trans);
      Renderer::DrawRectangle(0.0f, 0.0f, mSize.x(), mSize.y(), 0x00000000 | (unsigned char)(mExtrasFadeOpacity * 255));
  }
}

// Populate the system carousel with the legacy values
void  SystemView::getDefaultElements()
{
  // Carousel
  mCarousel.type = CarouselType::Horizontal;
  mCarousel.logoAlignment = TextAlignment::Center;
  mCarousel.size.x() = mSize.x();
  mCarousel.size.y() = 0.2325f * mSize.y();
  mCarousel.pos.x() = 0.0f;
  mCarousel.pos.y() = 0.5f * (mSize.y() - mCarousel.size.y());
  mCarousel.origin.x() = 0.0f;
  mCarousel.origin.y() = 0.0f;
  mCarousel.color = 0xFFFFFFD8;
  mCarousel.logoScale = 1.2f;
  mCarousel.logoRotation = 7.5;
  mCarousel.logoRotationOrigin.x() = -5;
  mCarousel.logoRotationOrigin.y() = 0.5;
  mCarousel.logoSize.x() = 0.25f * (Math::max(mSize.y(), mSize.x()));
  mCarousel.logoSize.y() = 0.155f * (Math::min(mSize.y(), mSize.x()));
  mCarousel.maxLogoCount = 3;
  mCarousel.zIndex = 40;

  // System Info Bar
  mSystemInfo.setSize(mSize.x(), mSystemInfo.getFont()->getLetterHeight()*2.2f);
  mSystemInfo.setPosition(0, (mCarousel.pos.y() + mCarousel.size.y() - 0.2f));
  mSystemInfo.setBackgroundColor(0xDDDDDDD8);
  mSystemInfo.setRenderBackground(true);
  mSystemInfo.setFont(Font::get(Renderer::Instance().Is480pOrLower() ? (int)FONT_SIZE_MEDIUM : (int)(0.035f * (Math::min(mSize.y(), mSize.x()))), Font::getDefaultPath()));
  mSystemInfo.setHorizontalAlignment(TextAlignment::Center);
  mSystemInfo.setColor(0x000000FF);
  mSystemInfo.setZIndex(50);
  mSystemInfo.setDefaultZIndex(50);
}

void SystemView::getCarouselFromTheme(const ThemeElement* elem)
{
  if (elem->HasProperty(ThemePropertyName::Type))
  {
    String type = elem->AsString(ThemePropertyName::Type);
    if (type == "vertical") mCarousel.type = CarouselType::Vertical;
    else if (type == "vertical_wheel") mCarousel.type = CarouselType::VerticalWheel;
    else mCarousel.type = CarouselType::Horizontal;
  }
  if (elem->HasProperty(ThemePropertyName::Size)) mCarousel.size = elem->AsVector(ThemePropertyName::Size) * mSize;
  if (elem->HasProperty(ThemePropertyName::Pos)) mCarousel.pos = elem->AsVector(ThemePropertyName::Pos) * mSize;
  if (elem->HasProperty(ThemePropertyName::Origin)) mCarousel.origin = elem->AsVector(ThemePropertyName::Origin);
  if (elem->HasProperty(ThemePropertyName::Color)) mCarousel.color = (unsigned int)elem->AsInt(ThemePropertyName::Color);
  if (elem->HasProperty(ThemePropertyName::LogoScale)) mCarousel.logoScale = elem->AsFloat(ThemePropertyName::LogoScale);
  if (elem->HasProperty(ThemePropertyName::LogoSize)) mCarousel.logoSize = elem->AsVector(ThemePropertyName::LogoSize) * mSize;
  if (elem->HasProperty(ThemePropertyName::MaxLogoCount)) mCarousel.maxLogoCount = Math::roundi(elem->AsFloat(ThemePropertyName::MaxLogoCount));
  if (elem->HasProperty(ThemePropertyName::ZIndex)) mCarousel.zIndex = elem->AsFloat(ThemePropertyName::ZIndex);
  if (elem->HasProperty(ThemePropertyName::LogoRotation)) mCarousel.logoRotation = elem->AsFloat(ThemePropertyName::LogoRotation);
  if (elem->HasProperty(ThemePropertyName::LogoRotationOrigin)) mCarousel.logoRotationOrigin = elem->AsVector(ThemePropertyName::LogoRotationOrigin);
  if (elem->HasProperty(ThemePropertyName::LogoAlignment))
  {
    String align = elem->AsString(ThemePropertyName::LogoAlignment);
    if (align == "left") mCarousel.logoAlignment = TextAlignment::Left;
    else if (align == "right") mCarousel.logoAlignment = TextAlignment::Right;
    else if (align == "top") mCarousel.logoAlignment = TextAlignment::Top;
    else if (align == "bottom") mCarousel.logoAlignment = TextAlignment::Bottom;
    else mCarousel.logoAlignment = TextAlignment::Center;
  }
  mCarousel.fontlarge = Font::getFromTheme(*elem, ThemePropertyCategory::All, Font::get(FONT_SIZE_LARGE));
  mCarousel.fontmedium = Font::getFromTheme(*elem, ThemePropertyCategory::All, Font::get(FONT_SIZE_MEDIUM));
  if (!mCarousel.fontlarge) mCarousel.fontlarge = Font::get(FONT_SIZE_LARGE);
  if (!mCarousel.fontlarge) mCarousel.fontlarge = Font::get(FONT_SIZE_MEDIUM);
}

void SystemView::RemoveCurrentSystem()
{
  removeSystem(mCurrentSystem);
}

void SystemView::removeSystem(SystemData * system)
{
  SystemData* previousSystem = Prev();
  for(auto it = mEntries.begin(); it != mEntries.end(); ++it)
    if (it->object == system)
    {
      mEntries.erase(it);
      break;
    }
  goToSystem(system == mCurrentSystem ? previousSystem : mCurrentSystem, true);
}

void SystemView::manageSystemsList()
{
  std::vector<Entry> backupedEntries = mEntries;

  for (const auto& system : mSystemManager.AllSystems())
  {
    if(system->Descriptor().IsPort())
      continue;

    bool hasGame = system->HasVisibleGame();
    bool systemIsAlreadyVisible = LookupSystemByName(system->Name()) != nullptr;

    if(!systemIsAlreadyVisible && hasGame) addSystem(system);
    else if (systemIsAlreadyVisible && !hasGame) removeSystem(system);
  }

  if (mEntries.empty())
  {
    mEntries = backupedEntries;
    mWindow.displayMessage(_("Last operation removed all systems!\n\nThey have been restored to allow normal operations, regardless of the current filters."));
  }
}

SystemData* SystemView::LookupSystemByName(const String& name)
{
  for (auto& mEntrie : mEntries)
    if (mEntrie.object->Name() == name)
      return mEntrie.object;
  return nullptr;
}

void SystemView::SetNextSystem(const SystemData* system)
{
  Mutex::AutoLock locker(mSystemLocker);
  mSystemFromWitchToExtractData = system;
  mSystemSignal.Fire();
}

void SystemView::Run()
{
  while(IsRunning())
  {
    mSystemSignal.WaitSignal();
    if (!IsRunning()) return; // Destructor called :)
    int favorites = 0;
    int hidden = 0;
    int count = 0;
    for(bool working = true; working; )
    {
      mSystemLocker.Lock();
      const SystemData* system = mSystemFromWitchToExtractData;
      mSystemFromWitchToExtractData = nullptr;
      mSystemLocker.UnLock();
      if (system != nullptr) count = system->GameCount(favorites, hidden);
      else working = false;
    }
    if (count != 0)
      mSender.Send({ count, favorites, hidden });
  }
}

void SystemView::ReceiveSyncMessage(const SystemGameCount& data)
{
  //! Still on system list?
  if (ViewController::Instance().isViewing(ViewType::SystemList))
  {
    mSystemLocker.Lock();
    bool workToDoFirst = mSystemFromWitchToExtractData != nullptr;
    mSystemLocker.UnLock();
    if (workToDoFirst) return;

    // animate mSystemInfo's opacity (fade out, wait, fade back in)
    cancelAnimation(1);
    cancelAnimation(2);

    const float infoStartOpacity = (float)mSystemInfo.getOpacity() / 255.f;

    Animation* infoFadeOut = new LambdaAnimation([infoStartOpacity, this] (float t)
                                                 {
                                                   mSystemInfo.setOpacity((unsigned char)(lerp<float>(infoStartOpacity, 0.f, t) * 255));
                                                 }, (int)(infoStartOpacity * 150));

    // also change the text after we've fully faded out
    setAnimation(infoFadeOut, 0, [this, data]
    {
      String text(_N("%i GAME AVAILABLE", "%i GAMES AVAILABLE", data.VisibleGames));
      text.Replace("%i", String(data.VisibleGames));
      if (data.Hidden != 0)
        text.Append(", ").Append(_N("%i GAME HIDDEN", "%i GAMES HIDDEN", data.Hidden))
            .Replace("%i", String(data.Hidden));
      if (data.Favorites != 0)
        text.Append(", ").Append(_N("%i FAVORITE", "%i FAVORITES", data.Favorites))
            .Replace("%i", String(data.Favorites));
      mSystemInfo.setText(text);
    }, false, 1);

    Animation* infoFadeIn = new LambdaAnimation([this](float t)
                                                {
                                                  mSystemInfo.setOpacity((unsigned char)(lerp<float>(0.f, 1.f, t) * 255));
                                                }, 300);

    // wait ms to fade in
    setAnimation(infoFadeIn, 800, nullptr, false, 2);
  }
}

