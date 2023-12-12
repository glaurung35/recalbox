#include "ThemeData.h"
#include "resources/TextureResource.h"
#include "pugixml/pugixml.hpp"
#include <components/VideoComponent.h>
#include <MainRunner.h>
#include "RootFolders.h"
#include "MenuThemeData.h"
#include "components/TextScrollComponent.h"
#include "components/BoxComponent.h"

bool ThemeData::sThemeHasMenuView = true;
bool ThemeData::sThemeHasHelpSystem = true;

HashSet<String>& ThemeData::SupportedViews()
{
  static HashSet<String> sSupportedViews;
  if (sSupportedViews.empty())
    for(const String& value : String("system,basic,detailed,menu,gameclip").Split(','))
      sSupportedViews.insert(value);
  return sSupportedViews;
}

HashSet<String>& ThemeData::SupportedFeatures()
{
  static HashSet<String> sSupportedFeatures;
  if (sSupportedFeatures.empty())
    for(const String& value : String("carousel,z-index").Split(','))
      sSupportedFeatures.insert(value);
  return sSupportedFeatures;
}

HashMap< String, HashMap<String, ThemeData::ElementProperty> >& ThemeData::ElementMap()
{
  static HashMap< String, HashMap<String, ThemeData::ElementProperty> > sElementMap =
  {
    { "image",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "maxSize", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "rotation", ElementProperty::Float },
        { "rotationOrigin", ElementProperty::NormalizedPair },
        { "path", ElementProperty::Path },
        { "tile", ElementProperty::Boolean },
        { "color", ElementProperty::Color },
        { "zIndex", ElementProperty::Float },
        { "disabled", ElementProperty::Boolean },
      },
    },
    { "box",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "rotation", ElementProperty::Float },
        { "rotationOrigin", ElementProperty::NormalizedPair },
        { "color", ElementProperty::Color },
        { "zIndex", ElementProperty::Float },
        { "disabled", ElementProperty::Boolean },
      },
    },
    { "video",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "maxSize", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "rotation", ElementProperty::Float },
        { "rotationOrigin", ElementProperty::NormalizedPair },
        { "path", ElementProperty::Path },
        { "zIndex", ElementProperty::Float },
        { "disabled", ElementProperty::Boolean },
      },
    },
    { "text",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "rotation", ElementProperty::Float },
        { "rotationOrigin", ElementProperty::NormalizedPair },
        { "text", ElementProperty::String },
        { "backgroundColor", ElementProperty::Color },
        { "fontPath", ElementProperty::Path },
        { "fontSize", ElementProperty::Float },
        { "color", ElementProperty::Color },
        { "alignment", ElementProperty::String },
        { "forceUppercase", ElementProperty::Boolean },
        { "lineSpacing", ElementProperty::Float },
        { "value", ElementProperty::String },
        { "zIndex", ElementProperty::Float },
        { "disabled", ElementProperty::Boolean },
      },
    },
    { "textscroll",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "rotation", ElementProperty::Float },
        { "rotationOrigin", ElementProperty::NormalizedPair },
        { "text", ElementProperty::String },
        { "backgroundColor", ElementProperty::Color },
        { "fontPath", ElementProperty::Path },
        { "fontSize", ElementProperty::Float },
        { "color", ElementProperty::Color },
        { "alignment", ElementProperty::String },
        { "forceUppercase", ElementProperty::Boolean },
        { "value", ElementProperty::String },
        { "zIndex", ElementProperty::Float },
        { "disabled", ElementProperty::Boolean },
      },
    },
    { "textlist",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "selectorHeight", ElementProperty::Float },
        { "selectorOffsetY", ElementProperty::Float },
        { "selectorColor", ElementProperty::Color },
        { "selectorImagePath", ElementProperty::Path },
        { "selectorImageTile", ElementProperty::Boolean },
        { "selectedColor", ElementProperty::Color },
        { "primaryColor", ElementProperty::Color },
        { "secondaryColor", ElementProperty::Color },
        { "fontPath", ElementProperty::Path },
        { "fontSize", ElementProperty::Float },
        { "scrollSound", ElementProperty::Path },
        { "alignment", ElementProperty::String },
        { "horizontalMargin", ElementProperty::Float },
        { "forceUppercase", ElementProperty::Boolean },
        { "lineSpacing", ElementProperty::Float },
        { "zIndex", ElementProperty::Float },
      },
    },
    { "container",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "zIndex", ElementProperty::Float },
        { "disabled", ElementProperty::Boolean },
      },
    },
    { "ninepatch",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "path", ElementProperty::Path },
        { "zIndex", ElementProperty::Float },
        { "disabled", ElementProperty::Boolean },
      },
    },
    { "datetime",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "color", ElementProperty::Color },
        { "fontPath", ElementProperty::Path },
        { "fontSize", ElementProperty::Float },
        { "alignment", ElementProperty::String },
        { "forceUppercase", ElementProperty::Boolean },
        { "zIndex", ElementProperty::Float },
        { "disabled", ElementProperty::Boolean },
        { "display", ElementProperty::String },
      },
    },
    { "rating",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "size", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "rotation", ElementProperty::Float },
        { "rotationOrigin", ElementProperty::NormalizedPair },
        { "filledPath", ElementProperty::Path },
        { "unfilledPath", ElementProperty::Path },
        { "zIndex", ElementProperty::Float },
        { "disabled", ElementProperty::Boolean },
      },
    },
    { "sound",
      {
        { "path", ElementProperty::Path },
      },
    },
    { "helpsystem",
      {
        { "pos", ElementProperty::NormalizedPair },
        { "textColor", ElementProperty::Color },
        { "iconColor", ElementProperty::Color },
        { "fontPath", ElementProperty::Path },
        { "fontSize", ElementProperty::Float },
        { "iconUpDown", ElementProperty::Path },
        { "iconLeftRight", ElementProperty::Path },
        { "iconUpDownLeftRight", ElementProperty::Path },
        { "iconA", ElementProperty::Path },
        { "iconB", ElementProperty::Path },
        { "iconX", ElementProperty::Path },
        { "iconY", ElementProperty::Path },
        { "iconL", ElementProperty::Path },
        { "iconR", ElementProperty::Path },
        { "iconStart", ElementProperty::Path },
        { "iconSelect", ElementProperty::Path },
      },
    },
    { "carousel",
      {
        { "type", ElementProperty::String },
        { "size", ElementProperty::NormalizedPair },
        { "pos", ElementProperty::NormalizedPair },
        { "origin", ElementProperty::NormalizedPair },
        { "color", ElementProperty::Color },
        { "logoScale", ElementProperty::Float },
        { "logoRotation", ElementProperty::Float },
        { "logoRotationOrigin", ElementProperty::NormalizedPair },
        { "logoSize", ElementProperty::NormalizedPair },
        { "logoAlignment", ElementProperty::String },
        { "maxLogoCount", ElementProperty::Float },
        { "defaultTransition", ElementProperty::String },
        { "zIndex", ElementProperty::Float },
      },
    },
    { "menuBackground",
      {
        { "color", ElementProperty::Color },
        { "path", ElementProperty::Path },
        { "fadePath", ElementProperty::Path },
      },
    },
    { "menuIcons",
      {
        { "iconKodi", ElementProperty::Path },
        { "iconSystem", ElementProperty::Path },
        { "iconSystem", ElementProperty::Path },
        { "iconUpdates", ElementProperty::Path },
        { "iconControllers", ElementProperty::Path },
        { "iconGames", ElementProperty::Path },
        { "iconUI", ElementProperty::Path },
        { "iconSound", ElementProperty::Path },
        { "iconNetwork", ElementProperty::Path },
        { "iconScraper", ElementProperty::Path },
        { "iconAdvanced", ElementProperty::Path },
        { "iconQuit", ElementProperty::Path },
        { "iconRestart", ElementProperty::Path },
        { "iconShutdown", ElementProperty::Path },
        { "iconFastShutdown", ElementProperty::Path },
        { "iconLicense", ElementProperty::Path },
        { "iconRecalboxRGBDual", ElementProperty::Path },
        { "iconTate", ElementProperty::Path },
        { "iconArcade", ElementProperty::Path },
        { "iconDownload", ElementProperty::Path },
      },
    },
    { "menuSwitch",
      {
        { "pathOn", ElementProperty::Path },
        { "pathOff", ElementProperty::Path },
      },
    },
    { "menuSlider",
      {
        { "path", ElementProperty::Path },
      },
    },
    { "menuButton",
      {
        { "path", ElementProperty::Path },
        { "filledPath", ElementProperty::Path },
      },
    },
    { "menuText",
      {
        { "fontPath", ElementProperty::Path },
        { "fontSize", ElementProperty::Float },
        { "color", ElementProperty::Color },
        { "separatorColor", ElementProperty::Color },
        { "selectedColor", ElementProperty::Color },
        { "selectorColor", ElementProperty::Color },
      },
    },
    { "menuTextSmall",
      {
        { "fontPath", ElementProperty::Path },
        { "fontSize", ElementProperty::Float },
        { "color", ElementProperty::Color },
        { "selectedColor", ElementProperty::Color },
        { "selectorColor", ElementProperty::Color },
      },
    },
    { "menuSize",
      {
        { "height", ElementProperty::Float },
      },
    }
  };
  return sElementMap;
}

