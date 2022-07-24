//
// Created by bkg2k on 18/07/22.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <utils/String.h>
#include <utils/os/fs/Path.h>
#include <utils/storage/HashMap.h>
#include <utils/storage/Set.h>
#include <utils/ISectionFileSpy.h>

/*!
 * Manage sectioned ini-files with sections in []
 *
 * Only deleted and modified keys are saved. All the test is left untouched, keeping comments and
 * all previously stores section & keys, even ill-formed ones
 *
 * Files are checked against too many ill-formed lines and control characters.
 * A backup files mey be specified, so that it is loaded if the initial file has too many errors
 */
class SectionFile
{
  public:
    /*!
     * @brief Default empty constructor
     */
    SectionFile()
      : SectionFile(Path::Empty, Path::Empty, false, nullptr)
    {
    }

    /*!
     * @brief Default text constructor
     * @param content Raw section text
     */
    explicit SectionFile(const String& content)
      : SectionFile(content, false, nullptr)
    {
    }

    /*!
     * @brief Complete text constructor
     * @param content Raw section text
     * @param spaceAroundSeparator True to save new or modified keys with spaces before and after the equal sign
     */
    SectionFile(const String& content, bool spaceAroundSeparator)
      : SectionFile(content, spaceAroundSeparator, nullptr)
    {
    }

    /*!
     * @brief Complete text constructor
     * @param content Raw section text
     * @param spy Spy interface
     */
    SectionFile(const String& content, ISectionFileSpy* spy)
      : SectionFile(content, false, spy)
    {
    }

    /*!
     * @brief Complete text constructor
     * @param content Raw section text
     * @param spaceAroundSeparator True to save new or modified keys with spaces before and after the equal sign
     * @param spy Spy interface
     */
    SectionFile(const String& content, bool spaceAroundSeparator, ISectionFileSpy* spy);

    /*!
     * @brief Default empty constructor
     * @param spy Spy interface
     */
    explicit SectionFile(ISectionFileSpy* spy)
      : SectionFile(Path::Empty, Path::Empty, false, spy)
    {
    }

    /*!
     * @brief Open and manage a section file
     * @param filePath Section file full path
     */
    explicit SectionFile(const Path& filePath)
      : SectionFile(filePath, Path::Empty, false, nullptr)
    {
    }

    /*!
     * @brief Open and manage a section file
     * @param filePath Section file full path
     * @param spy Spy interface
     */
    SectionFile(const Path& filePath, ISectionFileSpy* spy)
      : SectionFile(filePath, Path::Empty, false, spy)
    {
    }

    /*!
     * @brief Open and manage a section file
     * @param filePath Section file full path
     * @param spaceAroundSeparator True to save new or modified keys with spaces before and after the equal sign
     */
    SectionFile(const Path& filePath, bool spaceAroundSeparator)
      : SectionFile(filePath, Path::Empty, spaceAroundSeparator, nullptr)
    {
    }

    /*!
     * @brief Open and manage a section file
     * @param filePath Section file full path
     * @param spaceAroundSeparator True to save new or modified keys with spaces before and after the equal sign
     * @param spy Spy interface
     */
    SectionFile(const Path& filePath, bool spaceAroundSeparator, ISectionFileSpy* spy)
      : SectionFile(filePath, Path::Empty, spaceAroundSeparator, spy)
    {
    }

    /*!
     * @brief Open and manage a section file. If the primary file has too mush error,
     * then the fallback file is loaded. In such case, the fallback file is copied into the primary file
     * @param filePath Primary section file full path
     * @param fallbackFilePath Fallback full path
     */
    SectionFile(const Path& filePath, const Path& fallbackFilePath)
      : SectionFile(filePath, fallbackFilePath, false, nullptr)
    {
    }

    /*!
     * @brief Open and manage a section file. If the primary file has too mush error,
     * then the fallback file is loaded. In such case, the fallback file is copied into the primary file
     * @param filePath Primary section file full path
     * @param fallbackFilePath Fallback full path
     * @param spy Spy interface
     */
    SectionFile(const Path& filePath, const Path& fallbackFilePath, ISectionFileSpy* spy)
      : SectionFile(filePath, fallbackFilePath, false, spy)
    {
    }

    /*!
     * @brief Open and manage a section file. If the primary file has too mush error,
     * then the fallback file is loaded. In such case, the fallback file is copied into the primary file
     * @param filePath Primary section file full path
     * @param fallbackFilePath Fallback full path
     * @param spaceAroundSeparator True to save new or modified keys with spaces before and after the equal sign
     */
    SectionFile(const Path& filePath, const Path& fallbackFilePath, bool spaceAroundSeparator)
      : SectionFile(filePath, fallbackFilePath, spaceAroundSeparator, nullptr)
    {
    }

