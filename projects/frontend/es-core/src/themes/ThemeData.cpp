#include "ThemeData.h"
#include "resources/TextureResource.h"
#include "pugixml/pugixml.hpp"
#include <components/VideoComponent.h>
#include <MainRunner.h>
#include "components/TextScrollComponent.h"
#include "components/BoxComponent.h"
#include "SimpleTokenizer.h"
#include "utils/Files.h"

HashSet<String> ThemeData::mNoProcessAttributes;

// helper
unsigned int ThemeData::getHexColor(const char* str)
{
  if(str == nullptr)
  {
    { LOG(LogError) << "[Theme] Empty color"; }
    return 0xFFFFFFFF;
  }

  String string('$');
  string.Append(str);
  int val = 0;
  if ((string.Count() != 7 && string.Count() != 9) || !string.TryAsInt(val))
    { LOG(LogError) << "[Theme] " << FileList() << "Invalid color (bad length, \"" + String(str) + "\" - must be 6 or 8)"; }

  if (string.Count() == 7) val = (val << 8) | 0xFF;
  return (unsigned int)val;
}

ThemeData::ThemeData(ThemeFileCache& cache, const SystemData* system, IGlobalVariableResolver& globalResolver)
  : mCache(cache)
  , mSystem(system)
  , mVersion(0)
  , mLangageCode("en")
  , mCountryCode("us")
  , mLangageCodeInteger((int)'e' | ((int)'n' >> 8))
  , mLanguageCountryCodeInteger((int)'e' | ((int)'n' >> 8) | ((int)'U' >> 16) | ((int)'S' >> 24))
  , mRegionCodeInteger(0)
  , mGlobalResolver(globalResolver)
  , mGameResolver(nullptr)
  , mCompatiblity(Compatibility::None)
  , mValid(false)
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
      mCountryCode = lccc.SubString(pos + 1).UpperCase();
      mLangageCodeInteger = (int)mLangageCode[0] | ((int)mLangageCode[1] << 8);
      mLanguageCountryCodeInteger = mLangageCodeInteger | ((int)mCountryCode[0] << 16) | ((int)mCountryCode[1] << 24);
    }
  }

  if (mNoProcessAttributes.empty())
  {
    mNoProcessAttributes.insert("if");
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
    { LOG(LogError) << "[Theme] " << FileList() << "File does not exist!"; }
    return;
  }

  mVersion = 0;
  mViews.clear();

  mSystemThemeFolder = systemThemeFolder;
  mThemeName = path.IsDirectory() ? path.Filename() : path.Directory().Filename();

  mColorset = RecalboxConf::Instance().GetThemeColorSet(mThemeName);
  mIconset = RecalboxConf::Instance().GetThemeIconSet(mThemeName);
  mMenu = RecalboxConf::Instance().GetThemeMenuSet(mThemeName);
  mSystemview = RecalboxConf::Instance().GetThemeSystemView(mThemeName);
  mGamelistview = RecalboxConf::Instance().GetThemeGamelistView(mThemeName);
  mGameClipView = RecalboxConf::Instance().GetThemeGameClipView(mThemeName);
  // Main theme ?
  if (mSystem == nullptr)
  {
    bool needSave = false;
    CrawlThemeSubSets(path);
    //for(auto& kv : subSets) { LOG(LogError) << "[DEBUG]" << kv.first << '-' << kv.second; }
    if (CheckThemeOption(mColorset, "colorset")) { RecalboxConf::Instance().SetThemeColorSet(mThemeName, mColorset); needSave = true; }
    if (CheckThemeOption(mIconset, "iconset")) { RecalboxConf::Instance().SetThemeIconSet(mThemeName, mIconset); needSave = true; }
    if (CheckThemeOption(mMenu, "menu")) { RecalboxConf::Instance().SetThemeMenuSet(mThemeName, mMenu); needSave = true; }
    if (CheckThemeOption(mSystemview, "systemview")) { RecalboxConf::Instance().SetThemeSystemView(mThemeName, mSystemview); needSave = true; }
    if (CheckThemeOption(mGamelistview, "gamelistview")) { RecalboxConf::Instance().SetThemeGamelistView(mThemeName, mGamelistview); needSave = true; }
    if (CheckThemeOption(mGameClipView, "gameclipview")) { RecalboxConf::Instance().SetThemeGameClipView(mThemeName, mGameClipView); needSave = true; }
    if (needSave) RecalboxConf::Instance().Save();
  }

  // Region is set at the begining of the main theme loading
  String region = RecalboxConf::Instance().GetThemeRegion();
  if      (region == "jp") mRegionCodeInteger = 'J' | ((int)'P' << 8);
  else if (region == "eu") mRegionCodeInteger = 'E' | ((int)'U' << 8);
  else mRegionCodeInteger = 'U' | ((int)'S' << 8);

  pugi::xml_document doc;
  pugi::xml_parse_result res = doc.load_string(mCache.File(path).data());
  if(!res) { LOG(LogError) << "[Theme] " << FileList() << "XML parsing error: \n    " + String(res.description()); }

  pugi::xml_node root = doc.child("theme");
  if(!root) { LOG(LogError) << "[Theme] " << FileList() << "Missing <theme> tag!"; }
  mCompatiblity = ExtractCompatibility(root);

  // parse version
  mVersion = root.child("formatVersion").text().as_float(-404);
  if(mVersion == -404)
    { LOG(LogError) << "[Theme] " << FileList() << "<formatVersion> tag missing!\n   It's either out of date or you need to add <formatVersion>" + String(CURRENT_THEME_FORMAT_VERSION) + "</formatVersion> inside your <theme> tag."; }

  if(mVersion < MINIMUM_THEME_FORMAT_VERSION)
    { LOG(LogError) << "[Theme] " << FileList() << "Theme uses format version " + String(mVersion, 2) + ". Minimum supported version is " + String(MINIMUM_THEME_FORMAT_VERSION) + '.'; }

  parseIncludes(root);
  parseViews(root);
  parseFeatures(root);
  mIncludePathStack.pop_back();

  // Validate the theme
  mValid = true;
}

