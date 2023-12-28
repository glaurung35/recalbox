#include <utils/storage/HashMap.h>
#include <utils/cplusplus/StaticLifeCycleControler.h>
#include "components/HelpComponent.h"
#include "Renderer.h"
#include "components/ImageComponent.h"
#include "components/TextComponent.h"
#include "views/ViewController.h"
#include <themes/ThemeManager.h>
#include <WindowManager.h>

#define ICON_TEXT_SPACING (Renderer::Instance().Is480pOrLower() ? 2.0f : Math::max(Renderer::Instance().DisplayWidthAsFloat() * 0.004f, 2.0f)) // space between [icon] and [text] (px)
#define ENTRY_SPACING Math::max(Renderer::Instance().DisplayWidthAsFloat() * 0.008f, 2.0f) // space between [text] and next [icon] (px)

const HashMap<HelpType, Path>& HelpComponent::IconPathMap()
{
  static const HashMap<HelpType, Path> sIconPathMap =
  {
    { HelpType::UpDown,            Path(":/help/dpad_updown.svg") },
    { HelpType::LeftRight,         Path(":/help/dpad_leftright.svg") },
    { HelpType::AllDirections,     Path(":/help/dpad_all.svg") },
    { HelpType::Start,             Path(":/help/button_start.svg") },
    { HelpType::Select,            Path(":/help/button_select.svg") },
    { HelpType::A,                 Path(":/help/button_a.svg") },
    { HelpType::B,                 Path(":/help/button_b.svg") },
    { HelpType::X,                 Path(":/help/button_x.svg") },
    { HelpType::Y,                 Path(":/help/button_y.svg") },
    { HelpType::L,                 Path(":/help/button_l.svg") },
    { HelpType::R,                 Path(":/help/button_r.svg") },
    { HelpType::LR,                Path(":/help/button_lr.svg") },
    { HelpType::L2R2,              Path(":/help/button_l2r2.svg") },
    { HelpType::Joy1AllDirections, Path(":/help/joystick_all_L.svg") },
    { HelpType::Joy1UpDown,        Path(":/help/joystick_updown_L.svg") },
    { HelpType::Joy1LeftRight,     Path(":/help/joystick_leftright_L.svg") },
    { HelpType::Joy2AllDirections, Path(":/help/joystick_all_R.svg") },
    { HelpType::Joy2UpDown,        Path(":/help/joystick_updown_R.svg") },
    { HelpType::Joy2LeftRight,     Path(":/help/joystick_leftright_R.svg") },
  };
  return sIconPathMap;
}

HelpComponent::HelpComponent(WindowManager&window)
  : ThemableComponent(window)
  , IViewChanged(window)
  , mGrid(mWindow, Vector2i((int)Help::TypeCount() * 4, 1))
  , mScrolling(Scrolling::Initialize)
  , mScrollingTimeAccumulator(0)
  , mScrollingLength(0)
  , mScrollingOffset(0)
  , mFont(Font::get(FONT_SIZE_SMALL))
  , mPosition(0.012f, 0.9515f)
  , mIconColor(0x777777FF)
  , mTextColor(0x777777FF)
{
}