// helper
unsigned int ThemeData::getHexColor(const char* str)
{
  if(str == nullptr)
  {
    { LOG(LogError) << "[Themes] Empty color"; }
    return 0xFFFFFFFF;
  }

  String string('$');
  string.Append(str);
  int val = 0;
  if ((string.Count() != 7 && string.Count() != 9) || !string.TryAsInt(val))
    { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "Invalid color (bad length, \"" + String(str) + "\" - must be 6 or 8)"; }

  if (string.Count() == 7) val = (val << 8) | 0xFF;
  return (unsigned int)val;
}

ThemeData::ThemeData()
{
  mVersion = 0;
  SetThemeHasMenuView(false);
  mSystemThemeFolder.clear();
  mRandomPath.clear();
}

bool ThemeData::CheckThemeOption(String& selected, const HashMap<String, String>& subsets, const String& subset)
{
  String::List list = sortThemeSubSets(subsets, subset);
  // Empty subset?
  if (subsets.empty()) return false;
  if (list.empty()) return false;
  // Try to fix the value if not found
  bool found = false;
  for(const String& s : list)
    if (s == selected) { found = true; break; }
  if (!found)
  {
    selected = list.front();
    return true;
  }
  return false;
}

void ThemeData::loadFile(const String& systemThemeFolder, const Path& path)
{
  mPaths.push_back(path);

  if(!path.Exists())
  {
    { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "File does not exist!"; }
    return;
  }

  mVersion = 0;
  mViews.clear();

  mSystemThemeFolder = systemThemeFolder;

  String themeName = path.IsDirectory() ? path.Filename() : path.Directory().Filename();
  HashMap<String, String> subSets = getThemeSubSets(themeName);

  bool main = systemThemeFolder.empty();
  bool needSave = false;
  mColorset = RecalboxConf::Instance().GetThemeColorSet(themeName);
  if (main && CheckThemeOption(mColorset, subSets, "colorset")) { RecalboxConf::Instance().SetThemeColorSet(themeName, mColorset); needSave = true; }
  mIconset = RecalboxConf::Instance().GetThemeIconSet(themeName);
  if (main && CheckThemeOption(mIconset, subSets, "iconset")) { RecalboxConf::Instance().SetThemeIconSet(themeName, mIconset); needSave = true; }
  mMenu = RecalboxConf::Instance().GetThemeMenuSet(themeName);
  if (main && CheckThemeOption(mMenu, subSets, "menu")) { RecalboxConf::Instance().SetThemeMenuSet(themeName, mMenu); needSave = true; }
  mSystemview = RecalboxConf::Instance().GetThemeSystemView(themeName);
  if (main && CheckThemeOption(mSystemview, subSets, "systemview")) { RecalboxConf::Instance().SetThemeSystemView(themeName, mSystemview); needSave = true; }
  mGamelistview = RecalboxConf::Instance().GetThemeGamelistView(themeName);
  if (main && CheckThemeOption(mGamelistview, subSets, "gamelistview")) { RecalboxConf::Instance().SetThemeGamelistView(themeName, mGamelistview); needSave = true; }
  mGameClipView = RecalboxConf::Instance().GetThemeGameClipView(themeName);
  if (main && CheckThemeOption(mGameClipView, subSets, "gameclipview")) { RecalboxConf::Instance().SetThemeGameClipView(themeName, mGameClipView); needSave = true; }
  mRegion = RecalboxConf::Instance().GetThemeRegion(themeName);
  if (main && mRegion != "us" && mRegion != "eu" && mRegion != "jp") { mRegion="us"; RecalboxConf::Instance().SetThemeRegion(themeName, mRegion); needSave = true; }
  if (needSave) RecalboxConf::Instance().Save();

  pugi::xml_document doc;
  pugi::xml_parse_result res = doc.load_file(path.ToChars());
  if(!res)
    { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "XML parsing error: \n    " + String(res.description()); }

  pugi::xml_node root = doc.child("theme");
  if(!root)
    { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "Missing <theme> tag!"; }

  // parse version
  mVersion = root.child("formatVersion").text().as_float(-404);
  if(mVersion == -404)
    { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "<formatVersion> tag missing!\n   It's either out of date or you need to add <formatVersion>" + String(CURRENT_THEME_FORMAT_VERSION) + "</formatVersion> inside your <theme> tag."; }

  if(mVersion < MINIMUM_THEME_FORMAT_VERSION)
    { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "Theme uses format version " + String(mVersion, 2) + ". Minimum supported version is " + String(MINIMUM_THEME_FORMAT_VERSION) + '.'; }

  parseIncludes(root);
  parseViews(root);
  parseFeatures(root);
  mPaths.pop_back();
}