    /*!
     * @brief Open and manage a section file. If the primary file has too mush error,
     * then the fallback file is loaded. In such case, the fallback file is copied into the primary file
     * @param filePath Primary section file full path
     * @param fallbackFilePath Fallback full path
     * @param spaceAroundSeparator True to save new or modified keys with spaces before and after the equal sign
     * @param spy Spy interface
     */
    SectionFile(const Path& filePath, const Path& fallbackFilePath, bool spaceAroundSeparator, ISectionFileSpy* spy);

    /*
     * Setters
     */

    /*!
     * @brief Set extra space around the '=' separator on/off
     * @param extraSpace Extra space state
     */
    void SetExtraSpaceAroundSeparator(bool extraSpace) { mExtraSpace = extraSpace; }

    /*!
     * @brief Set spy interface
     * @param spy Spy interface
     */
    void SetSpyInterface(ISectionFileSpy* spy) { mSpy = spy; }

    /*!
     * @brief Change primary file. Further saving will save into this new files
     * @param newPath
     */
    void SetPrimaryFile(const Path& newPath) { mPrimary = newPath; }

    /*
     * File operations
     */

    /*!
     * @brief Export pending modifications & delete into the primary file
     * @return True if the operation is successful, false otherwise
     */
    bool Save() { return Save(mPrimary); }

    /*!
     * @brief Export the whole file content to a single string
     * instead of saving all in the primary file
     * Primary file is till loaded if available, before applying pending modifications
     * @return All content as a single string
     */
    String ExportToString()
    {
      String::List result;
      LoadLines(mPrimary, result);
      Export(result);
      return String::Join(result, String::CRLF);
    }

    /*
     * Section management
     */

    /*!
     * @brief section name list (default section not included)
     * @return
     */
    [[nodiscard]] String::List GetSectionNames() const
    {
      String::List result;
      for(const auto& kv : mSections)
        result.push_back(kv.first);
      return result;
    }

    /*
     * Test
     */

    [[nodiscard]] bool HasSection(const String& sectionName) const { return mSections.contains(sectionName); }

    [[nodiscard]] bool HasKey(const String& sectionName, const String& key) const
    {
      Section* section = mSections.try_get(sectionName);
      return section != nullptr ? section->Has(key) : false;
    }

    /*
     * Counts
     */

    [[nodiscard]] int SectionCount() const { return (int)mSections.size(); }

    [[nodiscard]] int Count(const String& sectionName) const
    {
      Section* section = mSections.try_get(sectionName);
      return section != nullptr ? section->Count() : 0;
    }

    /*
     * Delete operations
     */

    /*!
     * @brief Delete a whole section
     * @param section Section to delete
     * @return This
     */
    SectionFile& DeleteSection(const String& section) { mDeletedSections.insert(section); return *this; }

    SectionFile& Delete(const String& key) { mDefault.Delete(key); return *this; }

    SectionFile& Delete(const String& section, const String& key) { GetOrCreateSection(section).Delete(key); return *this; }

    /*
     * Set operations
     */

    SectionFile&  SetAsString(const String& key, const String& value) { SetItem(mDefault, key, value); return *this; }

    SectionFile&  SetAsString(const String& section, const String& key, const String& value) { SetItem(section, key, value); return *this; }

    SectionFile&  SetAsBool(const String& key, bool value) { SetItem(mDefault, key, String(value)); return *this; }

    SectionFile&  SetAsBool(const String& section, const String& key, bool value) { SetItem(section, key, String(value)); return *this; }

    SectionFile&  SetAsBoolText(const String& key, bool value) { SetItem(mDefault, key, String(value, true)); return *this; }

    SectionFile&  SetAsBoolText(const String& section, const String& key, bool value) { SetItem(section, key, String(value, true)); return *this; }

    SectionFile&  SetAsInt(const String& key, int value) { SetItem(mDefault, key, String(value)); return *this; }

    SectionFile&  SetAsInt(const String& section, const String& key, int value) { SetItem(section, key, String(value)); return *this; }

    SectionFile&  SetAsInt64(const String& key, long long value) { SetItem(mDefault, key, String(value)); return *this; }

    SectionFile&  SetAsInt64(const String& section, const String& key, long long value) { SetItem(section, key, String(value)); return *this; }

