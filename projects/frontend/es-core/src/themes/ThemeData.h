#pragma once

#include <deque>
#include <random>
#include <utils/String.h>
#include <utils/os/fs/Path.h>
#include <RecalboxConf.h>
#include "pugixml/pugixml.hpp"
#include "ThemeElement.h"
#include <utils/storage/Set.h>

// Forward declarations
template<typename T> class TextListComponent;
class Component;
class Sound;
class ImageComponent;
class NinePatchComponent;
class TextComponent;
class WindowManager;

class ThemeData
{
  public:
    //! Constructor
    ThemeData();

    /*!
     * @brief Load main theme
     * @param root Root folder
     */
    void LoadMain(const Path& root);

    /*!
     * @brief Load system theme
     * @param systemFolder System ftheme folder
     * @param root System root path
     */
    void LoadSystem(const String& systemFolder, const Path& root);

    static bool ThemeHasMenuView()
    { return sThemeHasMenuView; }

    static bool ThemeHasHelpSystem()
    { return sThemeHasHelpSystem; }

    static void SetThemeHasMenuView(bool on)
    { sThemeHasMenuView = on; }

    static void SetThemeHasHelpSystem(bool on)
    { sThemeHasHelpSystem = on; }

    void loadFile(const String& systemThemeFolder, const Path& path);

    enum class ElementProperty
    {
      NormalizedPair,
      Path,
      String,
      Color,
      Float,
      Boolean
    };

    // If expectedType is an empty string, will do no type checking.
    [[nodiscard]] const ThemeElement*
    getElement(const String& view, const String& element, const String& expectedType) const;

    static std::vector<Component*> makeExtras(const ThemeData& theme, const String& view, WindowManager& window);

    [[nodiscard]] String getGameClipView() const;

    static const char* getNoTheme() { return "0 - DEFAULT"; }

    static HashMap<String, String> getThemeSubSets(const String& theme);

    static String::List sortThemeSubSets(const HashMap<String, String>& subsetmap, const String& subset);

    [[nodiscard]] String getTransition() const;

    [[nodiscard]] bool getHasFavoritesInTheme() const { return (mVersion >= CURRENT_THEME_FORMAT_VERSION); } // #TODO: delete

    [[nodiscard]] bool isFolderHandled() const;

    static constexpr int MINIMUM_THEME_FORMAT_VERSION = 3;
    static constexpr int CURRENT_THEME_FORMAT_VERSION = 4;

  private:
    //! View content
    class ThemeView
    {
      public:
        //! Raw element array
        std::vector<ThemeElement> mElementArray;
        //! Element name to element index
        HashMap<String, int> mElements;
    };

    static bool sThemeHasMenuView;
    static bool sThemeHasHelpSystem;

    unsigned int getHexColor(const char* str);

    //! Authorized elements/sub-elements in the xml
    static HashMap<String, HashMap<String, ElementProperty>>& ElementMap();
    //! Supported features set
    static HashSet<String>& SupportedFeatures();
    //! Supported view set
    static HashSet<String>& SupportedViews();

    std::deque<Path> mPaths;
    float mVersion;
    String mColorset;
    String mIconset;
    String mMenu;
    String mSystemview;
    String mGamelistview;
    String mRegion;
    String mGameClipView;
    String mSystemThemeFolder;
    String mRandomPath;
    static constexpr const char* sRandomMethod = "$random(";

    void parseFeatures(const pugi::xml_node& themeRoot);

    void parseIncludes(const pugi::xml_node& themeRoot);

    void parseViews(const pugi::xml_node& themeRoot);

    void parseView(const pugi::xml_node& viewNode, ThemeView& view, bool forcedExtra);

    void parseElement(const pugi::xml_node& elementNode, const HashMap<String, ElementProperty>& typeMap,
                      ThemeElement& element);

    bool parseRegion(const pugi::xml_node& root);

    bool parseSubset(const pugi::xml_node& node);

    static void crawlIncludes(const pugi::xml_node& root, HashMap<String, String>& sets, std::deque<Path>& dequepath);

    static void findRegion(const pugi::xml_document& doc, HashMap<String, String>& sets);

    static bool CheckThemeOption(String& selected, const HashMap<String, String>& subsets, const String& subset);

    static String resolveSystemVariable(const String& systemThemeFolder, const String& path, String& randomPath)
    {
      String lccc = RecalboxConf::Instance().GetSystemLanguage().LowerCase();
      String lc = "en";
      String cc = "us";
      if (lccc.size() >= 5)
      {
        int pos = lccc.Find('_');
        if (pos >= 2 && pos < (int) lccc.size() - 1)
        {
          lc = lccc.SubString(0, pos);
          cc = lccc.SubString(pos + 1);
        }
      }

      String result = path;
      result.Replace("$system", systemThemeFolder).Replace("$language", lc).Replace("$country", cc);

      PickRandomPath(result, randomPath);
      return result;
    }

    static void PickRandomPath(String& value, String& randomPath)
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
          std::uniform_int_distribution<int> distrib {
            0,
            max - 1
          };
          randomPath = paths[distrib(engine)];
        }

      value.Replace(sRandomMethod + args + ')', randomPath);
    }

    HashMap<String, ThemeView> mViews;

    static String AddFiles(const std::deque<Path>& deque)
    {
      String result;
      result = "from theme \"" + deque.front().ToString() + "\"\n";
      for (auto it = deque.begin() + 1; it != deque.end(); it++)
        result += "  (from included file \"" + (*it).ToString() + "\")\n";
      result += "    ";
      return result;
    }
};