void ThemeData::parseIncludes(const pugi::xml_node& root)
{
  for (pugi::xml_node node = root.child("include"); node != nullptr; node = node.next_sibling("include"))
    if (Condition(node))
      if (parseSubset(node))
      {
        pugi::xml_attribute pathAttribute = node.attribute("path");
        String str = (bool)pathAttribute ? pathAttribute.as_string() : node.text().get();
        resolveSystemVariable(mSystemThemeFolder, str, mRandomPath);
        str.Trim();
        //workaround for an issue in parseincludes introduced by variable implementation
        if (!str.Contains("//"))
        {
          Path path = Path(str).ToAbsolute(mIncludePathStack.back().Directory());
          if(!ResourceManager::fileExists(path))
          {
            { LOG(LogWarning) << "[Theme] Included file \"" << str << "\" not found! (resolved to \"" << path.ToString() << "\")"; }
            continue;
          }

          String errorString; errorString += "    from included file \"" + str + "\":\n    ";

          mIncludePathStack.push_back(path);

          pugi::xml_document includeDoc;
          pugi::xml_parse_result result = includeDoc.load_string(mCache.File(path).data());
          if(!result) { LOG(LogError) << "[Theme] " << FileList() << errorString + "Error parsing file: \n    " + result.description(); }

          pugi::xml_node newRoot = includeDoc.child("theme");
          if(!newRoot) { LOG(LogError) << "[Theme] " << FileList() << errorString + "Missing <theme> tag!"; }
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
    { LOG(LogError) << "[Theme] " << FileList() << "Feature missing \"supported\" attribute!"; continue; }

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
    if(!node.attribute("name")) { LOG(LogError) << "[Theme] " << FileList() << "View missing \"name\" attribute!"; continue; }
    if (!Condition(node)) continue;
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

    if (!node.attribute("name")) { LOG(LogError) << "[Theme] " << FileList() << "Element of type \"" << typeString << R"(" missing "name" attribute!)"; continue; }
    if (!IsMatchingLocaleOrRegionOrNeutral(typeString)) continue;

    // Process normal item type
    ThemeElementType* type = ThemeSupport::ElementType().try_get(typeString);
    if (type == nullptr) { LOG(LogError) << "[Theme] " << FileList() << "Unknown element of type \"" << typeString << "\"!"; continue; }
    const ThemePropertyNameBits* properties = ThemeSupport::ElementMap().try_get(*type);
    if (properties == nullptr) { LOG(LogError) << "[Theme] " << FileList() << " Cannot get properties of element of type \"" << typeString << "\"!"; continue; }

    if (IsMatchingRegionOldTag(node))
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

bool ThemeData::IsMatchingRegionOldTag(const pugi::xml_node& node) const
{
  // No region?
  if (node.attribute("region") == nullptr) return true;

  for(String& region : String(node.attribute("region").as_string()).UpperCase().Split(','))
    if (region.Trim(); region.size() == 2)
      if (((((int)region[1] << 8) | region[0]) & 0xDFDF) == mRegionCodeInteger)
        return true;
  return false;
}

void ThemeData::parseElement(const pugi::xml_node& root, const ThemePropertyNameBits& typeList, ThemeElement& element)
{
  String elementNode = root.attribute("name").as_string();
  if (!Condition(root)) return;
  // process node attributes
  for (pugi::xml_attribute& attribute : root.attributes())
  {
    String name = attribute.name();
    if (mNoProcessAttributes.contains(name)) continue;
    // Localized?
    ThemePropertyName* property = nullptr;
    if (!IsPropertyMatchingLocaleOrRegionOrNeutral(element, name, property)) continue;
    // Check object property
    if (property == nullptr || !typeList.IsSet(*property))
    {
      { LOG(LogError) << "[Theme] " << FileList() << "Unknown property type \"" + name + "\" (for element " << elementNode << " of type " << root.name() << ")."; }
      continue;
    }
    // Process
    parseProperty(elementNode, *property, String(attribute.as_string()).Trim(), element);
  }
  // Process sub nodes
  for (pugi::xml_node node = root.first_child(); node != nullptr; node = node.next_sibling())
  {
    String name = node.name();
    // Localized?
    ThemePropertyName* property = nullptr;
    if (!IsPropertyMatchingLocaleOrRegionOrNeutral(element, name, property)) continue;
    // Check object property
    if (property == nullptr || !typeList.IsSet(*property))
    {
      { LOG(LogError) << "[Theme] " << FileList() << "Unknown property type \"" + name + "\" (for element " << elementNode << " of type " << root.name() << ")."; }
      continue;
    }
    // Get value from attribute or text
    String value = node.attribute("value") != nullptr ? String(node.attribute("value").as_string()) : node.text().as_string();
    // Process
    if (Condition(node))
      parseProperty(elementNode, *property, value.Trim(), element);
  }
}

bool ThemeData::IsMatchingLocaleOrRegionOrNeutral(String& name) const
{
  int locale = ExtractLocalizedCode(name);
  // No locale/region
  if (locale == 0) return true;
  // Match?
  return locale == mLangageCodeInteger || locale == mLanguageCountryCodeInteger || locale == mRegionCodeInteger;
}

bool ThemeData::IsPropertyMatchingLocaleOrRegionOrNeutral(ThemeElement& element, String& name, ThemePropertyName*& property) const
{
  int locale = ExtractLocalizedCode(name);
  if (locale == 0) // No locale/region
  {
    property = ThemeSupport::PropertyName().try_get(name);
    return property == nullptr || !element.IsAlreadyLocalized(*property); // If already localized, ignore non-localized text
  }
  // Property is localized/regionalized
  if (locale != mLangageCodeInteger && locale != mLanguageCountryCodeInteger && locale != mRegionCodeInteger) return false;
  property = ThemeSupport::PropertyName().try_get(name);
  if (property != nullptr) element.SetLocalized(*property); // Match!
  return true;
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
      { LOG(LogError) << "[Theme] " << FileList() << "invalid normalized pair (property \"" << ThemeSupport::ReversePropertyName(propertyName) << "\", value \"" << value << "\")"; }
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
      { LOG(LogError) << "[Theme] " << FileList() << "invalid float value (property \"" << ThemeSupport::ReversePropertyName(propertyName) << "\", value \"" + value + "\")"; }
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
    { LOG(LogError) << "[Theme] " << FileList() << "Unknown ThemeSupport::ElementPropertyType for \"" << elementName << "\", property " << ThemeSupport::ReversePropertyName(propertyName); }
  }
}

const ThemeElement* ThemeData::Element(const String& viewName, const String& elementName, ThemeElementType expectedType) const
{
  ThemeView* view = mViews.try_get(viewName);
  if (view == nullptr) return nullptr; // not found
  int* elementIndex = view->mElements.try_get(elementName);
  if (elementIndex == nullptr) return nullptr;
  ThemeElement* element = &(view->mElementArray[*elementIndex]);

  if(element->Type() == expectedType || expectedType == ThemeElementType::Polymorphic) return element;

  { LOG(LogWarning) << "[Theme] Requested mismatched theme type for [" << viewName << "." << elementName << "] - expected \"" << (int)expectedType << "\", got \"" << (int)element->Type() << "\""; }
  return nullptr;
}

void ThemeData::LoadMain(const Path& root)
{
  { LOG(LogInfo) << "[Theme] Loading main theme from: " << root; }
  loadFile(String::Empty, root);
}

void ThemeData::LoadSystem(const String& systemFolder, const Path& root)
{
  { LOG(LogInfo) << "[Theme] Loading " << mSystem->FullName() <<"' system theme from: " << root; }
  loadFile(systemFolder, root);
}

String ThemeData::getGameClipView() const
{
    return mGameClipView;
}

ThemeExtras::List ThemeData::GetExtras(const String& view, WindowManager& window, IVideoComponentAction* videoInterface) const
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
        case ThemeElementType::Video: if (!uniqueVideo) { comp = new VideoComponent(window, videoInterface); uniqueVideo = true; } break;
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
        case ThemeElementType::Polymorphic:
        default: break;
      }

      if (comp != nullptr)
      {
        comp->setDefaultZIndex(10);
        comp->DoApplyThemeElement(*this, view, elem.Name(), ThemePropertyCategory::All);
        comps.push_back({ elem.Name(), elem.Type(), comp });
      }
      else { LOG(LogWarning) << "[Theme] Extra type unknown: " << (int)elem.Type(); }
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
    if (Condition(node))
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
  HashSet<String>* plist = mSubSets.try_get(subset);
  // No subset at all ?
  if (plist == nullptr)
  {
    static String::List sEmptySubset;
    return sEmptySubset;
  }

  // Empty subset? special case for gameclipview
  HashSet<String>& list = *plist;
  if (subset == "gameclipview" && list.empty())
  {
    static String::List sEmptySubset { getNoTheme() };
    return sEmptySubset;
  }

  // Sort
  String::List result;
  for(const String& string : list)
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
  return String::Empty;
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
        .Replace("$country", mCountryCode);
    mGlobalResolver.ResolveVariableIn(path);
    if (mSystem != nullptr)
    {
      path.Replace("$fullname", mSystem->Descriptor().FullName())
          .Replace("$type", SystemDescriptor::ConvertSystemTypeToString(mSystem->Descriptor().Type()))
          .Replace("$pad", SystemDescriptor::ConvertDeviceRequirementToString(mSystem->Descriptor().PadRequirement()))
          .Replace("$keyboard", SystemDescriptor::ConvertDeviceRequirementToString(mSystem->Descriptor().KeyboardRequirement()))
          .Replace("$mouse", SystemDescriptor::ConvertDeviceRequirementToString(mSystem->Descriptor().MouseRequirement()))
          .Replace("$releaseyear", String(mSystem->Descriptor().ReleaseDate() / 100))
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
    int result = (int)name[pos + 1] | ((int)name[pos + 2] << 8); // language code
    if (pos < (int) name.size() - 5)
      result |= ((int)name[pos + 1] << 16) | ((int)name[pos + 2] << 24);
    name.Delete(pos, INT32_MAX);
    return result;
  }
  return 0;
}

