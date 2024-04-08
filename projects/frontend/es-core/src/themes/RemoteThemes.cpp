//
// Created by bkg2k on 08/04/24.
//

#include "RemoteThemes.h"
#include <patreon/PatronInfo.h>

bool RemoteThemes::FetchRemoteThemes()
{
  mRemoteThemeList.clear();
  if (!FetchFrom(sPublicRepository))
  {
    { LOG(LogError) << "[RemoteThemes] Cannot fetch public themes !"; }
    return false;
  }
  if (PatronInfo::Instance().IsPatron())
    if (!FetchFrom(sPrivateRepository))
    { LOG(LogError) << "[RemoteThemes] Cannot fetch private themes !"; }
  return true;
}

bool RemoteThemes::FetchFrom(const String& url)
{
  // Download
  /*HttpClient client;
  String output;
  for(int i = 3; -i >= 0; Thread::Sleep(1000))
    if (client.Execute(url, output))
      break;
  if (output.StartsWith('<') or output.empty())
  {
    { LOG(LogError) << "[RemoteThemes] Unable to fetch content from " << url; }
    return false;
  }*/
  String output = "[Le-nom-de-la-section-de-mon-theme]\n"
                  "\n"
                  "# Description du thème\n"
                  "folder = mytheme\n"
                  "name = RappelBoite Default Theme\n"
                  "author = Beka Jaydeukha\n"
                  "version = 3.2\n"
                  "fromrecalbox = 9.2\n"
                  "description = Mais tu dis que le bonheur est irréductible, et je dis que ton espoir n'est pas si désespéré à condition d'analyser que l'absolu ne doit pas être annihilé par l'illusoire précarité de nos amours et qu'il ne faut pas cautionner l'irréalité sous des aspérités absentes et désenchantées de nos pensées iconoclastes et désoxydées ear nos désirs excommuniés de la fatalité destituée, et vice et versa\n"
                  "\n"
                  "# Illustrations, une à plusieurs images par type. Les types sont independant, vous pouvez avoir 6 images pour les systèmes, 3 pour les gamelists et 2 pour les menus\n"
                  "images.systems.1 = system-screenshot1.png\n"
                  "images.systems.2 = system-screenshot2.png\n"
                  "images.games.1 = gamelist-screenshot1.png\n"
                  "images.games.2 = gamelist-screenshot2.png\n"
                  "images.menus.1 = menu-screenshot1.png\n"
                  "images.menus.2 = menu-screenshot3.png\n"
                  "\n"
                  "images.crt.systems.1 = system-screenshot1.png\n"
                  "images.crt.systems.2 = system-screenshot2.png\n"
                  "images.crt.games.1 = gamelist-screenshot1.png\n"
                  "images.crt.games.2 = gamelist-screenshot2.png\n"
                  "images.crt.menus.1 = menu-screenshot1.png\n"
                  "images.crt.menus.2 = menu-screenshot3.png\n"
                  "\n"
                  "images.tate.systems.1 = system-screenshot1.png\n"
                  "images.tate.systems.2 = system-screenshot2.png\n"
                  "images.tate.games.1 = gamelist-screenshot1.png\n"
                  "images.tate.games.2 = gamelist-screenshot2.png\n"
                  "images.tate.menus.1 = menu-screenshot1.png\n"
                  "images.tate.menus.2 = menu-screenshot3.png\n"
                  "\n"
                  "images.crt.tate.systems.1 = system-screenshot1.png\n"
                  "images.crt.tate.systems.2 = system-screenshot2.png\n"
                  "images.crt.tate.games.1 = gamelist-screenshot1.png\n"
                  "images.crt.tate.games.2 = gamelist-screenshot2.png\n"
                  "images.crt.tate.menus.1 = menu-screenshot1.png\n"
                  "images.crt.tate.menus.2 = menu-screenshot3.png\n"
                  "\n"
                  "# Detail des fichiers et de leurs cibles en terme de compatibilité et de resolutions\n"
                  "# Pour un thème classique, il n'y en aura qu'un. Pour les thème chargés en images et découpés par résolutions, il peut y en avoir plusieurs\n"
                  "1.zipfile = tagada.zip\n"
                  "1.compatiblity = hdmi\n"
                  "1.resolutions = fhd,hd\n";

  // Analyze
  SectionFile sections(output, true);
  if (sections.SectionCount() == 0)
  {
    { LOG(LogError) << "[RemoteThemes] No valid section read from " << url; }
    return false;
  }

  // Deserialize
  for(const String& sectionName : sections.GetSectionNames())
    Deserialize(sections, sectionName);

  return true;
}