void HelpComponent::UpdateHelps(bool force)
{
  // No change?
  if (mHelp.Equals(HelpItems()) && !force)
    return;

  mGrid.ClearEntries();
  mHelp = HelpItems();
	if(!RecalboxConf::Instance().GetShowHelp() || mHelp.Empty())
		return;

	const std::shared_ptr<Font>& font = mFont;

	// [icon] [spacer1] [text] [spacer2]
	std::vector< std::shared_ptr<ImageComponent> > icons;
	std::vector< std::shared_ptr<TextComponent> > labels;

	float width = 0;
	const float height = Math::round(font->getLetterHeight() * 1.25f);
	for (int i = 0; i < Help::TypeCount(); ++i)
    if (mHelp.IsSet(Help::TypeFromIndex(i)))
    {
      auto icon = std::make_shared<ImageComponent>(mWindow);
      const Path imagePath = mImagesPath[i];
      icon->setImage(imagePath);
      if (!ResourceManager::fileExists(imagePath))
      {
        { LOG(LogError) << "[HelpComponent] Icon image not found for item " << i; }
        icon->setImage(IconPathMap().get_or_return_default(Help::TypeFromIndex(i)));
      }

      icon->setColorShift(mIconColor);
      icon->setResize(0, height);
      icons.push_back(icon);

      auto lbl = std::make_shared<TextComponent>(mWindow, mHelp.Text(Help::TypeFromIndex(i)).ToUpperCaseUTF8(), font, mTextColor);
      labels.push_back(lbl);

      width += icon->getSize().x() + lbl->getSize().x() + ICON_TEXT_SPACING + ENTRY_SPACING;
    }

	mGrid.setSize(width, height);
  mScrollingLength = Math::roundi(width) - (Renderer::Instance().DisplayWidthAsInt() - Math::roundi(2.0f * mPosition.x()));
  mScrollingOffset = 0;
  mScrollingTimeAccumulator = 0;
  mScrolling = Scrolling::Initialize;
	for (int i = 0; i < (int)icons.size(); i++)
	{
		const int col = i*4;
		mGrid.setColWidthPerc(col, (icons[i]->getSize().x() / width));
		mGrid.setColWidthPerc(col + 1, (ICON_TEXT_SPACING / width));
		mGrid.setColWidthPerc(col + 2, (labels[i]->getSize().x() / width));
    mGrid.setColWidthPerc(col + 3, (ENTRY_SPACING / width));

		mGrid.setEntry(icons[i], Vector2i(col, 0), false, false);
		mGrid.setEntry(labels[i], Vector2i(col + 2, 0), false, false);
	}
	mGrid.setPosition(Vector3f(mPosition.x() * Renderer::Instance().DisplayWidthAsFloat(), mPosition.y() * Renderer::Instance().DisplayHeightAsFloat(), 0.0f));
}

void HelpComponent::setOpacity(unsigned char opacity)
{
	Component::setOpacity(opacity);

	for (int i = (int)mGrid.getChildCount(); --i >= 0; )
		mGrid.getChild(i)->setOpacity(opacity);
}

void HelpComponent::Render(const Transform4x4f& parentTrans)
{
	Transform4x4f trans = (parentTrans * getTransform()).translate({ (float)-mScrollingOffset, 0, 0 });
	if (mGrid.EntryCount() != 0)
    mGrid.Render(trans);
}

void HelpComponent::Update(int deltaTime)
{
  if (mGrid.EntryCount() != 0)
    if (mScrollingLength > 10)
      switch(mScrolling)
      {
        case Scrolling::Initialize:
        {
          mScrollingOffset = 0;
          mScrollingTimeAccumulator = 0;
          mScrolling = Scrolling::PauseLeft;
          break;
        }
        case Scrolling::PauseLeft:
        {
          mScrollingOffset = 0;
          if (mScrollingTimeAccumulator += deltaTime; mScrollingTimeAccumulator > sPauseTime)
          {
            mScrollingTimeAccumulator = 0;
            mScrolling = Scrolling::ScrollToRight;
          }
          break;
        }
        case Scrolling::ScrollToRight:
        {
          if (++mScrollingOffset >= mScrollingLength)
          {
            mScrollingTimeAccumulator = 0;
            mScrolling = Scrolling::PauseRight;
          }
          break;
        }
        case Scrolling::PauseRight:
        {
          mScrollingOffset = mScrollingLength - 1;
          if (mScrollingTimeAccumulator += deltaTime; mScrollingTimeAccumulator > sPauseTime)
          {
            mScrollingTimeAccumulator = 0;
            mScrolling = Scrolling::ScrollToLeft;
          }
          break;
        }
        case Scrolling::ScrollToLeft:
        {
          if (--mScrollingOffset <= 0)
          {
            mScrollingTimeAccumulator = 0;
            mScrolling = Scrolling::PauseLeft;
          }
          break;
        }
      }
}

