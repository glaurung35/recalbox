#include "ThemeData.h"
#include "resources/TextureResource.h"
#include "pugixml/pugixml.hpp"
#include <components/VideoComponent.h>
#include <MainRunner.h>
#include "RootFolders.h"
#include "MenuThemeData.h"
#include "components/TextScrollComponent.h"
#include "components/BoxComponent.h"

HashSet<String> ThemeData::mNoProcessAttributes;

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
    { LOG(LogError) << "[Themes] " << FileList() << "Invalid color (bad length, \"" + String(str) + "\" - must be 6 or 8)"; }

  if (string.Count() == 7) val = (val << 8) | 0xFF;
  return (unsigned int)val;
}

ThemeData::ThemeData(ThemeFileCache& cache, const SystemData* system, IExternalVariableResolver& globalResolver)
  : mCache(cache)
  , mSystem(system)
  , mVersion(0)
  , mLangageCode("en")
  , mRegionCode("us")
  , mLangageCodeInteger((int)'e' | ((int)'n' >> 8))
  , mLanguageRegionCodeInteger((int)'e' | ((int)'n' >> 8) | ((int)'U' >> 16) | ((int)'S' >> 24))
  , mGlobalResolver(globalResolver)
  , mGameResolver(nullptr)
{
  mSystemThemeFolder.clear();
  mRandomPath.clear();

  String lccc = RecalboxConf::Instance().GetSystemLanguage().LowerCase();
  if (lccc.size() >= 5)
  {
    int pos = lccc.Find('_');
    if (pos >= 2 && pos < (int) lccc.size() - 1)
    {
      mLangageCode = lccc.SubString(0, pos);
      mRegionCode = lccc.SubString(pos + 1);
      mLangageCodeInteger = (int)mLangageCode[0] | ((int)mLangageCode[1] >> 8);
      mLanguageRegionCodeInteger = mLangageCodeInteger | ((int)mRegionCode[0] >> 16) | ((int)mRegionCode[1] >> 24);
    }
  }

  if (mNoProcessAttributes.empty())
  {
    mNoProcessAttributes.insert("name");
    mNoProcessAttributes.insert("value");
    mNoProcessAttributes.insert("region");
    mNoProcessAttributes.insert("extra");
  }
}

bool ThemeData::CheckThemeOption(String& selected, const String& subset)
{
  if (mSubSets.empty()) return false;

  // Empty subset?
  String::List list = GetSubSetValues(subset);
  if (list.empty()) return false;

  // Try to fix the value if not found
  for(const String& s : list)
    if (s == selected)
      return false;

  // Take first value
  selected = list.front();
  return true;
}

