#ifndef _LOG_H_
#define _LOG_H_

#include <utils/String.h>
#include <utils/os/fs/Path.h>

//! Log types
enum class LogType
{
  Generic, //!< Default logs
  Themes,  //!< Theme logs
  __Count,
};

static constexpr const int sTypeCount = (int)LogType::__Count;

#define LOG(level) \
if (::LogLevel::level <= ::Log::ReportingLevel(LogType::Generic)) ::Log(LogType::Generic, LogLevel::level).get()

#define LOGT(level) \
if (::LogLevel::level <= ::Log::ReportingLevel(LogType::Themes)) ::Log(LogType::Themes, LogLevel::level).get()

//! Log level
enum class LogLevel
{
	LogError   = 0, //!< Error messages
	LogWarning = 1, //!< Warning messages
	LogInfo    = 2, //!< Information message
	LogDebug   = 3, //!< Debug message
  LogTrace   = 4, //!< Trace message (Internal debug!)
  _Count
};

class Log
{
  public:
    Log(LogType type, LogLevel level);
    ~Log();
    Log& get();

    static void SetPath(LogType type, const Path& path) { sPath[(int)type] = path; }
    static LogLevel ReportingLevel(LogType type) { return sReportingLevel[(int)type]; }
    static void SetReportingLevel(LogType type, LogLevel level) { sReportingLevel[(int)type] = level; }
    static void SetAllReportingLevel(LogLevel level);
    static void SetAllMinimumReportingLevel(LogLevel level);

    Log& operator << (char v) { mMessage.Append(v); return *this; }
    Log& operator << (const char* v) { mMessage.Append(v); return *this; }
    Log& operator << (const String& v) { mMessage.Append(v); return *this; }
    Log& operator << (const Path& v) { mMessage.Append(v.ToString()); return *this; }
    Log& operator << (int v) { mMessage.Append(v); return *this; }
    Log& operator << (unsigned int v) { mMessage.Append(v); return *this; }
    Log& operator << (long long v) { mMessage.Append(v); return *this; }
    Log& operator << (unsigned long long v) { mMessage.Append(v); return *this; }
    Log& operator << (long v) { mMessage.Append((long long)v); return *this; }
    Log& operator << (unsigned long v) { mMessage.Append((unsigned long long)v); return *this; }
    Log& operator << (bool v) { mMessage.Append(v); return *this; }
    Log& operator << (float v) { mMessage.Append(v, 4); return *this; }
    Log& operator << (const String::List& v) { for(const String& s : v) mMessage.Append(s).Append(' '); return *this; }
    Log& operator << (const std::vector<const char*>& v) { for(const char* s : v) mMessage.Append(s).Append(' '); return *this; }

  private:
    static bool sLogsRotated;
    static const char* sStringLevel[(int)LogLevel::_Count];
    static FILE* sFile;
    static Path sPath[sTypeCount];
    static LogLevel sReportingLevel[sTypeCount];

    LogType mType;
    LogLevel mMessageLevel;
    String mMessage;

    static void RotateLogs();
};

#endif