    SectionFile&  SetAsFloat(const String& key, float value) { SetItem(mDefault, key, String(value)); return *this; }

    SectionFile&  SetAsFloat(const String& section, const String& key, float value) { SetItem(section, key, String(value)); return *this; }

    SectionFile&  SetAsDouble(const String& key, double value) { SetItem(mDefault, key, String(value)); return *this; }

    SectionFile&  SetAsDouble(const String& section, const String& key, double value) { SetItem(section, key, String(value)); return *this; }

    SectionFile&  SetAsFloat(const String& key, float value, int precision) { SetItem(mDefault, key, String(value, precision)); return *this; }

    SectionFile&  SetAsFloat(const String& section, const String& key, float value, int precision) { SetItem(section, key, String(value, precision)); return *this; }

    SectionFile&  SetAsDouble(const String& key, double value, int precision) { SetItem(mDefault, key, String(value, precision)); return *this; }

    SectionFile&  SetAsDouble(const String& section, const String& key, double value, int precision) { SetItem(section, key, String(value, precision)); return *this; }

    /*
     * Get operations
     */

    [[nodiscard]] String AsString(const String& key) const { return GetItem(mDefault, key); }

    [[nodiscard]] String AsString(const String& section, const String& key) const { return GetItem(section, key); }

    [[nodiscard]] bool AsBool(const String& key) const { return GetItem(mDefault, key).AsBool(); }

    [[nodiscard]] bool AsBool(const String& section, const String& key) const { return GetItem(section, key).AsBool(); }

    [[nodiscard]] int AsInt(const String& key) const { return GetItem(mDefault, key).AsInt(); }

    [[nodiscard]] int AsInt(const String& section, const String& key) const { return GetItem(section, key).AsInt(); }

    [[nodiscard]] long long AsInt64(const String& key) const { return GetItem(mDefault, key).AsInt64(); }

    [[nodiscard]] long long AsInt64(const String& section, const String& key) const { return GetItem(section, key).AsInt64(); }

    [[nodiscard]] float AsFloat(const String& key) const { return GetItem(mDefault, key).AsFloat(); }

    [[nodiscard]] float AsFloat(const String& section, const String& key) const { return GetItem(section, key).AsFloat(); }

    [[nodiscard]] double AsDouble(const String& key) const { return GetItem(mDefault, key).AsDouble(); }

    [[nodiscard]] double AsDouble(const String& section, const String& key) const { return GetItem(section, key).AsDouble(); }

    [[nodiscard]] String AsStringDefault(const String& key, const String& defValue) const { return GetItem(mDefault, key, defValue); }

    [[nodiscard]] String AsStringDefault(const String& section, const String& key, const String& defValue) const { return GetItem(section, key, defValue); }

    [[nodiscard]] bool AsBoolDefault(const String& key, bool defValue) const { return GetItem(mDefault, key).AsBool(defValue); }

    [[nodiscard]] bool AsBoolDefault(const String& section, const String& key, bool defValue) const { return GetItem(section, key).AsBool(defValue); }

    [[nodiscard]] int AsIntDefault(const String& key, int defValue) const { return GetItem(mDefault, key).AsInt(0, defValue); }

    [[nodiscard]] int AsIntDefault(const String& section, const String& key, int defValue) const { return GetItem(section, key).AsInt(0, defValue); }

    [[nodiscard]] long long AsInt64Default(const String& key, long long defValue) const { return GetItem(mDefault, key).AsInt64(0, defValue); }

    [[nodiscard]] long long AsInt64Default(const String& section, const String& key, long long defValue) const { return GetItem(section, key).AsInt64(0, defValue); }

    [[nodiscard]] float AsFloatDefault(const String& key, float defValue) const { return GetItem(mDefault, key).AsFloat(0, defValue); }

    [[nodiscard]] float AsFloatDefault(const String& section, const String& key, float defValue) const { return GetItem(section, key).AsFloat(0, defValue); }

    [[nodiscard]] double AsDoubleDefault(const String& key, double defValue) const { return GetItem(mDefault, key).AsDouble(0, defValue); }

    [[nodiscard]] double AsDoubleDefault(const String& section, const String& key, double defValue) const { return GetItem(section, key).AsDouble(0, defValue); }

