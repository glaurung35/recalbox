#pragma once

#include <vector>
#include <string>

#define LEGACY_STRING(x) x, (int)sizeof(x)-1

/*!
 * Strings tool class
 * @note: Use empty multiline comment in front of method which have at least one direct equivalent in the String class
 */
class Strings
{
  private:
    //! Limit to 16bits unicode
    static constexpr int sUnicodeSize = 0x10000;

    static unsigned short sSmallToCapital[1 << (8 * sizeof(unsigned short))];
    static unsigned short sCapitalToSmall[1 << (8 * sizeof(unsigned short))];

    static bool sInitialized;

    static bool sInitialize();

  public:
    typedef std::vector<std::string> Vector;

    static const std::string Empty;

    /**/ static unsigned int UpperChar(const std::string& utf8string)
    {
      (void)sInitialized;
      int dummy = 0;
      unsigned int unicode = chars2Unicode(utf8string, dummy);
      if (unicode < sUnicodeSize) return sSmallToCapital[unicode] == 0 ? unicode : sSmallToCapital[unicode];
      return unicode;
    }

    static unsigned int UpperChar(unsigned int unicode)
    {
      if (unicode < sUnicodeSize) return sSmallToCapital[unicode] == 0 ? unicode : sSmallToCapital[unicode];
      return unicode;
    }

    static unsigned int chars2Unicode(const std::string& _string, int& _cursor);

    static std::string unicode2Chars(unsigned int _unicode);

    /**/ static std::string ToUpperUTF8(const std::string& _string);

    /**/ static std::string ToLowerASCII(const std::string& _string);

    /**/ static std::string ToUpperASCII(const std::string& _string);

    /**/ static std::string Trim(const std::string& _string);

    /**/ static std::string Trim(const std::string& _string, const char* _trimwhat);

    /**/ static std::string Replace(const std::string& _string, char _replace, char _with);

    /**/ static std::string Replace(const std::string& _string, const std::string& _replace, const std::string& _with);

    /**/ static std::string Replace(const std::string& _string, const std::string& _replace, const char* _with, int _withLength);

    /**/ static std::string& ReplaceAllIn(std::string& _string, char _replace, char _with);

    /**/ static std::string& ReplaceAllIn(std::string& _string, const std::string& _replace, const std::string& _with);

    /**/ static std::string& ReplaceAllIn(std::string& _string, char _replace, const char* _with, int _withlength);

    /**/ static std::string Extract(const std::string& source, const char* starttag, const char* endtag, int starttagl, int endtagl);

    /**/ static bool StartsWith(const std::string& _string, const std::string& _start);

    /**/ static bool StartsWith(const std::string& _string, const char* _start, int length);

    static std::string RemoveParenthesis(const std::string& _string);

    /**/static Vector Split(const std::string& _string, char splitter, bool multipleSplittersAsOne = false);

    /**/ static Vector Split(const std::string& _string, char splitter, int max, bool multipleSplittersAsOne = false);

    static Vector SplitQuoted(const std::string& _string, char splitter);

    static Vector SplitQuoted(const std::string& _string, char splitter, int max);

    /**/ static bool SplitAt(const std::string& _string, char splitter, std::string& left, std::string& right, bool trim);

    /**/ static std::string Join(const std::vector<std::string>& _string, const std::string& joiner);

    /**/ static std::string Join(const std::vector<const char*>& _string, const std::string& joiner);

    /**/ static std::string Join(const std::vector<std::string>& _string, char joiner);

    /**/ static std::string Join(const std::vector<const char*>& _string, char joiner);

    /**/ static std::string Format(const char* _string, ...);

    static std::string ScrambleSymetric2(const std::string& _input, const std::string& key);

    /**/ static bool ToInt(const std::string& source, int index, char stop, int& out);

    /**/ static bool ToInt(const std::string& source, int index, int& out) { return ToInt(source, index, 0, out); }

    /**/ static bool ToInt(const std::string& source, int& out) { return ToInt(source, 0, 0, out); }

    /**/ static bool ToLong(const std::string& source, int index, char stop, long long int& out);

    /**/ static bool ToLong(const std::string& source, long long int& out) { return ToLong(source, 0, 0, out); }

    /**/ static bool ToFloat(const std::string& source, int index, char stop, float& out);

    /**/ static bool ToFloat(const std::string& source, float& out) { return ToFloat(source, 0, 0, out); }

    /**/ static bool HexToInt(const std::string& from, int index, char stop, int& out);

    /**/ static bool HexToInt(const std::string& from, int index, int& out) { return HexToInt(from, index, 0, out); }

    /**/ static bool HexToInt(const std::string& from, int& out) { return HexToInt(from, 0, 0, out); }

    /**/ static std::string ToHexa(int hex, int length);

    /**/ static std::string UnicodeToUtf8(const std::vector<unsigned int>& unicodes);

    /**/ static std::vector<unsigned int> Utf8ToUnicode(const std::string& utf8);
    static std::string Decode64(const std::string& base64);
};

#pragma clang diagnostic pop