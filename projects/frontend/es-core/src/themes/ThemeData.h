#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <deque>
#include <string>
#include <boost/variant.hpp>
#include <utils/os/fs/Path.h>
#include "pugixml/pugixml.hpp"
#include "utils/math/Vectors.h"

template<typename T> class TextListComponent;

class GuiComponent;
class Sound;
class ImageComponent;
class NinePatchComponent;
class TextComponent;
class Window;

class ThemeException : public std::exception
{
public:
	std::string msg;

	const char* what() const noexcept override { return msg.c_str(); }

	template<typename T>
	friend ThemeException& operator<<(ThemeException& e, T msg);
	
	inline void setFiles(const std::deque<Path>& deque)
	{
		*this << "from theme \"" << deque.front().ToString() << "\"\n";
		for (auto it = deque.begin() + 1; it != deque.end(); it++)
			*this << "  (from included file \"" << (*it).ToString() << "\")\n";
		*this << "    ";
	}
};

template<typename T>
ThemeException& operator<<(ThemeException& e, T appendMsg)
{
	std::stringstream ss;
	ss << e.msg << appendMsg;
	e.msg = ss.str();
	return e;
}

class ThemeSet
{
  private:
    Path mPath;

  public:
    ThemeSet() = default;
    explicit ThemeSet(const Path& path) : mPath(path) {}
    inline std::string getName() const { return mPath.FilenameWithoutExtension(); }
    inline Path getThemePath(const std::string& system) const { return mPath / system / "theme.xml"; }
};

class ThemeData
{
  public:

    class ThemeElement
    {
      public:
        bool extra;
        std::string type;

        std::map< std::string, boost::variant<Vector2f, std::string, unsigned int, float, bool> > properties;

        template<typename T>
        T get(const std::string& prop) const { return boost::get<T>(properties.at(prop)); }

        inline bool has(const std::string& prop) const { return (properties.find(prop) != properties.end()); }
    };

  private:
    class ThemeView
    {
      public:
        std::map<std::string, ThemeElement> elements;
        std::vector<std::string> orderedKeys;
    };

  public:

    ThemeData();

	// throws ThemeException
	void loadFile(const std::string& systemThemeFolder, const Path& path);

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
    const ThemeElement* getElement(const std::string& view, const std::string& element, const std::string& expectedType) const;

    static std::vector<GuiComponent*> makeExtras(const ThemeData& theme, const std::string& view, Window* window);

    static const ThemeData& getDefault();
    static const ThemeData& getCurrent();

	static std::map<std::string, ThemeSet> getThemeSets();
	static std::map<std::string, std::string> getThemeSubSets(const std::string& theme);
	static std::map<std::string, std::string> sortThemeSubSets(const std::map<std::string, std::string>& subsetmap, const std::string& subset);
	static Path getThemeFromCurrentSet(const std::string& system);
	std::string getTransition() const;

    bool getHasFavoritesInTheme() const
    { return (mVersion >= CURRENT_THEME_FORMAT_VERSION); }
    bool isFolderHandled() const;

    static constexpr int MINIMUM_THEME_FORMAT_VERSION = 3;
    static constexpr int CURRENT_THEME_FORMAT_VERSION = 4;

  private:
    static std::map<std::string, std::map<std::string, ElementProperty>>& ElementMap();
    static std::vector<std::string>& SupportedFeatures();
    static std::vector<std::string>& SupportedViews();

	std::deque<Path> mPaths;
	float mVersion;
	std::string mColorset;
	std::string mIconset;
	std::string mMenu;
	std::string mSystemview;
	std::string mGamelistview;
	std::string mSystemThemeFolder;

    void parseFeatures(const pugi::xml_node& themeRoot);
    void parseIncludes(const pugi::xml_node& themeRoot);
    void parseViews(const pugi::xml_node& themeRoot);
    void parseView(const pugi::xml_node& viewNode, ThemeView& view);
    void parseElement(const pugi::xml_node& elementNode, const std::map<std::string, ElementProperty>& typeMap, ThemeElement& element);
    bool parseRegion(const pugi::xml_node& root);
    bool parseSubset(const pugi::xml_node& node);
    static void crawlIncludes(const pugi::xml_node& root, std::map<std::string, std::string>& sets, std::deque<Path>& dequepath);
    static void findRegion(const pugi::xml_document& doc, std::map<std::string, std::string>& sets);

    std::string resolveSystemVariable(const std::string& systemThemeFolder, const std::string& path);

    std::map<std::string, ThemeView> mViews;
};