  #ifndef TEST_ACCESS
  private:
  #endif
    /*!
     * @brief Line type used by the internal parser to identify lines and errors
     */
    enum class LineType
    {
      // Regular lines
      Blank           , //!< Empty or blank line (contains only spaces/tabs)
      Comment         , //!< Line comment, starting with '#'
      ActiveKeyValue  , //!< Active Key = value line
      InactiveKeyValue, //!< Inactive Key = value line (starting with ';')
      SectionHeader   , //!< Section header, formatted as [section_name]
      // Error lines
      Unidentified    , //!< Valid but unidentified (free text?)
      Binary          , //!< Contains invalid control characters
      TooLong         , //!< Exceed 4096 characters
      InvalidKey      , //!< Key = value line with invalid characters in key
    };

    //! Section data holder
    class Section
    {
      public:
        //! Key count (not including pendings)
        [[nodiscard]] int Count() const { return (int)mKeyValues.size(); }

        /*!
         * @brief Check if the current section has pending writes or delete
         * @return True if there is at least one pending write opr delete
         */
        [[nodiscard]] bool HasPendings() const { return !mPendingWrites.empty() || !mPendingDelete.empty(); }

        /*!
         * @brief Check kif this section contains the given key
         * @param key Key to check
         * @return True if the key exists, false otherwise
         */
        [[nodiscard]] bool Has(const String& key) const { return mKeyValues.contains(key) || mPendingWrites.contains(key); }

        /*!
         * @brief Get the value from the given key.
         * If the key does not exists in this section, en empty value is returned
         * @param key Key to get value from
         * @return Value
         */
        [[nodiscard]] const String& Get(const String& key) const
        {
          String* value = mPendingWrites.try_get(key);
          if (value == nullptr) value = mKeyValues.try_get(key);
          if (value != nullptr) return *value;
          return String::Empty;
        }

        /*!
         * @brief Set a new value to the given key
         * @param key Key to assign the new value
         * @param value Value to assign to the key
         */
        void Set(const String& key, const String& value)
        {
          mPendingDelete.erase(key);
          String* current = mKeyValues.try_get(key);
          if (current != nullptr && *current == value)
            mPendingWrites.erase(key);
          else
            mPendingWrites[key] = value;
        }

        /*!
         * @brief Delete the given key from this section
         * @param key Key to delete
         */
        void Delete(const String& key)
        {
          mKeyValues.erase(key);
          mPendingWrites.erase(key);
          mPendingDelete.insert(key);
        }

        /*!
         * @brief Import section content from the given index until the enf of list
         * or the start of a new section
         * @param from List to load values from
         * @param index Start index
         * @param errorLevel Computed error level from 0.0 (no error) up to 1.0 (100% error)
         * @param keepExistingValues if a key already exist in the section, do not overwrite the value
         * @return Index of a new section or end of list
         */
        int Import(const String::List& from, int index, double& errorLevel, bool keepExistingValues);

        /*!
         * @brief Export current section
         * @param to Output list
         * @param index Start index in the list
         * @param extraSpace True to add a space between key & '=', and between '=' and value
         * @return Last index (new section or end if list)
         */
        int Export(String::List& to, int index, bool extraSpace);

        /*!
         * @brief Try to extract the section name from the line formatted as [section_name]
         * @param line line to check
         * @param to Output section name
         * @return True if the line is a section line and the name has been successfully extracted
         */
        static bool ExtractSection(const String& line, String& to)
        {
          size_t start = line.find_first_not_of(" \t");
          if (start ==  std::string::npos || line[start] != '[') return false;
          size_t stop = line.find_last_not_of(" \t");
          if (stop ==  std::string::npos || line[stop] != ']') return false;
          if (stop > start)
          {
            to = line.SubString((int)start + 1, (int)stop - ((int)start + 1)).Trim(' ');
            return IsValidIdentifier(to, 0, to.Count());
          }
          return false;
        }

        /*!
         * @brief Check if an identifier in the given line is valid (only valid characters)
         * Identifier is located between first & last char, both included
         * @param line Line in which the identifier is located
         * @param firstCharIndex First character index of the identifier
         * @param lastCharIndex Last character index of the identifier
         * @return True if the identifier contains only valide characters, false otherwise
         */
        static bool IsValidIdentifier(const String& line, int firstCharIndex, int lastCharIndex)
        {
          int excluded = (int)line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.-", firstCharIndex);
          return (excluded < firstCharIndex || excluded > lastCharIndex);
        }

      #ifndef TEST_ACCESS
      private:
      #endif
        //! Key/Values
        HashMap<String, String> mKeyValues;
        //! Pending write key/values
        HashMap<String, String> mPendingWrites;
        //! Pending deleted keys
        HashSet<String> mPendingDelete;