void HelpComponent::SwitchToTheme(const ThemeData& theme, bool refreshOnly)
{
  (void)theme; // Always use main theme
  (void)refreshOnly; // Always rebuild, this is fast
  ViewChanged(ViewController::Instance().CurrentView(), mWindow.HasGui());
  UpdateHelps(true);
}

void HelpComponent::OnApplyThemeElement(const ThemeElement& element, ThemePropertyCategory properties)
{
  (void)properties;
  for(int i=Help::TypeCount(); --i>=0; )
    mImagesPath[i] = Path(IconPathMap().get_or_return_default(Help::TypeFromIndex(i)));

  if(element.HasProperty(ThemePropertyName::Pos))       mPosition = element.AsVector(ThemePropertyName::Pos);
  if(element.HasProperty(ThemePropertyName::TextColor)) mTextColor = (unsigned int)element.AsInt(ThemePropertyName::TextColor);
  if(element.HasProperty(ThemePropertyName::IconColor)) mIconColor = (unsigned int)element.AsInt(ThemePropertyName::IconColor);
  if(element.HasProperty(ThemePropertyName::FontSize) || element.HasProperty(ThemePropertyName::FontPath))
    mFont = Font::getFromTheme(element, ThemePropertyCategory::All, mFont);

  if(element.HasProperty(ThemePropertyName::IconUpDown))          mImagesPath[(int)HelpType::UpDown]        = Path(element.AsString(ThemePropertyName::IconUpDown));
  if(element.HasProperty(ThemePropertyName::IconLeftRight))       mImagesPath[(int)HelpType::LeftRight]     = Path(element.AsString(ThemePropertyName::IconLeftRight));
  if(element.HasProperty(ThemePropertyName::IconUpDownLeftRight)) mImagesPath[(int)HelpType::AllDirections] = Path(element.AsString(ThemePropertyName::IconUpDownLeftRight));
  if(element.HasProperty(ThemePropertyName::IconA))               mImagesPath[(int)HelpType::A]             = Path(element.AsString(ThemePropertyName::IconA));
  if(element.HasProperty(ThemePropertyName::IconB))               mImagesPath[(int)HelpType::B]             = Path(element.AsString(ThemePropertyName::IconB));
  if(element.HasProperty(ThemePropertyName::IconX))               mImagesPath[(int)HelpType::X]             = Path(element.AsString(ThemePropertyName::IconX));
  if(element.HasProperty(ThemePropertyName::IconY))               mImagesPath[(int)HelpType::Y]             = Path(element.AsString(ThemePropertyName::IconY));
  if(element.HasProperty(ThemePropertyName::IconL))               mImagesPath[(int)HelpType::L]             = Path(element.AsString(ThemePropertyName::IconL));
  if(element.HasProperty(ThemePropertyName::IconR))               mImagesPath[(int)HelpType::R]             = Path(element.AsString(ThemePropertyName::IconR));
  if(element.HasProperty(ThemePropertyName::IconStart))           mImagesPath[(int)HelpType::Start]         = Path(element.AsString(ThemePropertyName::IconStart));
  if(element.HasProperty(ThemePropertyName::IconSelect))          mImagesPath[(int)HelpType::Select]        = Path(element.AsString(ThemePropertyName::IconSelect));
}

void HelpComponent::ViewChanged(ViewType currentView, bool hasWindowOver)
{
  String viewName = "system";
  if (hasWindowOver)
  {
    // We are on a menu or a window. Select menu view or if it doesn't exists, just keep default
    if (ThemeManager::Instance().Main().HasMenuView())
      viewName = "menu";
  }
  else
  {
    switch(currentView)
    {
      case ViewType::GameList:
      {
        viewName = ((ISimpleGameListView&)ViewController::Instance().CurrentUi()).System().Name();
        break;
      }
      case ViewType::SystemList:
      case ViewType::None:
      case ViewType::SplashScreen:
      case ViewType::GameClip:
      case ViewType::CrtCalibration:
      default: break; // Keep system view
    }
  }
  DoApplyThemeElement(ThemeManager::Instance().Main(), viewName, "help", ThemePropertyCategory::All);
}

