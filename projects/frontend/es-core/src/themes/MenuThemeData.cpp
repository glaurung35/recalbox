#include <themes/MenuThemeData.h>

void MenuThemeData::Load(ThemeData& mainTheme)
{
  Reset();

	if (const ThemeElement* elem = mainTheme.Element("menu", "menuSize", ThemeElementType::MenuSize, ThemeElementType::None); elem != nullptr)
		if (elem->HasProperty(ThemePropertyName::Height))
			mSize.height = elem->AsFloat(ThemePropertyName::Height);

	if (const ThemeElement* elem = mainTheme.Element("menu", "menubg", ThemeElementType::MenuBackground, ThemeElementType::None); elem != nullptr)
	{
		if (elem->HasProperty(ThemePropertyName::Path))
			mBackground.path = Path(elem->AsString(ThemePropertyName::Path));
		if (elem->HasProperty(ThemePropertyName::FadePath))
			mBackground.fadePath = Path(elem->AsString(ThemePropertyName::FadePath));
		if (elem->HasProperty(ThemePropertyName::Color))
			mBackground.color = (unsigned int)elem->AsInt(ThemePropertyName::Color);
	}

	if (const ThemeElement* elem = mainTheme.Element("menu", "menutitle", ThemeElementType::MenuText, ThemeElementType::None); elem != nullptr)
	{
		if(elem->HasProperty(ThemePropertyName::FontPath) || elem->HasProperty(ThemePropertyName::FontSize))
			mTitle.font = Font::getFromTheme(*elem, ThemePropertyCategory::All, Font::get(FONT_SIZE_LARGE));
		if(elem->HasProperty(ThemePropertyName::Color))
			mTitle.color = (unsigned int)elem->AsInt(ThemePropertyName::Color);
	}

	if (const ThemeElement* elem = mainTheme.Element("menu", "menufooter", ThemeElementType::MenuText, ThemeElementType::None); elem != nullptr)
	{
    if(elem->HasProperty(ThemePropertyName::FontPath) || elem->HasProperty(ThemePropertyName::FontSize))
			mFooter.font = Font::getFromTheme(*elem, ThemePropertyCategory::All, Font::get(FONT_SIZE_SMALL));
		if(elem->HasProperty(ThemePropertyName::Color))
			mFooter.color = (unsigned int)elem->AsInt(ThemePropertyName::Color);
	}

	if (const ThemeElement* elem = mainTheme.Element("menu", "menutext", ThemeElementType::MenuText, ThemeElementType::None); elem != nullptr)
	{
    if(elem->HasProperty(ThemePropertyName::FontPath) || elem->HasProperty(ThemePropertyName::FontSize))
			mText.font = Font::getFromTheme(*elem, ThemePropertyCategory::All, Font::get(FONT_SIZE_MEDIUM));

		if(elem->HasProperty(ThemePropertyName::Color))          mText.color = (unsigned int)elem->AsInt(ThemePropertyName::Color);
		if(elem->HasProperty(ThemePropertyName::SeparatorColor)) mText.separatorColor = (unsigned int)elem->AsInt(ThemePropertyName::SeparatorColor);
		if(elem->HasProperty(ThemePropertyName::SelectedColor))  mText.selectedColor = (unsigned int)elem->AsInt(ThemePropertyName::SelectedColor);
		if(elem->HasProperty(ThemePropertyName::SelectorColor))  mText.selectorColor = (unsigned int)elem->AsInt(ThemePropertyName::SelectorColor);
	}

	if (const ThemeElement* elem = mainTheme.Element("menu", "menutextsmall", ThemeElementType::MenuTextSmall, ThemeElementType::None); elem != nullptr)
	{
    if(elem->HasProperty(ThemePropertyName::FontPath) || elem->HasProperty(ThemePropertyName::FontSize))
      mTextSmall.font = Font::getFromTheme(*elem, ThemePropertyCategory::All, Font::get(FONT_SIZE_SMALL));

		if(elem->HasProperty(ThemePropertyName::Color))         mTextSmall.color = (unsigned int)elem->AsInt(ThemePropertyName::Color);
		if(elem->HasProperty(ThemePropertyName::SelectedColor)) mText.selectedColor = (unsigned int)elem->AsInt(ThemePropertyName::SelectedColor);
		if(elem->HasProperty(ThemePropertyName::SelectorColor)) mText.selectedColor = (unsigned int)elem->AsInt(ThemePropertyName::SelectorColor);
	}

	if (const ThemeElement* elem = mainTheme.Element("menu", "menubutton", ThemeElementType::MenuButton, ThemeElementType::None); elem != nullptr)
	{
		if(elem->HasProperty(ThemePropertyName::Path))       mIconElement.button = elem->AsString(ThemePropertyName::Path);
		if(elem->HasProperty(ThemePropertyName::FilledPath)) mIconElement.button_filled = elem->AsString(ThemePropertyName::FilledPath);
	}

	if (const ThemeElement* elem = mainTheme.Element("menu", "menuswitch", ThemeElementType::MenuSwitch, ThemeElementType::None); elem != nullptr)
	{
		if(elem->HasProperty(ThemePropertyName::PathOn))  mIconElement.on = elem->AsString(ThemePropertyName::PathOn);
		if(elem->HasProperty(ThemePropertyName::PathOff)) mIconElement.off = elem->AsString(ThemePropertyName::PathOff);
	}

	if (const ThemeElement* elem = mainTheme.Element("menu", "menuslider", ThemeElementType::MenuSlider, ThemeElementType::None); elem != nullptr)
		if (elem->HasProperty(ThemePropertyName::Path)) mIconElement.knob = elem->AsString(ThemePropertyName::Path);

	if (const ThemeElement* elem = mainTheme.Element("menu", "menuicons", ThemeElementType::MenuIcons, ThemeElementType::None); elem != nullptr)
  {
		if (elem->HasProperty(ThemePropertyName::IconKodi))            mIconSet.mKodi = elem->AsString(ThemePropertyName::IconKodi);
		if (elem->HasProperty(ThemePropertyName::IconSystem))          mIconSet.mSystem = elem->AsString(ThemePropertyName::IconSystem);
		if (elem->HasProperty(ThemePropertyName::IconUpdates))         mIconSet.mUpdates = elem->AsString(ThemePropertyName::IconUpdates);
		if (elem->HasProperty(ThemePropertyName::IconGames))           mIconSet.mGames = elem->AsString(ThemePropertyName::IconGames);
		if (elem->HasProperty(ThemePropertyName::IconControllers))     mIconSet.mControllers = elem->AsString(ThemePropertyName::IconControllers);
		if (elem->HasProperty(ThemePropertyName::IconUI))              mIconSet.mUI = elem->AsString(ThemePropertyName::IconUI);
		if (elem->HasProperty(ThemePropertyName::IconSound))           mIconSet.mSound = elem->AsString(ThemePropertyName::IconSound);
		if (elem->HasProperty(ThemePropertyName::IconNetwork))         mIconSet.mNetwork = elem->AsString(ThemePropertyName::IconNetwork);
    if (elem->HasProperty(ThemePropertyName::IconScraper))         mIconSet.mScraper = elem->AsString(ThemePropertyName::IconScraper);
		if (elem->HasProperty(ThemePropertyName::IconAdvanced))        mIconSet.mAdvanced = elem->AsString(ThemePropertyName::IconAdvanced);
		if (elem->HasProperty(ThemePropertyName::IconQuit))            mIconSet.mQuit = elem->AsString(ThemePropertyName::IconQuit);
    if (elem->HasProperty(ThemePropertyName::IconRestart))         mIconSet.mRestart = elem->AsString(ThemePropertyName::IconRestart);
    if (elem->HasProperty(ThemePropertyName::IconShutdown))        mIconSet.mShutdown = elem->AsString(ThemePropertyName::IconShutdown);
    if (elem->HasProperty(ThemePropertyName::IconFastShutdown))    mIconSet.mFastShutdown = elem->AsString(ThemePropertyName::IconFastShutdown);
    if (elem->HasProperty(ThemePropertyName::IconLicense))         mIconSet.mLicense = elem->AsString(ThemePropertyName::IconLicense);
    if (elem->HasProperty(ThemePropertyName::IconRecalboxRGBDual)) mIconSet.mRecalboxRGBDual = elem->AsString(ThemePropertyName::IconRecalboxRGBDual);
    if (elem->HasProperty(ThemePropertyName::IconTate))            mIconSet.mTate = elem->AsString(ThemePropertyName::IconTate);
    if (elem->HasProperty(ThemePropertyName::IconArcade))          mIconSet.mArcade = elem->AsString(ThemePropertyName::IconArcade);
    if (elem->HasProperty(ThemePropertyName::IconDownload))        mIconSet.mDownload = elem->AsString(ThemePropertyName::IconDownload);
	}
}

