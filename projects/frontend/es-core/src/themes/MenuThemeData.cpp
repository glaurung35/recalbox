#include <themes/MenuThemeData.h>

void MenuThemeData::Load(ThemeData& mainTheme)
{
  Reset();

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menuSize", "menuSize"); elem != nullptr)
		if (elem->HasProperty("height"))
			mSize.height = elem->AsFloat("height");

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menubg", "menuBackground"); elem != nullptr)
	{
		if (elem->HasProperty("path"))
			mBackground.path = Path(elem->AsString("path"));
		if (elem->HasProperty("fadePath"))
			mBackground.fadePath = Path(elem->AsString("fadePath"));
		if (elem->HasProperty("color"))
			mBackground.color = (unsigned int)elem->AsInt("color");
	}

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menutitle", "menuText"); elem != nullptr)
	{
		if(elem->HasProperty("fontPath") || elem->HasProperty("fontSize"))
			mTitle.font = Font::getFromTheme(*elem, ThemePropertiesType::All, Font::get(FONT_SIZE_LARGE));
		if(elem->HasProperty("color"))
			mTitle.color = (unsigned int)elem->AsInt("color");
	}

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menufooter", "menuText"); elem != nullptr)
	{
		if(elem->HasProperty("fontPath") || elem->HasProperty("fontSize"))
			mFooter.font = Font::getFromTheme(*elem, ThemePropertiesType::All, Font::get(FONT_SIZE_SMALL));
		if(elem->HasProperty("color"))
			mFooter.color = (unsigned int)elem->AsInt("color");
	}

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menutext", "menuText"); elem != nullptr)
	{
		if(elem->HasProperty("fontPath") || elem->HasProperty("fontSize"))
			mText.font = Font::getFromTheme(*elem, ThemePropertiesType::All, Font::get(FONT_SIZE_MEDIUM));

		if(elem->HasProperty("color"))
			mText.color = (unsigned int)elem->AsInt("color");
		if(elem->HasProperty("separatorColor"))
			mText.separatorColor = (unsigned int)elem->AsInt("separatorColor");
		if(elem->HasProperty("selectedColor"))
			mText.selectedColor = (unsigned int)elem->AsInt("selectedColor");
		if(elem->HasProperty("selectorColor"))
			mText.selectorColor = (unsigned int)elem->AsInt("selectorColor");
	}

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menutextsmall", "menuTextSmall"); elem != nullptr)
	{
		if(elem->HasProperty("fontPath") || elem->HasProperty("fontSize"))
      mTextSmall.font = Font::getFromTheme(*elem, ThemePropertiesType::All, Font::get(FONT_SIZE_SMALL));

		if(elem->HasProperty("color"))
			mTextSmall.color = (unsigned int)elem->AsInt("color");
		if(elem->HasProperty("selectedColor"))
			mText.selectedColor = (unsigned int)elem->AsInt("selectedColor");
		if(elem->HasProperty("selectorColor"))
			mText.selectedColor = (unsigned int)elem->AsInt("selectorColor");
	}

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menubutton", "menuButton"); elem != nullptr)
	{
		if(elem->HasProperty("path"))
			mIconElement.button = elem->AsString("path");
		if(elem->HasProperty("filledPath"))
			mIconElement.button_filled = elem->AsString("filledPath");
	}

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menuswitch", "menuSwitch"); elem != nullptr)
	{
		if(elem->HasProperty("pathOn"))
			mIconElement.on = elem->AsString("pathOn");
		if(elem->HasProperty("pathOff"))
			mIconElement.off = elem->AsString("pathOff");
	}

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menuslider", "menuSlider"); elem != nullptr)
		if(elem->HasProperty("path"))
			mIconElement.knob = elem->AsString("path");

	if (const ThemeElement* elem = mainTheme.getElement("menu", "menuicons", "menuIcons"); elem != nullptr)
  {
		if (elem->HasProperty("iconKodi"))
			mIconSet.kodi = elem->AsString("iconKodi");

		if (elem->HasProperty("iconSystem"))
      mIconSet.system = elem->AsString("iconSystem");

		if (elem->HasProperty("iconUpdates"))
      mIconSet.updates = elem->AsString("iconUpdates");

		if (elem->HasProperty("iconGames"))
      mIconSet.games = elem->AsString("iconGames");

		if (elem->HasProperty("iconControllers"))
      mIconSet.controllers = elem->AsString("iconControllers");

		if (elem->HasProperty("iconUI"))
      mIconSet.ui = elem->AsString("iconUI");

		if (elem->HasProperty("iconSound"))
      mIconSet.sound = elem->AsString("iconSound");

		if (elem->HasProperty("iconNetwork"))
      mIconSet.network = elem->AsString("iconNetwork");

		if (elem->HasProperty("iconScraper"))
      mIconSet.scraper = elem->AsString("iconScraper");

		if (elem->HasProperty("iconAdvanced"))
      mIconSet.advanced = elem->AsString("iconAdvanced");

		if (elem->HasProperty("iconQuit"))
      mIconSet.quit = elem->AsString("iconQuit");

    if (elem->HasProperty("iconRestart"))
      mIconSet.restart = elem->AsString("iconRestart");

    if (elem->HasProperty("iconShutdown"))
      mIconSet.shutdown = elem->AsString("iconShutdown");

    if (elem->HasProperty("iconFastShutdown"))
      mIconSet.fastshutdown = elem->AsString("iconFastShutdown");

    if (elem->HasProperty("iconLicense"))
      mIconSet.license = elem->AsString("iconLicense");

    if (elem->HasProperty("iconRecalboxRGBDual"))
      mIconSet.recalboxrgbdual = elem->AsString("iconRecalboxRGBDual");

    if (elem->HasProperty("iconTate"))
      mIconSet.tate = elem->AsString("iconTate");

    if (elem->HasProperty("iconArcade"))
      mIconSet.arcade = elem->AsString("iconArcade");

    if (elem->HasProperty("iconDownload"))
      mIconSet.download = elem->AsString("iconDownload");
	}
}

void MenuThemeData::Reset()
{
  mBackground  = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, Path(":/frame.png"), Path(":/scroll_gradient.png"), nullptr                     };
  mTitle       = { 0x555555FF, 0x555555FF, 0x555555FF, 0xFFFFFFFF, Path::Empty        , Path::Empty                  , Font::get(FONT_SIZE_LARGE)  };
  mFooter      = { 0xC6C6C6FF, 0xC6C6C6FF, 0xC6C6C6FF, 0xFFFFFFFF, Path::Empty        , Path::Empty                  , Font::get(FONT_SIZE_SMALL)  };
  mText        = { 0x777777FF, 0xFFFFFFFF, 0x878787FF, 0xC6C7C6FF, Path::Empty        , Path::Empty                  , Font::get(FONT_SIZE_MEDIUM) };
  mTextSmall   = { 0x777777FF, 0xFFFFFFFF, 0x878787FF, 0xC6C7C6FF, Path::Empty        , Path::Empty                  , Font::get(FONT_SIZE_SMALL)  };
  mIconElement = { Path(":/button.png"), Path(":/button_filled.png"), Path(":/on.svg"), Path(":/off.svg"), Path(":/option_arrow.svg"), Path(":/arrow.svg"), Path(":/slider_knob.svg") };
  mSize        = { .height =  0.85f };
}
