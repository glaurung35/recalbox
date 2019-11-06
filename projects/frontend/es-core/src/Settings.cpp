#include "Settings.h"
#include "utils/Log.h"
#include "pugixml/pugixml.hpp"
#include "platform.h"
#include "RootFolders.h"
//#include <boost/assign.hpp>

Settings *Settings::sInstance = nullptr;

static std::string& SettingsNotToSave()
{
  static const char* _SettingsNotToSave[] =
  {
    "Debug",
    "DebugGrid",
    "DebugText",
    "ShowExit",
    "Windowed",
    "VSync",
    "HideConsole",
    "IgnoreGamelist",
    "UpdateCommand",
    "UpdateServer",
    "VersionFile",
    "SharePartition",
    "RecalboxSettingScript",
    "RecalboxConfigScript",
    "LastVersionFile",
    "VersionMessage",
    "MusicDirectory",
    "ThemeChanged",
    "ThemeHasMenuView",
    "Arch",
  };
  static std::string _SettingsNotToSaveBuilt;
  if (_SettingsNotToSaveBuilt.empty())
    for(const char* snts: _SettingsNotToSave)
    {
      _SettingsNotToSaveBuilt += snts;
      _SettingsNotToSaveBuilt += ' ';
    }
  return _SettingsNotToSaveBuilt;
}

Settings::Settings()
{
  setDefaults();
  loadFile();
}

Settings *Settings::getInstance() {
    if (sInstance == nullptr)
        sInstance = new Settings();

    return sInstance;
}

void Settings::setDefaults() {
    mBoolMap.clear();
    mIntMap.clear();

    mBoolMap["BackgroundJoystickInput"] = false;
    mBoolMap["DrawFramerate"] = false;
    mBoolMap["ShowExit"] = true;
    mBoolMap["Windowed"] = false;
    mBoolMap["UseOSK"] = true;
    mBoolMap["ShowClock"] = false;

#ifdef _RPI_
	// don't enable VSync by default on the Pi, since it already
	// has trouble trying to render things at 60fps in certain menus
	mBoolMap["VSync"] = false;
    #else
    mBoolMap["VSync"] = true;
#endif

    mBoolMap["ShowHelpPrompts"] = true;
    mBoolMap["ScrapeRatings"] = true;
    mBoolMap["MixImages"] = true;
    mBoolMap["IgnoreGamelist"] = false;
    mBoolMap["HideConsole"] = true;
    mBoolMap["QuickSystemSelect"] = true;
    mBoolMap["FavoritesOnly"] = false;
    mBoolMap["ShowHidden"] = false;

    mBoolMap["Debug"] = false;
    mBoolMap["DebugGrid"] = false;
    mBoolMap["DebugText"] = false;
    mBoolMap["MoveCarousel"] = true;
    mBoolMap["Overscan"] = false;

    mIntMap["ScreenSaverTime"] = 5 * 60 * 1000; // 5 minutes
	mIntMap["MusicPopupTime"] = 3;
    mIntMap["ScraperResizeWidth"] = 400;
    mIntMap["ScraperResizeHeight"] = 0;
    mIntMap["SystemVolume"] = 96;
	mIntMap["HelpPopupTime"] = 4;
    mIntMap["NetplayPopupTime"] = 4;
	mIntMap["MaxVRAM"] = 80;

    mStringMap["TransitionStyle"] = "fade";
    mStringMap["PopupPosition"] = "Top/Right";
    mStringMap["ThemeSet"] = "";
	mStringMap["ThemeColorSet"] = "";
	mStringMap["ThemeIconSet"] = "";
	mStringMap["ThemeMenu"] = "";
	mStringMap["ThemeSystemView"] = "";
	mStringMap["ThemeGamelistView"] = "";
	mStringMap["ThemeRegionName"] = "eu";
    mStringMap["ScreenSaverBehavior"] = "dim";
    mStringMap["Scraper"] = "Screenscraper";
    mStringMap["Lang"] = "en_US";
    mStringMap["INPUT P1"] = "DEFAULT";
    mStringMap["INPUT P2"] = "DEFAULT";
    mStringMap["INPUT P3"] = "DEFAULT";
    mStringMap["INPUT P4"] = "DEFAULT";
    mStringMap["Overclock"] = "none";
    mStringMap["UpdateCommand"] = "/recalbox/scripts/recalbox-upgrade.sh";
    mStringMap["UpdateServer"] = "archive.recalbox.com";
    mStringMap["VersionFile"] = "/recalbox/recalbox.version";
    mStringMap["UpdateVersionFile"] = "/recalbox/share/system/upgrade/recalbox.version.update";
    mStringMap["SharePartition"] = "/recalbox/share/";
    mStringMap["RecalboxSettingScript"] = "/recalbox/scripts/recalbox-config.sh";
    mStringMap["RecalboxUpgradeScript"] = "/recalbox/scripts/upgrade/recalbox-upgrade.sh";
    mStringMap["LastChangelog"] = "/recalbox/share/system/recalbox.changelog.done";
    mStringMap["UpdateChangelog"] = "/recalbox/share/system/upgrade/recalbox.changelog.update";
    mStringMap["Changelog"] = "/recalbox/recalbox.changelog";
    mStringMap["MusicDirectory"] = "/recalbox/share/music/";

}