void ThemeData::loadFile(const String& systemThemeFolder, const Path& path)
{
  mIncludePathStack.push_back(path);

  if(!path.Exists())
  {
    { LOG(LogError) << "[Themes] " << FileList() << "File does not exist!"; }
    return;
  }

  mVersion = 0;
  mViews.clear();

  mSystemThemeFolder = systemThemeFolder;

  String themeName = path.IsDirectory() ? path.Filename() : path.Directory().Filename();

  mColorset = RecalboxConf::Instance().GetThemeColorSet(themeName);
  mIconset = RecalboxConf::Instance().GetThemeIconSet(themeName);
  mMenu = RecalboxConf::Instance().GetThemeMenuSet(themeName);
  mSystemview = RecalboxConf::Instance().GetThemeSystemView(themeName);
  mGamelistview = RecalboxConf::Instance().GetThemeGamelistView(themeName);
  mGameClipView = RecalboxConf::Instance().GetThemeGameClipView(themeName);
  mRegion = RecalboxConf::Instance().GetThemeRegion(themeName);
  // Main theme ?
  if (mSystem == nullptr)
  {
    bool needSave = false;
    CrawlThemeSubSets(path);
    //for(auto& kv : subSets) { LOG(LogError) << "[DEBUG]" << kv.first << '-' << kv.second; }
    if (CheckThemeOption(mColorset, "colorset")) { RecalboxConf::Instance().SetThemeColorSet(themeName, mColorset); needSave = true; }
    if (CheckThemeOption(mIconset, "iconset")) { RecalboxConf::Instance().SetThemeIconSet(themeName, mIconset); needSave = true; }
    if (CheckThemeOption(mMenu, "menu")) { RecalboxConf::Instance().SetThemeMenuSet(themeName, mMenu); needSave = true; }
    if (CheckThemeOption(mSystemview, "systemview")) { RecalboxConf::Instance().SetThemeSystemView(themeName, mSystemview); needSave = true; }
    if (CheckThemeOption(mGamelistview, "gamelistview")) { RecalboxConf::Instance().SetThemeGamelistView(themeName, mGamelistview); needSave = true; }
    if (CheckThemeOption(mGameClipView, "gameclipview")) { RecalboxConf::Instance().SetThemeGameClipView(themeName, mGameClipView); needSave = true; }
    if (mRegion != "us" && mRegion != "eu" && mRegion != "jp") { mRegion="us"; RecalboxConf::Instance().SetThemeRegion(themeName, mRegion); needSave = true; }
    if (needSave) RecalboxConf::Instance().Save();
  }

  pugi::xml_document doc;
  pugi::xml_parse_result res = doc.load_string(mCache.File(path).data());
  if(!res) { LOG(LogError) << "[Themes] " << FileList() << "XML parsing error: \n    " + String(res.description()); }

  pugi::xml_node root = doc.child("theme");
  if(!root) { LOG(LogError) << "[Themes] " << FileList() << "Missing <theme> tag!"; }

  // parse version
  mVersion = root.child("formatVersion").text().as_float(-404);
  if(mVersion == -404)
    { LOG(LogError) << "[Themes] " << FileList() << "<formatVersion> tag missing!\n   It's either out of date or you need to add <formatVersion>" + String(CURRENT_THEME_FORMAT_VERSION) + "</formatVersion> inside your <theme> tag."; }

  if(mVersion < MINIMUM_THEME_FORMAT_VERSION)
    { LOG(LogError) << "[Themes] " << FileList() << "Theme uses format version " + String(mVersion, 2) + ". Minimum supported version is " + String(MINIMUM_THEME_FORMAT_VERSION) + '.'; }

  parseIncludes(root);
  parseViews(root);
  parseFeatures(root);
  mIncludePathStack.pop_back();
}

