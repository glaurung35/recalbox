//
// Created by bkg2k on 12/12/23.
//

#include "ThemeManager.h"
#include "RootFolders.h"
#include <systems/SystemData.h>

ThemeManager::ThemeManager()
  : StaticLifeCycleControler<ThemeManager>("theme-manager")
{
  LoadMainTheme();
}

ThemeManager::~ThemeManager()
{
  for(auto& kv : mSystem)
    delete kv.second;
}

void ThemeManager::LoadMainTheme()
{
  // Check theme validity
  String theme = RecalboxConf::Instance().GetThemeFolder();
  ThemeList list = AvailableThemes();
  if (!list.contains(theme))
    RecalboxConf::Instance().SetThemeFolder(list.begin()->second.FilenameWithoutExtension());

  // Locate main theme file
  Path root;
  mRootPath = list[RecalboxConf::Instance().GetThemeFolder()];
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

  { LOG(LogInfo) << "[ThemeManager] Theme folder: " << mRootPath; }

  mMain.LoadMain(root);
  mMenu.Load(mMain);
}

void ThemeManager::LoadSystemTheme(const SystemData* system)
{
  Path systemPath;
  // Check root/<system>/theme.xml first
  if (systemPath = mRootPath / system->Descriptor().ThemeFolder() / sRootThemeFile; !systemPath.Exists())
    // Then check root/default/theme.xml
    if (systemPath = mRootPath / sDefaultSystemThemeFolder / sRootThemeFile; !systemPath.Exists())
      // Then check root/theme.xml
      systemPath = mRootPath / sRootThemeFile;

  CreateOrGetSystem(system).LoadSystem(system->Descriptor().ThemeFolder(), systemPath);
}

void ThemeManager::ThemeChanged(WindowManager& window)
{

}

ThemeData& ThemeManager::CreateOrGetSystem(const SystemData* system)
{
  ThemeData** theme = mSystem.try_get(system);
  if (theme == nullptr) theme = &(mSystem[system] = new ThemeData());
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