bool ThemeData::Condition(const pugi::xml_node& node)
{
  pugi::xml_attribute conditionAttribute = node.attribute("if");
  if (!conditionAttribute) return true; // No condition, condition always true

  String condition = conditionAttribute.as_string();
  if (condition.Trim().empty())
  { LOG(LogError) << "[Theme] Empty condition! Assuming true."; return true; }

  SimpleTokenizer tokenizer(condition);
  return EvaluateExpression(tokenizer, 0);
}

bool ThemeData::EvaluateExpression(SimpleTokenizer& tokenizer, int deepLevel) const
{
  bool result = false;
  bool inverter = false;
  for(SimpleTokenizer::Type previousType = tokenizer.TokenType();
      previousType != SimpleTokenizer::Type::End;
      previousType = tokenizer.Unary() ? previousType : tokenizer.TokenType())
    switch(tokenizer.Next())
    {
      case SimpleTokenizer::Type::Close:
      {
        if (deepLevel == 0)
        { LOG(LogDebug) << "[Theme] Unexpected ) at index " << tokenizer.Index() << " in conditionnal string " << tokenizer.ParsedString(); }
        return result;
      }
      case SimpleTokenizer::Type::Open:
      case SimpleTokenizer::Type::Identifier:
      {
        // Evaluate token
        bool evaluated = tokenizer.TokenType() == SimpleTokenizer::Type::Identifier ?
                         EvaluateIdentifier(tokenizer) :
                         EvaluateExpression(tokenizer, deepLevel + 1);
        // Not?
        if (inverter) evaluated = !evaluated;
        // Apply operation
        if      (previousType == SimpleTokenizer::Type::And  ) result &= evaluated;
        else if (previousType == SimpleTokenizer::Type::Or   ) result |= evaluated;
        else if (previousType == SimpleTokenizer::Type::Start ||
                 previousType == SimpleTokenizer::Type::Open) result = evaluated;
        else { LOG(LogDebug) << "[Theme] Syntax error at index " << tokenizer.Index() << " in conditional string " << tokenizer.ParsedString(); return false; }
        break;
      }
      case SimpleTokenizer::Type::And:
      case SimpleTokenizer::Type::Or:
      {
        if (previousType != SimpleTokenizer::Type::Identifier &&
            previousType != SimpleTokenizer::Type::Close)
        { LOG(LogDebug) << "[Theme] Syntax error at index " << tokenizer.Index() << " in conditional string " << tokenizer.ParsedString(); return false; }
        inverter = false;
        break;
      }
      case SimpleTokenizer::Type::Not:
      {
        if (previousType != SimpleTokenizer::Type::And &&
            previousType != SimpleTokenizer::Type::Or &&
            previousType != SimpleTokenizer::Type::Start &&
            previousType != SimpleTokenizer::Type::Open)
        { LOG(LogDebug) << "[Theme] Syntax error at index " << tokenizer.Index() << " in conditional string " << tokenizer.ParsedString(); return false; }
        inverter = !inverter;
        break;
      }
      case SimpleTokenizer::Type::Error:
      {
        { LOG(LogDebug) << "[Theme] Syntax error at index " << tokenizer.Index() << " in conditional string " << tokenizer.ParsedString(); }
        return false;
      }
      case SimpleTokenizer::Type::Start:
      {
        { LOG(LogDebug) << "[Theme] Internal logic error at index " << tokenizer.Index() << " in conditional string " << tokenizer.ParsedString(); }
        return false;
      }
      case SimpleTokenizer::Type::End:
      {
        if (deepLevel != 0)
        { LOG(LogDebug) << "[Theme] Missing ) at index " << tokenizer.Index() << " in conditionnal string " << tokenizer.ParsedString(); }
        if (previousType != SimpleTokenizer::Type::Identifier &&
            previousType != SimpleTokenizer::Type::Close)
        { LOG(LogDebug) << "[Theme] Missing operand at index " << tokenizer.Index() << " in conditionnal string " << tokenizer.ParsedString(); }
        break;
      }
    }
  // Hu?
  return result;
}

