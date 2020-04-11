//
// Created by thierry.imbert on 18/02/2020.
//
#pragma once

#include <utils/os/fs/Path.h>
#include <utils/storage/HashMap.h>

class IniFile
{
  public:
    /*!
     * @brief Constructor
     * @param initialConfigOnly true if only the original file has to be loaded
     */
    explicit IniFile(const Path& confpath);

    /*!
     * @brief Constructor
     * @param confpath File to load
     * @param fallbackpath File to load if confpath has not been loaded
     */
    explicit IniFile(const Path& confpath, const Path& fallbackpath);

    /*!
     * @brief Save the configuration file and backup the current one
     * @return True if the operation is successful
     */
    bool Save();

    /*!
     * @brief Get string value from the given key
     * @param name Key
     * @return Value or empty string if the key does not exist
     */
    std::string AsString(const std::string &name) const;

    /*!
     * @brief Get string value from the given key or return the default value
     * @param name Key
     * @param defaultValue Default value
     * @return Value or default value if the key does not exist
     */
    std::string AsString(const std::string &name, const std::string &defaultValue) const;

    /*!
     * @brief Get boolean value from the given key or return the default value
     * @param name Key
     * @param defaultValue Default value (optional, false by default)
     * @return Value or default value if the key does not exist
     */
    bool AsBool(const std::string& name, bool defaultValue = false) const;

    /*!
     * @brief Get value as unsigned int from the given key or return the default value
     * @param name Key
     * @param defaultValue Default value (optional, 0 by default)
     * @return Value or default value if the key does not exist
     */
    unsigned int AsUInt(const std::string& name, unsigned int defaultValue = 0) const;

    /*!
     * @brief Get value as signed int from the given key or return the default value
     * @param name Key
     * @param defaultValue Default value (optional, 0 by default)
     * @return Value or default value if the key does not exist
     */
    int AsInt(const std::string& name, int defaultValue = 0) const;

    /*!
     * @brief Set the value as string of the given key
     * @param name Key
     * @param value Value to set
     */
    void SetString(const std::string &name, const std::string &value);

    /*!
     * @brief Set the value as boolean of the given key
     * @param name Key
     * @param value Value to set
     */
    void SetBool(const std::string &name, bool value);

    /*!
     * @brief Set the value as an unsigned int of the given key
     * @param name Key
     * @param value Value to set
     */
    void SetUInt(const std::string &name, unsigned int value);

    /*!
     * @brief Set the value as a signed int of the given key
     * @param name Key
     * @param value Value to set
     */
    void SetInt(const std::string &name, unsigned int value);

    /*!
     * @brief Set the value as a string list, comma separated, of the given key
     * @param name Key
     * @param values string list
     */
    void SetList(const std::string &name, const std::vector<std::string> &values);

    /*!
     * @brief Check if a value is in the given named list
     * @param name Key from which to obtain the list
     * @param value Value to seek for in the list
     * @return True if the list exists and the value is found. False otherwise
     */
    bool isInList(const std::string &name, const std::string &value) const;

    /*!
     * @brief Check if the given line is a valide 'key=value'
     * @param line Text line to seek for key/value
     * @param key Extracted key if found
     * @param value Extracted value if found
     * @return true if a valid key=value has been found
     */
    static bool IsValidKeyValue(const std::string& line, std::string& key, std::string& value);

    /*!
     * @brief Check if this instance has loaded a file and has keys and values
     * @return True if at least one key/value pair has been loaded
     */
    bool IsValid() const { return mValid; }

    /*!
     * @brief Called after loading the file
     */
    virtual void OnLoad() {}

    /*!
     * @brief Called after saving the file
     */
    virtual void OnSave() {}

  private:
    //! Configuration map: key, value
    HashMap<std::string, std::string> mConfiguration;
    //! File path
    Path mFilePath;
    //! Fallback File path
    Path mFallbackFilePath;
    //! This object is valid and has keys/values
    bool mValid;

    /*!
     * @brief Load configuration file
     * @return True if a configuration file has been loaded successfully
     */
    bool Load();
};