template<typename K, typename V>
void saveMap(pugi::xml_node &node, std::map<K, V> &map, const char *type) {
    for (auto iter = map.begin(); iter != map.end(); iter++) {
        // key is on the "don't save" list, so don't save it
        if (SettingsNotToSave().find(iter->first) != std::string::npos)
            continue;

        pugi::xml_node parent_node = node.append_child(type);
        parent_node.append_attribute("name").set_value(iter->first.c_str());
        parent_node.append_attribute("value").set_value(iter->second);
    }
}

void Settings::saveFile()
{
    const Path path = RootFolders::DataRootFolder / "/system/.emulationstation/es_settings.cfg";

    pugi::xml_document doc;

    pugi::xml_node config = doc.append_child("config");
    
    saveMap<std::string, bool>(config, mBoolMap, "bool");
    saveMap<std::string, int>(config, mIntMap, "int");
    saveMap<std::string, float>(config, mFloatMap, "float");

    //saveMap<std::string, std::string>(config, mStringMap, "string");
    for (auto & iter : mStringMap) {
        pugi::xml_node node = config.append_child("string");
        node.append_attribute("name").set_value(iter.first.c_str());
        node.append_attribute("value").set_value(iter.second.c_str());
    }

    doc.save_file(path.ToChars());
}

void Settings::loadFile()
{
    const Path path = RootFolders::DataRootFolder / "/system/.emulationstation/es_settings.cfg";

    if (!path.Exists()) return;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.ToChars());
    if (!result) {
        LOG(LogError) << "Could not parse Settings file!\n   " << result.description();
        return;
    }

    pugi::xml_node config = doc.child("config");
    if(config != nullptr) { /* correct file format, having a config root node */
      for (pugi::xml_node node = config.child("bool"); node != nullptr; node = node.next_sibling("bool"))
        setBool(node.attribute("name").as_string(), node.attribute("value").as_bool());
      for (pugi::xml_node node = config.child("int"); node != nullptr; node = node.next_sibling("int"))
        setInt(node.attribute("name").as_string(), node.attribute("value").as_int());
      for (pugi::xml_node node = config.child("float"); node != nullptr; node = node.next_sibling("float"))
        setFloat(node.attribute("name").as_string(), node.attribute("value").as_float());
      for (pugi::xml_node node = config.child("string"); node != nullptr; node = node.next_sibling("string"))
        setString(node.attribute("name").as_string(), node.attribute("value").as_string());
    } else { /* the old format, without the root config node -- keep for a transparent upgrade */
      for (pugi::xml_node node = doc.child("bool"); node != nullptr; node = node.next_sibling("bool"))
        setBool(node.attribute("name").as_string(), node.attribute("value").as_bool());
      for (pugi::xml_node node = doc.child("int"); node != nullptr; node = node.next_sibling("int"))
        setInt(node.attribute("name").as_string(), node.attribute("value").as_int());
      for (pugi::xml_node node = doc.child("float"); node != nullptr; node = node.next_sibling("float"))
        setFloat(node.attribute("name").as_string(), node.attribute("value").as_float());
      for (pugi::xml_node node = doc.child("string"); node != nullptr; node = node.next_sibling("string"))
        setString(node.attribute("name").as_string(), node.attribute("value").as_string());
    }
}

//Print a warning message if the setting we're trying to get doesn't already exist in the map, then return the value in the map.
#define SETTINGS_GETSET(type, mapName, getMethodName, setMethodName) type Settings::getMethodName(const std::string& name) \
{ \
    if(mapName.find(name) == mapName.end()) \
    { \
        LOG(LogError) << "Tried to use unset setting " << name << "!"; \
    } \
    return mapName[name]; \
} \
void Settings::setMethodName(const std::string& name, type value) \
{ \
    mapName[name] = value; \
}

SETTINGS_GETSET(bool, mBoolMap, getBool, setBool)

SETTINGS_GETSET(int, mIntMap, getInt, setInt)

SETTINGS_GETSET(float, mFloatMap, getFloat, setFloat)

SETTINGS_GETSET(const std::string&, mStringMap, getString, setString)