void ThemeData::parseIncludes(const pugi::xml_node& root)
{
  for (pugi::xml_node node = root.child("include"); node != nullptr; node = node.next_sibling("include"))
  {
    if (parseSubset(node))
    {
      String errorString;
      String str = resolveSystemVariable(mSystemThemeFolder, node.text().get(), mRandomPath);

      //workaround for an issue in parseincludes introduced by variable implementation
      if (!str.Contains("//"))
      {
        Path path = Path(str).ToAbsolute(mPaths.back().Directory());
        if(!ResourceManager::fileExists(path))
        {
          { LOG(LogWarning) << "[ThemeData] Included file \"" << str << "\" not found! (resolved to \"" << path.ToString() << "\")"; }
          continue;
        }

        errorString += "    from included file \"" + str + "\":\n    ";

        mPaths.push_back(path);

        pugi::xml_document includeDoc;
        pugi::xml_parse_result result = includeDoc.load_file(path.ToChars());
        if(!result)
          { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << errorString + "Error parsing file: \n    " + result.description(); }

        pugi::xml_node newRoot = includeDoc.child("theme");
        if(!newRoot)
          { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << errorString + "Missing <theme> tag!"; }
        parseIncludes(newRoot);
        parseViews(newRoot);
        parseFeatures(newRoot);

        mPaths.pop_back();
      }
    }
  }
}

