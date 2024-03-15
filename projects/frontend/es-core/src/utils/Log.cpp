#include "Log.h"
#include <unistd.h>
#include <fcntl.h>
#include "RootFolders.h"
#include "utils/datetime/DateTime.h"
#include "utils/os/system/Mutex.h"

bool Log::sLogsRotated = false;
Path Log::sPath[sTypeCount] =
{
  RootFolders::DataRootFolder / "system/logs" / "frontend.log",
  RootFolders::DataRootFolder / "system/logs" / "themes.log",
};
LogLevel Log::sReportingLevel[sTypeCount] = { LogLevel::LogInfo, LogLevel::LogInfo };

const char* Log::sStringLevel[] =
{
  "ERROR",
  "WARN!",
  "INFO ",
	"DEBUG",
  "TRACE",
};

Log::Log(LogType type, LogLevel level)
  : mType(type)
  , mMessageLevel(level)
{
  // Not thread safe but quick test
  if (!sLogsRotated)
  {
    static Mutex sLocker;
    Mutex::AutoLock lock(sLocker);
    // Check again under the mutex so that we ensure only once rotation is done
    if (!sLogsRotated)
    {
      sLogsRotated = true;
      RotateLogs();
    }
  }
}

void Log::SetAllReportingLevel(LogLevel level)
{
  for(int i = sTypeCount; --i >= 0;)
    sReportingLevel[i] = level;
}

void Log::SetAllMinimumReportingLevel(LogLevel level)
{
  for(int i = sTypeCount; --i >= 0;)
    if (sReportingLevel[i] < level)
      sReportingLevel[i] = level;
}

void Log::RotateLogs()
{
  for(int i = sTypeCount; --i >= 0;)
  {
    Path backup1 = sPath[i].ChangeExtension(".backup");
    Path backup2 = sPath[i].ChangeExtension(".backup-2");
    Path backup3 = sPath[i].ChangeExtension(".backup-3");
    (void)backup3.Delete();
    Path::Rename(backup2, backup3);
    Path::Rename(backup1, backup2);
    Path::Rename(sPath[i], backup1);
  }
}

Log& Log::get()
{
	mMessage.Append('[')
	        .Append(DateTime().ToPreciseTimeStamp())
	        .Append("] (")
	        .Append(sStringLevel[(int)mMessageLevel])
	        .Append(") : ");

	return *this;
}

Log::~Log()
{
  mMessage += '\n';
  int fd = open(sPath[(int)mType].ToChars(), O_CREAT | O_APPEND | O_WRONLY | O_NOATIME | O_SYNC, 0666);
  if (fd >= 0)
  {
    write(fd, mMessage.data(), mMessage.size());
    close(fd);
  }
  else printf("*** Unable to log! *** %s", mMessage.c_str());

  if(mMessageLevel == LogLevel::LogError || sReportingLevel[(int)mType] >= LogLevel::LogDebug)
    (void)fputs(mMessage.c_str(), stdout);
}
