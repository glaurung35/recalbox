//
// Created by bkg2k on 12/12/23.
//

#include "ThemeManager.h"
#include "RootFolders.h"
#include "guis/GuiSyncWaiter.h"
#include "utils/locale/LocaleHelper.h"
#include <systems/SystemData.h>

ThemeManager::ThemeManager(IGlobalVariableResolver& globalResolver)
  : StaticLifeCycleControler<ThemeManager>("theme-manager")
  , mMain(mCache, nullptr, globalResolver)
  , mWaiter(nullptr)
  , mGlobalResolver(globalResolver)
{
  ThemeSupport::InitializeStatics();
}

void ThemeManager::Initialize(WindowManager* window)
{
  DoThemeChange(window);
}

ThemeManager::~ThemeManager()
{
  for(auto& kv : mSystem)
    delete kv.second;
}

Path ThemeManager::GetThemeRootPath()
{
  // Check theme validity
  String theme = RecalboxConf::Instance().GetThemeFolder();
  ThemeList list = AvailableThemes();
  if (!list.contains(theme))
    RecalboxConf::Instance().SetThemeFolder(list.begin()->second.FilenameWithoutExtension());

  return list[RecalboxConf::Instance().GetThemeFolder()];
}

void ThemeManager::LoadMainTheme()
{
  // Locate main theme file
  Path root;
  mRootPath = GetThemeRootPath();
  if (root = mRootPath / sRootThemeFile; !root.Exists())
  {
    // Try locating in subfolders
    for(const Path& sub : mRootPath.GetDirectoryContent(true))
      if ((sub / sRootThemeFile).Exists())
      {
        root = sub;
        break;
      }
  }

  mMain.Reset();
  mMain.LoadMain(root);
  mMenu.Load(mMain);
}

void ThemeManager::LoadSystemTheme(const SystemData& system)
{
  Path systemPath;
  // Check root/<system>/theme.xml first
  if (systemPath = mRootPath / system.Descriptor().ThemeFolder() / sRootThemeFile; !systemPath.Exists())
    // Then check root/default/theme.xml
    if (systemPath = mRootPath / sDefaultSystemThemeFolder / sRootThemeFile; !systemPath.Exists())
      // Then check root/theme.xml
      systemPath = mRootPath / sRootThemeFile;

  ThemeData& systemTheme = CreateOrGetSystem(&system);
  systemTheme.Reset();
  systemTheme.LoadSystem(system.Descriptor().ThemeFolder(), systemPath);
}

const SystemData* ThemeManager::ThreadPoolRunJob(const SystemData*& feed)
{
  LoadSystemTheme(*feed);
  return feed;
}

void ThemeManager::DoThemeChange(WindowManager* window, bool force)
{
  DateTime start;
  Path newPath = GetThemeRootPath();
  if (mRootPath.IsEmpty()) { LOG(LogInfo) << "[ThemeManager] Loading initial theme: " << newPath; }
  else if (newPath != mRootPath) { LOG(LogInfo) << "[ThemeManager] Switching to new theme: " << newPath; }
  else { LOG(LogInfo) << "[ThemeManager] Current theme options havre changed. Refreshing theme: " << newPath; }

  //! Clear cache if required
  bool update = (newPath == mRootPath);
  if (!update || force) mCache.Clear();

  if (!mRootPath.IsEmpty())
  {
    mWaiter = new GuiSyncWaiter(*window, update ? _("Updating current theme...") :
                               (_F(_("Loading new theme {0}")) / newPath.FilenameWithoutExtension()).ToString());
    mWaiter->Show();
    mWaitBarReference = DateTime();
  }

  // Reload Main themes
  LoadMainTheme();
  // Reload system themes
  if (!mSystem.empty())
  {
    ThreadPool<const SystemData*, const SystemData*> pool(this, "theme-loader", false);
    for (auto& kv: mSystem) pool.PushFeed(kv.first);
    pool.Run(-2, false);
  }
  DateTime start2;
  { LOG(LogWarning) << "[ThemeManager] Load time: " << (start2 - start).ToMillisecondsString() << " ms"; }
  // Refresh
  NotifyThemeChanged(update);
  { LOG(LogWarning) << "[ThemeManager] Refresh time: " << (DateTime() - start2).ToMillisecondsString() << " ms"; }

  if (mWaiter != nullptr)
  {
    mWaiter->Hide();
    delete mWaiter;
    mWaiter = nullptr;
  }
}

void ThemeManager::ThemeSwitchTick()
{
  if (mWaiter != nullptr)
    if (DateTime now; (now - mWaitBarReference).TotalMilliseconds() > 300)
    {
      mWaiter->Refresh();
      mWaitBarReference = now;
    }
}

void ThemeManager::NotifyThemeChanged(bool refreshOnly)
{
  for(IThemeSwitchable* switchable : mSwitchables)
    if (switchable != nullptr)
    {
      if (SystemData* system = switchable->SystemTheme(); system != nullptr)
        switchable->SwitchToTheme(CreateOrGetSystem(system), refreshOnly, this);
      else
        switchable->SwitchToTheme(mMain, refreshOnly, this);
    }
}

ThemeData& ThemeManager::CreateOrGetSystem(const SystemData* system)
{
  ThemeData** theme = mSystem.try_get(system);
  if (theme == nullptr) theme = &(mSystem[system] = new ThemeData(mCache, system, mGlobalResolver));
  return **theme;
}

HashMap<String, Path> ThemeManager::AvailableThemes()
{
  HashMap<String, Path> sets;

  static Path paths[] =
    {
      RootFolders::TemplateRootFolder / "/system/.emulationstation/themes",
      RootFolders::DataRootFolder     / "/themes",
      RootFolders::DataRootFolder     / "/system/.emulationstation/themes"
    };

  for (const Path& master : paths)
  {
    Path::PathList list = master.GetDirectoryContent();
    for(const Path& path : list)
      if (path.IsDirectory() && (path / sRootThemeFile).Exists())
      {
        String name = path.FilenameWithoutExtension();
        while(sets.contains(name))
          if (name.Count() >= 3 && name[name.Count() - 2] == '#' && name[name.Count() - 3] == ' ') name[name.Count()-2]++;
          else name.Append(" #2");
        sets[name] = path;
      }
  }

  return sets;
}