bool ThemeData::parseSubset(const pugi::xml_node& node)
{
  // No subset?
  if (node.attribute("subset") == nullptr) return true;

  const String subsetAttr = node.attribute("subset").as_string();
  const String nameAttr = node.attribute("name").as_string();

  return (subsetAttr == "colorset" && nameAttr == mColorset) ||
         (subsetAttr == "iconset" && nameAttr == mIconset) ||
         (subsetAttr == "menu" && nameAttr == mMenu) ||
         (subsetAttr == "systemview" && nameAttr == mSystemview) ||
         (subsetAttr == "gamelistview" && nameAttr == mGamelistview) ||
         (subsetAttr == "gameclipview" && nameAttr == mGameClipView);
}

void ThemeData::parseFeatures(const pugi::xml_node& root)
{
  for (pugi::xml_node node = root.child("feature"); node != nullptr; node = node.next_sibling("feature"))
  {
    if(!node.attribute("supported"))
    { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "Feature missing \"supported\" attribute!"; }

    const String supportedAttr = node.attribute("supported").as_string();
    if (SupportedFeatures().contains(supportedAttr))
      parseViews(node);
  }
}

void ThemeData::parseViews(const pugi::xml_node& root)
{
  // parse views
  for (pugi::xml_node node = root.child("view"); node != nullptr; node = node.next_sibling("view"))
  {
    if(!node.attribute("name")) { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "View missing \"name\" attribute!"; }
    for(String& viewName : String(node.attribute("name").as_string()).LowerCase().Split(','))
    {
      viewName.Trim();
      if (viewName == "menu") SetThemeHasMenuView(true);
      if (SupportedViews().contains(viewName))
      {
        ThemeView& view = mViews.insert(std::pair<String, ThemeView>(viewName, ThemeView())).first->second;
        parseView(node, view, false);
      }
    }
  }
}