void ThemeData::parseIncludes(const pugi::xml_node& root)
{
  for (pugi::xml_node node = root.child("include"); node != nullptr; node = node.next_sibling("include"))
    if (parseSubset(node))
    {
      String str = node.text().get();
      resolveSystemVariable(mSystemThemeFolder, str, mRandomPath);
      str.Trim();
      //workaround for an issue in parseincludes introduced by variable implementation
      if (!str.Contains("//"))
      {
        Path path = Path(str).ToAbsolute(mIncludePathStack.back().Directory());
        if(!ResourceManager::fileExists(path))
        {
          { LOG(LogWarning) << "[ThemeData] Included file \"" << str << "\" not found! (resolved to \"" << path.ToString() << "\")"; }
          continue;
        }

        String errorString; errorString += "    from included file \"" + str + "\":\n    ";

        mIncludePathStack.push_back(path);

        pugi::xml_document includeDoc;
        pugi::xml_parse_result result = includeDoc.load_string(mCache.File(path).data());
        if(!result) { LOG(LogError) << "[Themes] " << FileList() << errorString + "Error parsing file: \n    " + result.description(); }

        pugi::xml_node newRoot = includeDoc.child("theme");
        if(!newRoot) { LOG(LogError) << "[Themes] " << FileList() << errorString + "Missing <theme> tag!"; }
        parseIncludes(newRoot);
        parseViews(newRoot);
        parseFeatures(newRoot);

        mIncludePathStack.pop_back();
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
    { LOG(LogError) << "[Themes] " << FileList() << "Feature missing \"supported\" attribute!"; continue; }

    const String supportedAttr = node.attribute("supported").as_string();
    if (ThemeSupport::SupportedFeatures().contains(supportedAttr))
      parseViews(node);
  }
}

void ThemeData::parseViews(const pugi::xml_node& root)
{
  // parse views
  for (pugi::xml_node node = root.child("view"); node != nullptr; node = node.next_sibling("view"))
  {
    if(!node.attribute("name")) { LOG(LogError) << "[Themes] " << FileList() << "View missing \"name\" attribute!"; continue; }
    for(String& viewName : String(node.attribute("name").as_string()).LowerCase().Split(','))
    {
      viewName.Trim();
      if (ThemeSupport::SupportedViews().contains(viewName))
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
    String typeString(node.name());
    // Process sub folder extra
    if (typeString == "extras" && !forcedExtra)
    {
      parseView(node, view, true);
      continue;
    }

    if (!node.attribute("name")) { LOG(LogError) << "[Themes] " << FileList() << "Element of type \"" << typeString << R"(" missing "name" attribute!)"; continue; }

    // Process normal item type
    ThemeElementType* type = ThemeSupport::ElementType().try_get(typeString);
    if (type == nullptr) { LOG(LogError) << "[Themes] " << FileList() << "Unknown element of type \"" << typeString << "\"!"; continue; }
    const ThemePropertyNameBits* properties = ThemeSupport::ElementMap().try_get(*type);
    if (properties == nullptr) { LOG(LogError) << "[Themes] " << FileList() << " Cannot get properties of element of type \"" << typeString << "\"!"; continue; }

    if (parseRegion(node))
    {
      String names = node.attribute("name").as_string();
      String name;
      for(bool again = true; again;)
      {
        if (again = names.Extract(',', name, names, true); !again) name = names.Trim();
        int* elementIndex = view.mElements.try_get(name);
        if (elementIndex == nullptr)
        {
          view.mElements[name] = (int) view.mElementArray.size();
          view.mElementArray.push_back(ThemeElement(name, *type, node.attribute("extra").as_bool(false) || forcedExtra));
          parseElement(node, *properties, view.mElementArray.back());
        }
        else parseElement(node, *properties, view.mElementArray[*elementIndex].MergeExtra(node.attribute("extra").as_bool(false)));
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

void ThemeData::parseProperty(const String& elementName, ThemePropertyName propertyName, String& value, ThemeElement& element)
{
  switch(ThemeSupport::PropertyTypeFromName(propertyName))
  {
    case ThemePropertyType::NormalizedPair:
    {
      float x = 0;
      float y = 0;
      if (value.TryAsFloat(0, ' ', x))
        if (int pos = value.Find(' '); pos >= 0)
          if (value.TryAsFloat((int) pos + 1, 0, y))
          {
            element.AddVectorProperty(propertyName, x, y);
            break;
          }
      { LOG(LogError) << "[Themes] " << FileList() << "invalid normalized pair (property \"" << ThemeSupport::ReversePropertyName(propertyName) << "\", value \"" << value << "\")"; }
      break;
    }
    case ThemePropertyType::String:
    {
      resolveSystemVariable(mSystemThemeFolder, value, mRandomPath);
      element.AddStringProperty(propertyName, value);
      break;
    }
    case ThemePropertyType::Path:
    {
      resolveSystemVariable(mSystemThemeFolder, value, mRandomPath);
      Path path = Path(value).ToAbsolute(mIncludePathStack.back().Directory());
      if (ResourceManager::fileExists(path))
        element.AddStringProperty(propertyName, path.ToString());
      break;
    }
    case ThemePropertyType::Color:
    {
      element.AddIntProperty(propertyName, (int) getHexColor(value.c_str()));
      break;
    }
    case ThemePropertyType::Float:
    {
      float floatVal = 0;
      if (!value.TryAsFloat(floatVal))
      { LOG(LogError) << "[Themes] " << FileList() << "invalid float value (property \"" << ThemeSupport::ReversePropertyName(propertyName) << "\", value \"" + value + "\")"; }
      element.AddFloatProperty(propertyName, floatVal);
      break;
    }
    case ThemePropertyType::Boolean:
    {
      // only look at first char
      char first = value.empty() ? '\0' : value[0];
      element.AddBoolProperty(propertyName, (first == '1' || first == 't' || first == 'T' || first == 'y' || first == 'Y'));
      break;
    }
    default:
    { LOG(LogError) << "[Themes] " << FileList() << "Unknown ThemeSupport::ElementPropertyType for \"" << elementName << "\", property " << ThemeSupport::ReversePropertyName(propertyName); }
  }
}

void ThemeData::parseElement(const pugi::xml_node& root, const ThemePropertyNameBits& typeList, ThemeElement& element)
{
  String elementNode = root.attribute("name").as_string();
  // process node attributes
  for (pugi::xml_attribute& attribute : root.attributes())
  {
    String name = attribute.name();
    if (mNoProcessAttributes.contains(name)) continue;
    // Check object property
    ThemePropertyName* property = ThemeSupport::PropertyName().try_get(name);
    if (property == nullptr || !typeList.IsSet(*property))
    {
      { LOG(LogError) << "[Themes] " << FileList() << "Unknown property type \"" + name + "\" (for element " << elementNode << " of type " << root.name() << ")."; }
      continue;
    }
    // Localized?
    if (int locale = ExtractLocalizedCode(name); locale != 0)
    {
      // No matching? continue
      if (locale != mLangageCodeInteger && locale != mLanguageRegionCodeInteger) continue;
      element.SetLocalized(*property); // Match!
    }
    else if (element.IsAlreadyLocalized(*property)) continue; // If already localized, ignore non-localized text
    // Get value
    String value = attribute.as_string();
    // Process
    parseProperty(elementNode, *property, value.Trim(), element);
  }
  // Process sub nodes
  for (pugi::xml_node node = root.first_child(); node != nullptr; node = node.next_sibling())
  {
    String name = node.name();
    // Check object property
    ThemePropertyName* property = ThemeSupport::PropertyName().try_get(name);
    if (property == nullptr || !typeList.IsSet(*property))
    {
      { LOG(LogError) << "[Themes] " << FileList() << "Unknown property type \"" + name + "\" (for element " << elementNode << " of type " << root.name() << ")."; }
      continue;
    }
    // Localized?
    if (int locale = ExtractLocalizedCode(name); locale != 0)
    {
      // No matching? continue
      if (locale != mLangageCodeInteger && locale != mLanguageRegionCodeInteger) continue;
      element.SetLocalized(*property); // Match!
    }
    else if (element.IsAlreadyLocalized(*property)) continue; // If already localized, ignore non-localized text
    // Get value from attribute or text
    String value = node.attribute("value") != nullptr ? String(node.attribute("value").as_string()) : node.text().as_string();
    // Process
    parseProperty(elementNode, *property, value.Trim(), element);
  }
}

const ThemeElement* ThemeData::Element(const String& viewName, const String& elementName, ThemeElementType expectedType) const
{
  ThemeView* view = mViews.try_get(viewName);
  if (view == nullptr) return nullptr; // not found
  int* elementIndex = view->mElements.try_get(elementName);
  if (elementIndex == nullptr) return nullptr;
  ThemeElement* element = &(view->mElementArray[*elementIndex]);

  if(element->Type() != expectedType && expectedType != ThemeElementType::None)
  {
    { LOG(LogWarning) << "[ThemeData] Requested mismatched theme type for [" << viewName << "." << elementName << "] - expected \"" << (int)expectedType << "\", got \"" << (int)element->Type() << "\""; }
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
  { LOG(LogInfo) << "[ThemeManager] Loading " << mSystem->FullName() <<"' system theme from: " << root; }
  loadFile(systemFolder, root);
}

String ThemeData::getGameClipView() const
{
    return mGameClipView;
}

ThemeExtras::List ThemeData::GetExtras(const String& view, WindowManager& window) const
{
  ThemeExtras::List comps;

  ThemeView* viewTheme = mViews.try_get(view);
  if (viewTheme == nullptr) return comps;

  bool uniqueVideo = false;
  for(const ThemeElement& elem : viewTheme->mElementArray)
  {
    if(elem.Extra())
    {
      ThemableComponent* comp = nullptr;
      switch(elem.Type())
      {
        case ThemeElementType::Image: comp = new ImageComponent(window); break;
        case ThemeElementType::Box: comp = new BoxComponent(window); break;
        case ThemeElementType::Video: if (!uniqueVideo) { comp = new VideoComponent(window); uniqueVideo = true; } break;
        case ThemeElementType::Text: comp = new TextComponent(window); break;
        case ThemeElementType::ScrollText: comp = new TextScrollComponent(window); break;
        case ThemeElementType::TextList:
        case ThemeElementType::Container:
        case ThemeElementType::NinePatch:
        case ThemeElementType::DateTime:
        case ThemeElementType::Rating:
        case ThemeElementType::Sound:
        case ThemeElementType::HelpSystem:
        case ThemeElementType::Carousel:
        case ThemeElementType::MenuBackground:
        case ThemeElementType::MenuIcons:
        case ThemeElementType::MenuSwitch:
        case ThemeElementType::MenuSlider:
        case ThemeElementType::MenuButton:
        case ThemeElementType::MenuText:
        case ThemeElementType::MenuTextSmall:
        case ThemeElementType::MenuSize:
        case ThemeElementType::None:
        default: break;
      }

      if (comp != nullptr)
      {
        comp->setDefaultZIndex(10);
        //comp->DoApplyThemeElement(*this, view, elem.Name(), ThemePropertyCategory::All);
        comps.push_back({ elem.Name(), elem.Type(), comp });
      }
      else { LOG(LogWarning) << "[ThemeData] Extra type unknown: " << (int)elem.Type(); }
    }
  }

  return comps;
}

void ThemeData::RefreshExtraProperties(ThemeExtras::List& extras, const String& view) const
{
  ThemeView* viewTheme = mViews.try_get(view);
  if (viewTheme == nullptr) return;

  for(ThemeExtras::Extra& extra : extras)
    extra.Component().DoApplyThemeElement(*this, view, extra.Name(), ThemePropertyCategory::All);
}

void ThemeData::CrawlThemeSubSets(const Path& themeRootPath)
{
  if (themeRootPath.Exists())
  {
    // Crawl
    mIncludePathStack.push_back(themeRootPath);
    pugi::xml_document doc;
    doc.load_string(mCache.File(themeRootPath).data());
    pugi::xml_node root = doc.child("theme");
    CrawlIncludes(root);
    CrawlRegions(doc);
    mIncludePathStack.pop_back();
  }
}

void ThemeData::CrawlIncludes(const pugi::xml_node& root)
{
  for (pugi::xml_node node = root.child("include"); node != nullptr; node = node.next_sibling("include"))
  {
    mSubSets[node.attribute("subset").as_string()].insert(node.attribute("name").as_string());

    Path relPath(node.text().get());
    Path path = relPath.ToAbsolute(mIncludePathStack.back().Directory());
    mIncludePathStack.push_back(path);
    pugi::xml_document includeDoc;
    /*pugi::xml_parse_result result =*/ includeDoc.load_string(mCache.File(path).data());
    pugi::xml_node newRoot = includeDoc.child("theme");
    CrawlIncludes(newRoot);
    CrawlRegions(includeDoc);
    mIncludePathStack.pop_back();
  }
}

void ThemeData::CrawlRegions(const pugi::xml_document& doc)
{
  pugi::xpath_node_set regionattr = doc.select_nodes("//@region");
  for (auto xpath_node : regionattr)
    if (xpath_node.attribute() != nullptr)
      mSubSets["region"].insert(xpath_node.attribute().value());
}

String::List ThemeData::GetSubSetValues(const String& subset) const
{
  HashSet<String>* list = mSubSets.try_get(subset);
  // No subset at all ?
  if (list == nullptr)
  {
    static String::List sEmptySubset;
    return sEmptySubset;
  }

  // Empty subset? special case for gameclipview
  if (subset == "gameclipview" && !list->empty())
  {
    static String::List sEmptySubset { getNoTheme() };
    return sEmptySubset;
  }

  // Sort
  String::List result;
  for(const String& string : *list)
    result.push_back(string);
  std::sort(result.begin(), result.end());

  return result;
}

String ThemeData::getTransition() const
{
  const auto* elem = Element("system", "systemcarousel", ThemeElementType::Carousel);
  if (elem != nullptr)
    if (elem->HasProperty(ThemePropertyName::DefaultTransition))
      return elem->AsString(ThemePropertyName::DefaultTransition);
  return "instant";
}

bool ThemeData::isFolderHandled() const
{
  const auto* elem = Element("detailed", "md_folder_name", ThemeElementType::Text);
  return elem != nullptr && elem->HasProperty(ThemePropertyName::Pos);
}

String ThemeData::FileList()
{
  String result;
  result = "from theme \"" + mIncludePathStack.front().ToString() + "\"\n";
  for (auto it = mIncludePathStack.begin() + 1; it != mIncludePathStack.end(); it++)
    result += "  (from included file \"" + (*it).ToString() + "\")\n";
  result += "    ";
  return result;
}

void ThemeData::Reset()
{
  mViews.clear();
  mSubSets.clear();
  mIncludePathStack.clear();
  mVersion = 0.f;
  mColorset.clear();
  mIconset.clear();
  mMenu.clear();
  mSystemview.clear();
  mGamelistview.clear();
  mRegion.clear();
  mGameClipView.clear();
  mSystemThemeFolder.clear();
  mRandomPath.clear();
}

void ThemeData::resolveSystemVariable(const String& systemThemeFolder, [[in,out]] String& path, String& randomPath)
{
  if (path.Contains('$'))
  {
    path.Replace("$system", systemThemeFolder)
        .Replace("$language", mLangageCode)
        .Replace("$country", mRegionCode);
    mGlobalResolver.ResolveVariableIn(path);
    if (mSystem != nullptr)
    {
      path.Replace("$fullname", mSystem->Descriptor().FullName())
          .Replace("$type", SystemDescriptor::ConvertSystemTypeToString(mSystem->Descriptor().Type()))
          .Replace("$pad", SystemDescriptor::ConvertDeviceRequirementToString(mSystem->Descriptor().PadRequirement()))
          .Replace("$keyboard", SystemDescriptor::ConvertDeviceRequirementToString(mSystem->Descriptor().KeyboardRequirement()))
          .Replace("$mouse", SystemDescriptor::ConvertDeviceRequirementToString(mSystem->Descriptor().MouseRequirement()))
          .Replace("$releaseyear", DateTime((long long int)mSystem->Descriptor().ReleaseDate()).ToStringFormat("%YYYY"))
          .Replace("$netplay", mSystem->Descriptor().HasNetPlayCores() ? "yes" : "no")
          .Replace("$lightgun", mSystem->Descriptor().LightGun() ? "yes" : "no");
      if (mGameResolver != nullptr) mGameResolver->ResolveVariableIn(path);
    }
    PickRandomPath(path, randomPath);
  }
}

void ThemeData::PickRandomPath(String& value, String& randomPath)
{
  if (!value.Contains(sRandomMethod)) return;

  String args;
  if (value.Extract(sRandomMethod, ")", args, true))
    if (randomPath.empty())
    {
      String::List paths = args.Split(',');
      std::random_device rd;
      std::default_random_engine engine(rd());
      const int max = (int) paths.size();
      std::uniform_int_distribution<int> distrib { 0, max - 1 };
      randomPath = paths[distrib(engine)];
    }

  value.Replace(sRandomMethod + args + ')', randomPath);
}

int ThemeData::ExtractLocalizedCode(String& name)
{
  if (int pos = name.Find('.'); pos > 0 && pos < (int)name.size() - 2)
  {
    int result = (int)name[pos + 1] | ((int)name[pos + 2] >> 8); // language code
    if (pos < (int) name.size() - 5)
      result |= ((int)name[pos + 1] >> 16) | ((int)name[pos + 2] >> 24);
    name.Delete(pos);
    return result;
  }
  return 0;
}
