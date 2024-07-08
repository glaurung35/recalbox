//
// Created by bkg2k on 30/05/24.
//

#include "GuiMenuUserScripts.h"
#include "games/GameFilesUtils.h"
#include <usernotifications/NotificationManager.h>

GuiMenuUserScripts::GuiMenuUserScripts(WindowManager& window)
  : GuiMenuBase(window, "USER SCRIPTS", this)
  , mSender(*this)
  , mWaiter(nullptr)
  , mScriptIndexToRun(0)
{
  int index = 0;
  Path::PathList scripts = NotificationManager::Instance().GetManualScriptList();
  std::sort(scripts.begin(), scripts.end());
  for(const Path& path : scripts)
    AddSubMenu(ExtractScriptName(path), index++);
}

String GuiMenuUserScripts::ExtractScriptName(const Path& script)
{
  return GameFilesUtils::RemoveParenthesis(script.FilenameWithoutExtension());
}

void GuiMenuUserScripts::SubMenuSelected(int id)
{
  mScriptIndexToRun = id;
  Thread::Start("script");
}

void GuiMenuUserScripts::ScriptStarts(const Path& script, ScriptAttributes attributes)
{
  Mutex::AutoLock locker(mGuardian);
  mPendingData.push_back(EventData(script, attributes));
  mSender.Send();
}

void GuiMenuUserScripts::ScriptOutputLine(const Path& script, ScriptAttributes attributes, const String& line)
{
  Mutex::AutoLock locker(mGuardian);
  mPendingData.push_back(EventData(script, attributes, line));
  mSender.Send();
}

void GuiMenuUserScripts::ScriptCompleted(const Path& script, ScriptAttributes attributes, const String& output, bool error, const String& errors)
{
  Mutex::AutoLock locker(mGuardian);
  mPendingData.push_back(EventData(script, attributes, output, error, errors));
  mSender.Send();
}

void GuiMenuUserScripts::Run()
{
  NotificationManager::Instance().RunManualScriptAt(mScriptIndexToRun, this);
}

void GuiMenuUserScripts::ReceiveSyncMessage()
{
  for(;;)
  {
    mGuardian.Lock();
    if (mPendingData.empty()) { mGuardian.UnLock(); break; }
    EventData data = mPendingData.front();
    mPendingData.erase(mPendingData.begin());
    mGuardian.UnLock();

    switch (data.mType)
    {
      case ScriptEvent::Start:
      {
        if ((data.mAttributes & ScriptAttributes::Synced) == 0)
          mWindow.displayMessage((_F(_("Script {0} started successfully!")) / ExtractScriptName(data.mPath)).ToString());
        else
        {
          mWaiter = new GuiASyncWaiter(mWindow, (_F(_("Running {0}...")) / ExtractScriptName(data.mPath)).ToString());
          mWindow.pushGui(mWaiter);
        }
        break;
      }
      case ScriptEvent::Line:
      {
        if (mWaiter != nullptr)
        {
          int percent = 0;
          if (data.mLastLine.StartsWith('%') && data.mLastLine.TryAsInt(1, 0, percent))
          {
            mWaiter->SetProgress(percent);
            //{ LOG(LogDebug) << "Percent: " << percent; }
          }
          else
          {
            mWaiter->SetProgressText(data.mLastLine);
            //{ LOG(LogDebug) << "Line: " << data.mLastLine; }
          }
        }
        break;
      }
      case ScriptEvent::Stop:
      {
        if (mWaiter != nullptr)
          mWaiter->Close();

        if (hasFlag(data.mAttributes, ScriptAttributes::Synced))
        {
          String title = _("Script execution complete");
          if (data.mError)
          {
            String text = (_F(_("Script {0} has failed!")) / ExtractScriptName(data.mPath)).ToString();
            if (!data.mStdErr.empty())
              text.Append('\n').Append(_("Width the following Error output:")).Append('\n').Append('\n').Append(data.mStdErr);
            mWindow.displayScrollMessage(title, text);
          }
          String text = (_F(_("Script {0} executed successfully!")) / ExtractScriptName(data.mPath)).ToString();
          if (!hasFlag(data.mAttributes, ScriptAttributes::ReportProgress) && !data.mStdOut.empty())
            text.Append('\n').Append(_("Width the following output:")).Append('\n').Append('\n').Append(data.mStdOut);
          mWindow.displayScrollMessage(title, text);
        }
        break;
      }
    }
  }
}