void ThemeData::parseView(const pugi::xml_node& root, ThemeView& view, bool forcedExtra)
{
  for (pugi::xml_node node = root.first_child(); node != nullptr; node = node.next_sibling())
  {
    if(!node.attribute("name"))
      { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "Element of type \"" + String(node.name()) + R"(" missing "name" attribute!)"; }
    if (String(node.name()) == "helpsystem")
      SetThemeHasHelpSystem(true);

    const HashMap< String, HashMap<String, ThemeData::ElementProperty> >& elementMap = ElementMap();

    // Process sub folder extra
    if (strcmp(node.name(), "extra") == 0 && !forcedExtra)
    {
      parseView(node, view, true);
      continue;
    }
    // Process normal item type
    auto elemTypeIt = elementMap.find(node.name());
    if(elemTypeIt == elementMap.end())
      { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "Unknown element of type \"" + String(node.name()) + "\"!"; }
    const auto & subElementMap = elemTypeIt->second;

    if (parseRegion(node))
    {
      String nameAttr = node.attribute("name").as_string();
      nameAttr.Trim();
      for(String& key : nameAttr.Split(','))
      {
        key.Trim();
        int* elementIndex = view.mElements.try_get(key);
        if (elementIndex == nullptr)
        {
          view.mElements[key] = (int) view.mElementArray.size();
          view.mElementArray.push_back(ThemeElement(key, node.name(), node.attribute("extra").as_bool(false) || forcedExtra));
          parseElement(node, subElementMap, view.mElementArray.back());
        }
        else parseElement(node, subElementMap, view.mElementArray[*elementIndex]);
      }
    }
  }
}

bool ThemeData::parseRegion(const pugi::xml_node& node)
{
  // No region?
  if (node.attribute("region") == nullptr) return true;

  for(String& region : String(node.attribute("region").as_string()).LowerCase().Split(','))
    if (region.Trim() == mRegion)
      return true;
  return false;
}