bool ThemeData::EvaluateIdentifier(const SimpleTokenizer& tokenizer) const
{
  bool evaluated = false;
  if      (tokenizer.Token() == "crt"       ) evaluated = mGlobalResolver.HasCrt();
  else if (tokenizer.Token() == "jamma"     ) evaluated = mGlobalResolver.HasJamma();
  else if (tokenizer.Token() == "overscan"  ) evaluated = mGlobalResolver.HasCrt() && !mGlobalResolver.HasJamma();
  else if (tokenizer.Token() == "tate"      ) evaluated = mGlobalResolver.IsTate();
  else if (tokenizer.Token() == "qvga"      ) evaluated = mGlobalResolver.IsQVGA();
  else if (tokenizer.Token() == "vga"       ) evaluated = mGlobalResolver.IsVGA();
  else if (tokenizer.Token() == "hd"        ) evaluated = mGlobalResolver.IsHD();
  else if (tokenizer.Token() == "fhd"       ) evaluated = mGlobalResolver.IsFHD();
  else if (tokenizer.Token() == "virtual"   ) evaluated = mSystem != nullptr ? mSystem->IsVirtual() : false;
  else if (tokenizer.Token() == "arcade"    ) evaluated = mSystem != nullptr ? mSystem->IsArcade() : false;
  else if (tokenizer.Token() == "port"      ) evaluated = mSystem != nullptr ? mSystem->IsPorts() : false;
  else if (tokenizer.Token() == "console"   ) evaluated = mSystem != nullptr ? mSystem->Descriptor().Type() == SystemDescriptor::SystemType::Console : false;
  else if (tokenizer.Token() == "handheld"  ) evaluated = mSystem != nullptr ? mSystem->Descriptor().Type() == SystemDescriptor::SystemType::Handheld : false;
  else if (tokenizer.Token() == "computer"  ) evaluated = mSystem != nullptr ? mSystem->Descriptor().Type() == SystemDescriptor::SystemType::Computer : false;
  else if (tokenizer.Token() == "fantasy"   ) evaluated = mSystem != nullptr ? mSystem->Descriptor().Type() == SystemDescriptor::SystemType::Fantasy : false;
  else if (tokenizer.Token() == "engine"    ) evaluated = mSystem != nullptr ? mSystem->Descriptor().Type() == SystemDescriptor::SystemType::Engine : false;
  else if (tokenizer.Token() == "favorite"  ) evaluated = mSystem != nullptr ? mSystem->IsFavorite() : false;
  else if (tokenizer.Token() == "lastplayed") evaluated = mSystem != nullptr ? mSystem->IsLastPlayed() : false;
  else { LOG(LogDebug) << "[Theme] Unknown identifier " << tokenizer.Token() << " at index " << tokenizer.Index() << " in conditional string " << tokenizer.ParsedString() << ". False evaluation assumed."; }
  return evaluated;
}