        /*!
         * @brief Flush pending delete & modifications of existing keys
         * @param to Lines to flush into
         * @param index Section first line
         * @param extraSpace True to add a space between key & '=', and between '=' and value
         * @return Last index processed: next section or enf of list
         */
        int FlushPending(String::List& to, int index, bool extraSpace);

        /*!
         * @brief Flush new key/values
         * @param to Lines to flush into
         * @param index Line index to add new values
         * @param extraSpace True to add a space between key & '=', and between '=' and value
         * @return Last index processed: next section or enf of list
         */
        int FlushNew(String::List& to, int index, bool extraSpace);

        /*!
         * @brief Remove things that have not been flushed
         * Deleted key that were just added for instance
         */
        void RemoveUnflushed();

        /*!
         * @brief Parse & identify the given line
         * If the line is a key/value, leftOutput and rightOutput are filled with key and value
         * Anything else does not fill output strings
         * @param line Line to identify
         * @param leftOutput Left output (contains key for key=value lines)
         * @param rightOutput Right output (contains value for key=value lines)
         * @return Line type
         */
        static LineType ParseLine(const String& line, String& leftOutput, String& rightOutput);
    };

    //! File path
    Path mPrimary;
    //! Fallback
    Path mFallback;

    //! Default unnamed section
    Section mDefault;
    //! Named sections
    HashMap<String, Section> mSections;
    //! Pending delete sections
    HashSet<String> mDeletedSections;

    //! Spy interface
    ISectionFileSpy* mSpy;

    //! Space around '=' when saving files
    bool mExtraSpace;
    //! Loaded from fallback
    bool mLoadedFromFallBack;

    /*
     * File operations
     */

    /*!
     * @brief Global loading process. Load either primary file or fallback file if available
     * and the primary file has too much error.
     * @return True if the operation is successful, false otherwise
     */
    void Load();

    /*!
     * @brief Load file content and import everything into internal structure
     * @param filePath File to import
     * @param keepExistingValues If true, do not overwrite existing keys in internal structures
     * @return True if imported lines are clean, false if there are too much errors
     */
    bool Load(const Path& filePath, bool keepExistingValues);

    /*!
     * @brief Load primary of fallback file, export modified/deleted/new keys, then save to file
     * @param filePath Target file
     * @return True if the operation is successful, false otherwise
     */
    bool Save(const Path& filePath);

    /*!
     * @brief Load file content and split lines
     * @param filePath File to load
     * @param to Line storage
     * @return True if the operation is successful, false otherwise
     */
    static bool LoadLines(const Path& filePath, String::List& to);

    /*!
     * @brief Join lines from the given list and save them all into the target file
     * @param filePath File to save lines to
     * @param from Source lines
     * @return True if the operation is successful, false otherwise
     */
    static bool SaveLines(const Path& filePath, String::List& from);

    /*
     * Import/Export
     */

    /*!
     * @brief Import key/Values from the given list into internal structure
     * @param from List to import
     * @param keepExistingValues If true, do not overwrite existing keys in internal structures
     * @return True if imported lines are clean, false if there are too much errors
     */
    bool Import(const String::List& from, bool keepExistingValues);

    /*!
     * @brief Export internal structure to the given line list,
     * updating & deleting required lines
     * @param to List to update
     */
    void Export(String::List& to);

    /*
     * Section/Key management
     */

    /*!
     * @brief Get the section object according to the given name
     * May create the section if it does not exist yet
     * @param sectionKey Section name
     * @return Section object
     */
    Section& GetOrCreateSection(const String& sectionKey);

    [[nodiscard]] const String& GetItem(const String& sectionName, const String& key) const
    {
      Section* section = mSections.try_get(sectionName);
      return section != nullptr ? GetItem(*section, key) : String::Empty;
    }

    [[nodiscard]] const String& GetItem(const String& sectionName, const String& key, const String& defValue) const
    {
      Section* section = mSections.try_get(sectionName);
      return section != nullptr ? GetItem(*section, key, defValue) : defValue;
    }

    static const String& GetItem(const Section& section, const String& key) { return section.Get(key); }

    static const String& GetItem(const Section& section, const String& key, const String& defValue) { return section.Has(key) ? section.Get(key) : defValue; }

    void SetItem(const String& section, const String& key, const String& value) { SetItem(GetOrCreateSection(section), key, value); }

    static void SetItem(Section& section, const String& key, const String& value) { section.Set(key, value); }
};
