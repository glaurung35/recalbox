//
// Created by bkg2k on 31/10/2020.
//

#include <MainRunner.h>
#include "MenuThemeConfiguration.h"
#include "guis/MenuMessages.h"

MenuThemeConfiguration::MenuThemeConfiguration(WindowManager& window, const String& themeName)
  : Menu(window, _("THEME CONFIGURATION"))
  , mThemeName(themeName)
{
}

void MenuThemeConfiguration::BuildMenuItems()
{
  IniFile::PurgeKey(mThemeName);
  const ThemeData& theme = ThemeManager::Instance().Main();
  ItemSelector<String>* colorSet     = BuildSelector(_("THEME COLORSET"    ), _(MENUMESSAGE_UI_THEME_COLORSET_MSG),
                                                     RecalboxConf::Instance().GetThemeColorSet(mThemeName),
                                                     theme.GetSubSetValues("colorset")    , Components::ColorSet);
  ItemSelector<String>* iconSet      = BuildSelector(_("THEME ICONSET"     ), _(MENUMESSAGE_UI_THEME_ICONSET_MSG),
                                                     RecalboxConf::Instance().GetThemeIconSet(mThemeName),
                                                     theme.GetSubSetValues("iconset")     , Components::IconSet);
  ItemSelector<String>* menuSet      = BuildSelector(_("THEME MENU"        ), _(MENUMESSAGE_UI_THEME_MENU_MSG),
                                                     RecalboxConf::Instance().GetThemeMenuSet(mThemeName),
                                                     theme.GetSubSetValues("menu")        , Components::MenuSet);
  ItemSelector<String>* systemView   = BuildSelector(_("THEME SYSTEMVIEW"  ), _(MENUMESSAGE_UI_THEME_SYSTEMVIEW_MSG),
                                                     RecalboxConf::Instance().GetThemeSystemView(mThemeName),
                                                     theme.GetSubSetValues("systemview")  , Components::SystemView);
  ItemSelector<String>* gameListView = BuildSelector(_("THEME GAMELISTVIEW"), _(MENUMESSAGE_UI_THEME_GAMELISTVIEW_MSG),
                                                     RecalboxConf::Instance().GetThemeGamelistView(mThemeName),
                                                     theme.GetSubSetValues("gamelistview"), Components::GamelistView);
  ItemSelector<String>* gameClipView = BuildSelector(_("THEME GAMECLIPVIEW"), _(MENUMESSAGE_UI_THEME_GAMECLIPVIEW_MSG),
                                                     RecalboxConf::Instance().GetThemeGameClipView(mThemeName),
                                                     theme.GetSubSetValues("gameclipview"), Components::GameClipView);

  // Empty?
  if (colorSet == nullptr &&
      iconSet == nullptr &&
      menuSet == nullptr &&
      systemView == nullptr &&
      gameListView == nullptr &&
      gameClipView == nullptr)
  {
    Close();
    mWindow.displayMessage(_("THIS THEME HAS NO OPTION"));
  }
}

void MenuThemeConfiguration::MenuSingleChanged(int id, int index, const String& value)
{
  (void)index;
  switch((Components)id)
  {
    case Components::ColorSet: RecalboxConf::Instance().SetThemeColorSet(mThemeName, value).Save(); break;
    case Components::IconSet: RecalboxConf::Instance().SetThemeIconSet(mThemeName, value).Save(); break;
    case Components::MenuSet: RecalboxConf::Instance().SetThemeMenuSet(mThemeName, value).Save(); break;
    case Components::SystemView: RecalboxConf::Instance().SetThemeSystemView(mThemeName, value).Save(); break;
    case Components::GamelistView: RecalboxConf::Instance().SetThemeGamelistView(mThemeName, value).Save(); break;
    case Components::GameClipView: RecalboxConf::Instance().SetThemeGameClipView(mThemeName, value).Save(); break;
  }
  ThemeManager::Instance().DoThemeChange(&mWindow);
}

ItemSelector<String>* MenuThemeConfiguration::BuildSelector(const String& label, const String& help, const String& selected, const String::List& items, Components id)
{
  // No option?
  if (items.empty()) return nullptr;

  bool found = false;
  String realSelected;
  for(const String& s : items) if (s == selected) { found = true; realSelected = s; break; }
  if (!found) realSelected = items.front();

  // Build list
  SelectorEntry<String>::List list;
  for (const String& s : items) list.push_back({ s, s, s == realSelected });
  // Try numeric sorting, else  use an alphanumeric sort
  if (!TrySortNumerically(list))
    std::sort(list.begin(), list.end(), [] (const SelectorEntry<String>& a, const SelectorEntry<String>& b) -> bool { return a.mText < b.mText; });

  if (!items.empty())
    return AddList<String>(label, (int)id, this, list, realSelected, list.front().mValue, help);

  return nullptr;
}

bool MenuThemeConfiguration::TrySortNumerically(SelectorEntry<String>::List& list)
{
  HashMap<String, int> nameToNumeric;
  for(const SelectorEntry<String>& item : list)
  {
    if (item.mText.empty()) return false;
    size_t pos = item.mText.find_first_not_of("0123456789");
    if (pos == 0) return false;
    nameToNumeric[item.mText] = (pos == std::string::npos) ? item.mText.AsInt() : item.mText.AsInt(item.mText[pos]);
  }

  std::sort(list.begin(), list.end(), [&nameToNumeric] (const SelectorEntry<String>& a, const SelectorEntry<String>& b) -> bool { return nameToNumeric[a.mText] < nameToNumeric[b.mText]; });
  return true;
}