ThemeData::Compatibility ThemeData::ExtractCompatibility(const pugi::xml_node& node)
{
  pugi::xml_attribute compatibilityNode = node.attribute("compatibility");
  if (!compatibilityNode) return Compatibility::Hdmi;

  String compatibility = compatibilityNode.as_string();

  Compatibility result = Compatibility::None;
  String item;
  for(;!compatibility.empty();)
  {
    if (!compatibility.Extract(',', item, compatibility, true))
    {
      item = compatibility.Trim();
      compatibility.clear();
    }
    if      (item == "hdmi" ) result |= Compatibility::Hdmi;
    else if (item == "jamma") result |= Compatibility::Jamma;
    else if (item == "crt"  ) result |= Compatibility::Crt;
    else if (item == "tate" ) result |= Compatibility::Tate;
  }
  if (result == Compatibility::None) result = Compatibility::Hdmi;
  return result;
}

ThemeData::Resolutions ThemeData::ExtractResolutions(const pugi::xml_node& node)
{
  pugi::xml_attribute resolutionNode = node.attribute("resolutions");
  if (!resolutionNode) return Resolutions::FHD | Resolutions::HD;

  String resolutions = resolutionNode.as_string();

  Resolutions result = Resolutions::None;
  String item;
  for(;!resolutions.empty();)
  {
    if (!resolutions.Extract(',', item, resolutions, true))
    {
      item = resolutions.Trim();
      resolutions.clear();
    }
    if      (item == "qvga") result |= Resolutions::QVGA;
    else if (item == "vga" ) result |= Resolutions::VGA;
    else if (item == "hd"  ) result |= Resolutions::HD;
    else if (item == "fhd" ) result |= Resolutions::FHD;
  }
  if (result == Resolutions::None) result = Resolutions::FHD | Resolutions::HD;
  return result;
}