void MenuThemeData::Reset()
{
  mBackground  = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, Path(":/frame.png"), Path(":/scroll_gradient.png"), nullptr                     };
  mTitle       = { 0x555555FF, 0x555555FF, 0x555555FF, 0xFFFFFFFF, Path::Empty        , Path::Empty                  , Font::get(FONT_SIZE_LARGE)  };
  mFooter      = { 0xC6C6C6FF, 0xC6C6C6FF, 0xC6C6C6FF, 0xFFFFFFFF, Path::Empty        , Path::Empty                  , Font::get(FONT_SIZE_SMALL)  };
  mText        = { 0x777777FF, 0xFFFFFFFF, 0x878787FF, 0xC6C7C6FF, Path::Empty        , Path::Empty                  , Font::get(FONT_SIZE_MEDIUM) };
  mTextSmall   = { 0x777777FF, 0xFFFFFFFF, 0x878787FF, 0xC6C7C6FF, Path::Empty        , Path::Empty                  , Font::get(FONT_SIZE_SMALL)  };
  mIconSet.Reset();
  mIconElement = { Path(":/button.png"), Path(":/button_filled.png"), Path(":/on.svg"), Path(":/off.svg"), Path(":/option_arrow.svg"), Path(":/arrow.svg"), Path(":/slider_knob.svg") };
  mSize        = { .height =  0.85f };
}