void ThemeData::parseElement(const pugi::xml_node& root, const HashMap<String, ElementProperty>& typeMap, ThemeElement& element)
{
  for (pugi::xml_node node = root.first_child(); node != nullptr; node = node.next_sibling())
  {
    auto typeIt = typeMap.find(node.name());
    if(typeIt == typeMap.end())
      { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "Unknown property type \"" + String(node.name()) + "\" (for element of type " + root.name() + ")."; }

    String str = resolveSystemVariable(mSystemThemeFolder, node.text().as_string(), mRandomPath).Trim();

    switch(typeIt->second)
    {
      case ElementProperty::NormalizedPair:
      {
        float x = 0;
        float y = 0;
        if (str.TryAsFloat(0, ' ', x))
          if (int pos = str.Find(' '); pos >= 0)
            if (str.TryAsFloat((int) pos + 1, 0, y))
            {
              element.AddVectorProperty(node.name(), x, y);
              break;
            }
        { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "invalid normalized pair (property \"" + String(node.name()) + "\", value \"" + str + "\")"; }
        break;
      }
      case ElementProperty::String:
      {
        element.AddStringProperty(node.name(), str);
        break;
      }
      case ElementProperty::Path:
      {
        Path path = Path(str).ToAbsolute(mPaths.back().Directory());
        String variable = node.text().get();
        if(!ResourceManager::fileExists(path))
        {
          //too many warnings with region and system variable surcharge in themes
          if (!root.attribute("region") && !variable.Contains("$system"))
          {
            String ss = "  Warning " + AddFiles(mPaths); // "from theme yadda yadda, included file yadda yadda
            ss += String("could not find file \"") + node.text().get() + "\" ";
            if(path.ToString() != node.text().get())
              ss += "(which resolved to \"" + path.ToString() + "\") ";
            { LOG(LogTrace) << "[ThemeData] " << ss; }
          }
          break;
        }
        element.AddStringProperty(node.name(), path.ToString());
        break;
      }
      case ElementProperty::Color:
      {
        element.AddIntProperty(node.name(), (int) getHexColor(str.c_str()));
        break;
      }
      case ElementProperty::Float:
      {
        float floatVal = 0;
        if (!str.TryAsFloat(floatVal))
        { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "invalid float value (property \"" + String(node.name()) + "\", value \"" + str + "\")"; }
        element.AddFloatProperty(node.name(), floatVal);
        break;
      }
      case ElementProperty::Boolean:
      {
        // only look at first char
        char first = str[0];
        element.AddBoolProperty(node.name(), (first == '1' || first == 't' || first == 'T' || first == 'y' || first == 'Y'));
        break;
      }
      default:
        { LOG(LogError) << "[Themes] " << AddFiles(mPaths) << "Unknown ElementPropertyType for \"" << root.attribute("name").as_string() << "\", property " << node.name(); }
    }
  }
}


const ThemeElement* ThemeData::getElement(const String& viewName, const String& elementName, const String& expectedType) const
{
  ThemeView* view = mViews.try_get(viewName);
  if (view == nullptr) return nullptr; // not found
  int* elementIndex = view->mElements.try_get(elementName);
  if (elementIndex == nullptr) return nullptr;
  ThemeElement* element = &(view->mElementArray[*elementIndex]);

  if(element->Type() != expectedType && !expectedType.empty())
  {
    { LOG(LogWarning) << "[ThemeData] Requested mismatched theme type for [" << viewName << "." << elementName << "] - expected \"" << expectedType << "\", got \"" << element->Type() << "\""; }
    return nullptr;
  }

  return element;
}

void ThemeData::LoadMain(const Path& root)
{
  { LOG(LogInfo) << "[ThemeManager] Loading main theme from: " << root; }
  loadFile(String::Empty, root);
}

void ThemeData::LoadSystem(const String& systemFolder, const Path& root)
{
  { LOG(LogInfo) << "[ThemeManager] Loading system theme from: " << root; }
  loadFile(systemFolder, root);
}

String ThemeData::getGameClipView() const
{
    return mGameClipView;
}

std::vector<Component*> ThemeData::makeExtras(const ThemeData& theme, const String& view, WindowManager& window)
{
  std::vector<Component*> comps;

  ThemeView* viewTheme = theme.mViews.try_get(view);
  if (viewTheme == nullptr) return comps;

  for(const ThemeElement& elem : viewTheme->mElementArray)
  {
    if(elem.Extra())
    {
      Component* comp = nullptr;
      const String& t = elem.Type();
      if      (t == "image")      comp = new ImageComponent(window);
      else if (t == "video")      comp = new VideoComponent(window);
      else if (t == "text")       comp = new TextComponent(window);
      else if (t == "textscroll") comp = new TextScrollComponent(window);
      else if (t == "box")        comp = new BoxComponent(window);

      if (comp != nullptr)
      {
        comp->setDefaultZIndex(10);
        comp->applyTheme(theme, view, elem.Name(), ThemeProperties::All);
        comps.push_back(comp);
      }
      else { LOG(LogWarning) << "[ThemeData] Extra type unknown: " << elem.Type(); }
    }
  }

  return comps;
}