void RemoteThemes::Deserialize(const SectionFile& sections, const String& sectionName)
{
  // Extract main data
  String folder = sections.AsString(sectionName, "folder");
  if (folder.empty()) { LOG(LogError) << "[RemoteThemes] Unspecified folder in section " << sectionName; return; }
  String name = sections.AsString(sectionName, "name");
  if (name.empty()) { LOG(LogError) << "[RemoteThemes] Unspecified folder in section " << sectionName; return; }
  String author = sections.AsStringDefault(sectionName, "author", "Unknown");
  String description = sections.AsString(sectionName, "description");
  String version = sections.AsString(sectionName, "version");
  String minrecalbox = sections.AsString(sectionName, "fromrecalbox");

  // Create remote theme object
  RemoteTheme theme(folder, author, name, description, version, minrecalbox);

  // Extract images lists
  RemoteTheme::ImageLists Hdmi;
  RemoteTheme::ImageLists HdmiTate;
  RemoteTheme::ImageLists Crt;
  RemoteTheme::ImageLists CrtTate;
  for(int i = 1; ; ++i)
  {
    bool found = DeserializeImages(sections, sectionName, "", i, Hdmi);
    found |= DeserializeImages(sections, sectionName, "tate", i, HdmiTate);
    found |= DeserializeImages(sections, sectionName, "crt", i, Crt);
    found |= DeserializeImages(sections, sectionName, "crt.tate", i, CrtTate);

    if (!found)
      break;
  }
  theme.SetImageLists(std::move(Hdmi), std::move(HdmiTate), std::move(Crt), std::move(CrtTate));

  // Extract sub theme data
  for(int i = 1; ; ++i)
  {
    String zipFile = sections.AsString(sectionName, String(i).Append(".zipfile"));
    if (zipFile.empty())
    {
      if (theme.SubThemeCount() == 0) { LOG(LogError) << "[RemoteThemes] No zip file specified in section " << sectionName; return; }
      break;
    }
    ThemeData::Compatibility compatibility = ThemeData::ConvertCompatibility(sections.AsString(sectionName, String(i).Append(".compatibility")));
    ThemeData::Resolutions resolutions = ThemeData::ConvertResolutions(sections.AsString(sectionName, String(i).Append(".resolutions")));

    theme.AddSubTheme(zipFile, compatibility, resolutions);
  }

  // Store
  if (theme.SubThemeCount() != 0)
    mRemoteThemeList.push_back(theme);
}

bool RemoteThemes::DeserializeImages(const SectionFile& sections, const String& sectionName, const String& suffix, int index, RemoteTheme::ImageLists& output)
{
  bool found = false;
  String prefix("images.");
  if (!suffix.empty()) prefix.Append(suffix).Append('.');
  String systemImage = sections.AsString(sectionName, String(prefix).Append("systems.").Append(index));
  String gameImage = sections.AsString(sectionName, String(prefix).Append("games.").Append(index));
  String menuImage = sections.AsString(sectionName, String(prefix).Append("menus.").Append(index));

  if (!systemImage.empty()) found = true, output.mSystemListImages.push_back(systemImage);
  if (!gameImage.empty()) found = true, output.mGameListImages.push_back(gameImage);
  if (!menuImage.empty()) found = true, output.mMenuImages.push_back(menuImage);

  return found;
}