bool ThemeData::FetchCompatibility(const Path& path, [[out]] ThemeData::Compatibility& compatibility, [[out]] Resolutions& resolutions, String& name, int& version, int& recalboxVersion)
{
  if (!path.Exists()) { LOG(LogError) << "[Theme] " << path << " does not exist!"; return false; }

  pugi::xml_document doc;
  pugi::xml_parse_result res = doc.load_string(Files::LoadFile(path).data());
  if (!res) { LOG(LogError) << "[Theme] XML parsing error: \n    " + String(res.description()); return false; }

  pugi::xml_node root = doc.child("theme");
  if (!root) { LOG(LogError) << "[Theme] Missing <theme> tag!"; return false; }

  // Extract compatibility
  compatibility = ExtractCompatibility(root);

  // Extract resolutions
  resolutions = ExtractResolutions(root);

  // Extract name
  name = path.Directory().FilenameWithoutExtension();
  pugi::xml_attribute nameAttribute = root.attribute("name");
  if ((bool)nameAttribute)
    if (String newName = nameAttribute.as_string(); !newName.Trim().empty())
      name = newName;

  // Extract version
  version = 0;
  pugi::xml_attribute versionAttribute = root.attribute("version");
  if ((bool)versionAttribute)
    if (String newVersion = versionAttribute.as_string(); !newVersion.Trim().empty())
      if (String major, minor; newVersion.Extract('.', major, minor, true))
        version = (major.AsInt() << 8) + minor.AsInt();

  // Extract recalbox version
  recalboxVersion = 0;
  pugi::xml_attribute recalboxAttribute = root.attribute("recalbox");
  if ((bool)recalboxAttribute)
    if (String newRecalbox = recalboxAttribute.as_string(); !newRecalbox.Trim().empty())
      if (String major, minor; newRecalbox.Extract('.', major, minor, true))
        recalboxVersion = (major.AsInt() << 8) + minor.AsInt();

  return true;
}