HashMap<String, String> ThemeData::getThemeSubSets(const String& theme)
{
  HashMap<String, String> sets;
  std::deque<Path> dequepath;

  static const size_t pathCount = 3;
  Path paths[pathCount] =
  {
    RootFolders::TemplateRootFolder / "/system/.emulationstation/themes/" / theme,
    RootFolders::DataRootFolder     / "/themes/" / theme,
    RootFolders::DataRootFolder     / "/system/.emulationstation/themes/" / theme
  };

  for (const auto& path : paths)
  {
    Path::PathList list = path.GetDirectoryContent();
    for(auto& setPath : list)
    {
      if (setPath.IsDirectory())
      {
        Path themePath = setPath / "theme.xml";
        dequepath.push_back(themePath);
        pugi::xml_document doc;
        doc.load_file(themePath.ToChars());
        pugi::xml_node root = doc.child("theme");
        crawlIncludes(root, sets, dequepath);
        dequepath.pop_back();
      }
    }
    Path master = path / "theme.xml";
    if (master.Exists())
    {
      dequepath.push_back(master);
      pugi::xml_document doc;
      doc.load_file(master.ToChars());
      pugi::xml_node root = doc.child("theme");
      crawlIncludes(root, sets, dequepath);
      findRegion(doc, sets);
      dequepath.pop_back();
    }
  }

  return sets;
}

void ThemeData::crawlIncludes(const pugi::xml_node& root, HashMap<String, String>& sets, std::deque<Path>& dequepath)
{
  for (pugi::xml_node node = root.child("include"); node != nullptr; node = node.next_sibling("include"))
  {
    sets[node.attribute("name").as_string()] = node.attribute("subset").as_string();

    Path relPath(node.text().get());
    Path path = relPath.ToAbsolute(dequepath.back().Directory());
    dequepath.push_back(path);
    pugi::xml_document includeDoc;
    /*pugi::xml_parse_result result =*/ includeDoc.load_file(path.ToChars());
    pugi::xml_node newRoot = includeDoc.child("theme");
    crawlIncludes(newRoot, sets, dequepath);
    findRegion(includeDoc, sets);
    dequepath.pop_back();
  }
}

void ThemeData::findRegion(const pugi::xml_document& doc, HashMap<String, String>& sets)
{
  pugi::xpath_node_set regionattr = doc.select_nodes("//@region");
  for (auto xpath_node : regionattr)
  {
    if (xpath_node.attribute() != nullptr)
      sets[xpath_node.attribute().value()] = "region";
  }
}

// as the getThemeSubSets process is heavy, doing it 1 time for all subsets then sorting on demand
String::List ThemeData::sortThemeSubSets(const HashMap<String, String>& subsetmap, const String& subset)
{
  String::List sortedsets;

  for (const auto& it : subsetmap)
    if (it.second == subset)
      sortedsets.push_back(it.first);

  if(subset == "gameclipview" && !sortedsets.empty() )
      sortedsets.push_back(getNoTheme());

  std::sort(sortedsets.begin(), sortedsets.end());

  return sortedsets;
}

String ThemeData::getTransition() const
{
  String result;
  const auto* elem = getElement("system", "systemcarousel", "carousel");
  if (elem != nullptr) {
    if (elem->HasProperty("defaultTransition")) {
      if (elem->AsString("defaultTransition") == "instant") {
        result = "instant";
        return result;
      }
      if (elem->AsString("defaultTransition") == "fade") {
        result = "fade";
        return result;
      }
      if (elem->AsString("defaultTransition") == "slide") {
        result = "slide";
        return result;
      }
    }
  }
  return result;
}

bool ThemeData::isFolderHandled() const
{
  const auto* elem = getElement("detailed", "md_folder_name", "text");
  return elem != nullptr && elem->HasProperty("pos");
}