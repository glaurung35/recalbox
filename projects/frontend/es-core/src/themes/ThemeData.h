#pragma once

#include <deque>
#include <random>
#include <utils/String.h>
#include <utils/os/fs/Path.h>
#include <RecalboxConf.h>
#include "pugixml/pugixml.hpp"
#include "ThemeElement.h"
#include "ThemeFileCache.h"
#include "ThemeExtras.h"
#include "IGlobalVariableResolver.h"
#include "SimpleTokenizer.h"
#include <themes/ThemeSupport.h>
#include <themes/ThemeExtras.h>

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
    explicit ThemeData(ThemeFileCache& cache, const SystemData* system, IGlobalVariableResolver& globalResolver);

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

    /*!
     * @brief Check if the curren theme contains a menu view
     * @return True if the curren theme contains a menu view, false otherwise
     */
    [[nodiscard]] bool HasMenuView() const { return mViews.contains("menu"); }

    void loadFile(const String& systemThemeFolder, const Path& path);

    // If expectedType is an empty string, will do no type checking.
    [[nodiscard]] const ThemeElement* Element(const String& view, const String& element, ThemeElementType expectedType) const;

    /*!
     * @brief Build Extra component array from the curren theme
     * @param view View name
     * @param window Window manager (for components initialization)
     * @return Extra list
     */
    ThemeExtras::List GetExtras(const String& view, WindowManager& window) const;

    /*!
     * @brief Re-apply theme properties to the current extra list, allowing to refresh components and make them
     * doing a theme refresh or a theme switch
     * @param extras Extra list
     * @param view View from which to extract theme elements
     */
    void RefreshExtraProperties(ThemeExtras::List& extras, const String& view) const;

    [[nodiscard]] String getGameClipView() const;

    static const char* getNoTheme() { return "0 - DEFAULT"; }

    [[nodiscard]] String getTransition() const;

    [[nodiscard]] bool isFolderHandled() const;

    static constexpr int MINIMUM_THEME_FORMAT_VERSION = 4;
    static constexpr int CURRENT_THEME_FORMAT_VERSION = 5;

    /*!
     * @brief Cleanup current ThemeData
     */
    void Reset();

    /*!
     * @brief Get subset values for the given subset name
     * @param subset Subset name
     * @return subset values
     */
    [[nodiscard]] String::List GetSubSetValues(const String& subset) const;

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

    //! Cache reference
    ThemeFileCache& mCache;
    //! System reference
    const SystemData* mSystem;

    //! View's properties
    HashMap<String, ThemeView> mViews;
    //! Recorded subsets
    HashMap<String, HashSet<String>> mSubSets;

    //! Static list of element attributes not to process as element properties
    static HashSet<String> mNoProcessAttributes;

    std::deque<Path> mIncludePathStack;
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

    //! User langage
    String mLangageCode;
    //! User region
    String mRegionCode;
    //! User langage as integer
    int mLangageCodeInteger;
    //! User language/region as integer
    int mLanguageRegionCodeInteger;

    //! External global variable resolver
    IGlobalVariableResolver& mGlobalResolver;
    //! External game-related variable resolver
    IGlobalVariableResolver* mGameResolver;

    static constexpr const char* sRandomMethod = "$random(";

    unsigned int getHexColor(const char* str);

    void parseFeatures(const pugi::xml_node& themeRoot);

    void parseIncludes(const pugi::xml_node& themeRoot);

    void parseViews(const pugi::xml_node& themeRoot);

    void parseView(const pugi::xml_node& viewNode, ThemeView& view, bool forcedExtra);

    void parseElement(const pugi::xml_node& elementNode, const ThemePropertyNameBits& typeList, ThemeElement& element);

    /*!
     * @brief Parse an element's property
     * @param elementName Element name (parent node)
     * @param propertyName Property name (curren tnode)
     * @param value Property value
     * @param propertyType Expected property type
     * @param element Element in which to store property
     */
    void parseProperty(const String& elementName, ThemePropertyName propertyName, String& value, ThemeElement& element);

    bool parseRegion(const pugi::xml_node& root);

    bool parseSubset(const pugi::xml_node& node);

    void resolveSystemVariable(const String& systemThemeFolder, [[out]] String& path, String& randomPath);

    static void PickRandomPath(String& value, String& randomPath);

    bool Condition(const pugi::xml_node& node);

    [[nodiscard]] bool Evaluate(const SimpleTokenizer& tokenizer) const;

    /*!
     * @brief Build a file list from the current include stack for logging only
     * @return Log string
     */
    String FileList();

    /*!
     * @brief Browse the whole theme, looking for all subset & subset values
     * @param root Root theme file
     */
    void CrawlThemeSubSets(const Path& root);

    /*!
     * @brief Crawl includes, looking for subsets
     * @param root Root node
     */
    void CrawlIncludes(const pugi::xml_node& root);

    /*!
     * @brief Crawl includes, looking for region subsets
     * @param doc Document main node
     */
    void CrawlRegions(const pugi::xml_document& doc);

    /*!
     * @brief Check if the selected option exists in thje given sibset.
     * If not, it is adjusted to the first option (default)
     * @param selected Selected option
     * @param subset Subset name
     * @return True if the option selected option has been adjusted, false otherwise
     */
    bool CheckThemeOption(String& selected, const String& subset);

    /*!
     * @brief Try to find language and region code in the name, after a final '.' ir available
     * @param name Original name - May be trimmed from localized info when returning
     * @return Language code, language + region code, or 0 if no information is available
     */
    static int ExtractLocalizedCode(String& name);
};
