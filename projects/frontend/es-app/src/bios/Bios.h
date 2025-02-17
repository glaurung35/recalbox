//
// Created by bkg2k on 28/12/2019.
//
#pragma once

#include <utils/os/fs/Path.h>
#include <utils/hash/Md5.h>
#include <utils/Xml.h>

/*!
 * @brief Single bios entry class
 * @note Accessing this class through multiple thread w/o mutexes is safe, as only cardinal values may change.
 * all sub-classes are set only once when loaded from file
 */
class Bios
{
  public:
    static constexpr int sMaxBiosPath = 2;

    enum class Status
    {
      Unknown,         //!< Not yet scanned
      FileNotFound,    //!< File does not exist
      HashNotMatching, //!< File exists but Hash does not match any possible hashes
      HashMatching,    //!< File exists and hash is also matching one of the possible hashes
    };

    enum class ReportStatus
    {
      Unknown, //!< Not yet scanned
      Green,   //!< Bios is Ok
      Yellow,  //!< Not mandatory missing or hash mismatch (if allowed)
      Red,     //!< File for found or hash mismatch (and not allowed)
    };

  private:
    /*!
     * @brief MD5 Binary Hash container
     */
    struct Md5Hash
    {
      private:
        //! Binary representation of a MD5 hash
        unsigned char mBytes[16];
        bool mValid;

      public:
        /*!
         * @brief Default Constructor
         */
        Md5Hash()
          : mBytes {},
            mValid(false)
        {
          memset(mBytes, 0, sizeof(mBytes));
        }

        /*!
         * @brief Copy constructor
         * @param source Source Hash
         */
        Md5Hash& operator = (const Md5Hash& source)
        {
          if (&source != this)
          {
            memcpy(mBytes, source.mBytes, sizeof(mBytes));
            mValid = source.mValid;
          }
          return *this;
        }

        /*!
         * @brief Copy constructor
         * @param source Source Hash
         */
        Md5Hash(const Md5Hash& source)
          : mBytes {},
            mValid(source.mValid)
        {
          memcpy(mBytes, source.mBytes, sizeof(mBytes));
        }

        /*!
         * @brief Construct from MD5 object
         * @param source MD5 object
         */
        explicit Md5Hash(const MD5& source)
          : mBytes {},
            mValid(true)
        {
          memcpy(mBytes, source.Output(), sizeof(mBytes));
        }

        /*!
         * @brief Deserialization constructor
         * @param source Source stringized hash
         */
        explicit Md5Hash(const String& source);

        /*!
         * @brief Compare to another MD5
         * @param other Md5 to compare to
         * @return True if both hashes match
         */
        bool IsMatching(const Md5Hash& other) const
        {
          return memcmp(mBytes, other.mBytes, sizeof(mBytes)) == 0;
        }

        /*!
         * @brief Check if the MD5 is valid: either copied from a valid one or computed from MD5 class or deserialized
         * @return True if the current MD5 is valid
         */
        [[nodiscard]] bool IsValid() const { return mValid; }

        /*!
         * @brief String representation of the MD5
         * @return string
         */
        [[nodiscard]] String ToString() const;
    };

    //! Bios root
    const Path mBiosRoot;
    static Path sBiosPath;

    //! Bios path
    Path mPath[sMaxBiosPath];
    //! Core list
    String mCores;
    //! Optional notes
    String mNotes;
    //! hashes for bios with multiple possible hashes
    std::vector<Md5Hash> mHashes;
    //! Mandatory bios
    bool mMandatory;
    //! Hash matching madatory
    bool mHashMatchMandatory;

    //! Real File hash
    Md5Hash mRealFileHash;
    //! Scan status
    Status mStatus;
    //! Work status
    ReportStatus mReportStatus;

    //! Bios move status
    bool mMoved;
    //! Bios move error status
    bool mMoveFailed;

    //! Get path string
    [[nodiscard]] String PathString(int index) const;

  public:
    /*!
     * @brief Default constructor
     */
    Bios();

    /*!
     * @brief Deserialization constructor
     * @param biosNode Xml node to deserialize from
     */
    explicit Bios(const XmlNode& biosNode);

    /*!
     * @brief Is the current bios valid?
     * @return True if
     */
    [[nodiscard]] bool IsValid() const { return !mPath[0].IsEmpty() && !mCores.empty() && !mHashes.empty(); }

    /*!
     * @brief Scan the bios & update internal status
     */
    void Scan();

    /*!
     * @brief Check if this bios is for the given core
     * @param core Core to seek for
     * @return True of the current bios is for the given core
     */
    [[nodiscard]] bool IsForCore(const String& core) const;

    /*!
     * @brief Generate missing bios report
     * @return Text Report
     */
    [[nodiscard]] String GenerateReport() const;

    /*
     * Accessors
     */

    //! Check if this bios is mandatory
    [[nodiscard]] bool IsMandatory() const { return mMandatory; }

    //! Check if hashmatching is mandatory for this bios
    [[nodiscard]] bool IsHashMatchingMandatory() const { return mHashMatchMandatory; }

    //! Report real status
    [[nodiscard]] Status BiosStatus() const { return mStatus; }

    //! Report light status
    [[nodiscard]] ReportStatus LightStatus() const { return mReportStatus; }

    //! Report real status
    [[nodiscard]] const char* BiosStatusAsString() const
    {
      switch(mStatus)
      {
        case Status::Unknown: break;
        case Status::FileNotFound: return "FileNotFound";
        case Status::HashNotMatching: return "HashNotMatching";
        case Status::HashMatching: return "HashMatching";
      }
      return "unknown";
    }

    //! Report light status
    [[nodiscard]] const char* LightStatusAsString() const
    {
      switch(mReportStatus)
      {
        case ReportStatus::Unknown: break;
        case ReportStatus::Green: return "Green";
        case ReportStatus::Yellow: return "Yellow";
        case ReportStatus::Red: return "Red";
      }
      return "unknown";
    }

    //! Return move status
    [[nodiscard]] bool MoveStatus() const { return mMoved; }
    //! Return move error status
    [[nodiscard]] bool MoveErrorStatus() const { return mMoveFailed; }

    /*!
     * @brief Check the given md5 against the known md5 list
     * and return true if a matching is found
     * @param md5 MD5 to check
     * @return True if the given md5 match one of the known MD5
     */
    bool IsMD5Known(const String& md5) const;

    /*!
     * @brief Get bios name
     * @param shorten Get a short name instead of the long path
     * @return bios name
     */
    [[nodiscard]] String Filename(bool shorten = true) const;

    //! Bios file path
    [[nodiscard]] const Path& Filepath() const { return mPath[0]; }

    //! Bios file path
    [[nodiscard]] String FilepathWithParent() const { return (sBiosPath / mPath[0]).ToString(); }

    //! Core list
    [[nodiscard]] const String& Cores() const { return mCores; }

    //! Note
    [[nodiscard]] const String& Notes() const { return mNotes; }

    //! Current bios MD5
    [[nodiscard]] String CurrentMD5() const { return mRealFileHash.ToString(); }

    //! All MD5 list
    [[nodiscard]] String::List MD5List() const;

    //! MD5 Count
    [[nodiscard]] int MD5Count() const { return (int)mHashes.size(); }
};
