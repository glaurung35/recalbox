#ifndef _LOG_H_
#define _LOG_H_

#define LOG(level) \
if (::LogLevel::level <= ::Log::getReportingLevel()) ::Log().get(LogLevel::level)

#include <string>
#include <utils/os/fs/Path.h>
#include "Strings.h"

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
    ~Log();
    Log& get(LogLevel level = LogLevel::LogInfo);

    static LogLevel getReportingLevel() { return reportingLevel; }
    static void setReportingLevel(LogLevel level) { reportingLevel = level; }

    static void open(const char* filename = nullptr);
    static void close();

    Log& operator << (char v) { mMessage.Append(v); return *this; }
    Log& operator << (const char* v) { mMessage.Append(v); return *this; }
    Log& operator << (const std::string& v) { mMessage.Append(v); return *this; }
    Log& operator << (const String& v) { mMessage.Append(v); return *this; }
    Log& operator << (int v) { mMessage.Append(v); return *this; }
    Log& operator << (unsigned int v) { mMessage.Append(v); return *this; }
    Log& operator << (long long v) { mMessage.Append(v); return *this; }
    Log& operator << (unsigned long long v) { mMessage.Append(v); return *this; }
    Log& operator << (long v) { mMessage.Append((long long)v); return *this; }
    Log& operator << (unsigned long v) { mMessage.Append((unsigned long long)v); return *this; }
    Log& operator << (bool v) { mMessage.Append(v); return *this; }
    Log& operator << (float v) { mMessage.Append(v, 4); return *this; }
    Log& operator << (const String::List& v) { for(const std::string& s : v) mMessage.Append(s).Append(' '); return *this; }

  private:
    static const char* sStringLevel[(int)LogLevel::_Count];
    static FILE* sFile;
    static LogLevel reportingLevel;
    String mMessage;
    LogLevel messageLevel;

    static Path getLogPath(const char* filename);

    static void flush();

    static void doClose();
};

#endif
