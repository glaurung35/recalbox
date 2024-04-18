//
// Created by bkg2k on 08/04/24.
//

#include "RemoteThemes.h"
#include <patreon/PatronInfo.h>

bool RemoteThemes::FetchRemoteThemes()
{
  mRemoteThemeList.clear();
  if (!FetchFrom(String(sPublicRepository).Append("/-/raw/main")))
  {
    { LOG(LogError) << "[RemoteThemes] Cannot fetch public themes !"; }
    return false;
  }
  if (PatronInfo::Instance().IsPatron())
    if (!FetchFrom(String(sPrivateRepository).Append("/-/raw/main")))
    { LOG(LogError) << "[RemoteThemes] Cannot fetch private themes !"; }
  return true;
}

bool RemoteThemes::FetchFrom(const String& url)
{
  // Download
  HttpClient client;
  String output;
  for(int i = 3; --i >= 0; Thread::Sleep(1000))
    if (client.Execute(url + "/theme.json", output))
      break;
  if (output.empty() || !output.StartsWith('{'))
  {
    { LOG(LogError) << "[RemoteThemes] Unable to fetch content from " << url; }
    return false;
  }

  // Analyze
  rapidjson::Document json;
  json.Parse<rapidjson::kParseTrailingCommasFlag | rapidjson::kParseCommentsFlag>(output.data());
  if (json.HasParseError())
  {
    { LOG(LogError) << "[RemoteThemes] No valid section read from " << url; }
    return false;
  }

  // Deserialize
  rapidjson::Value& themes = json["themes"];
  if (!themes.IsArray())
  {
    { LOG(LogError) << "[RemoteThemes] No valid themes object in " << url; }
    return false;
  }
  int index = 0;
  for(rapidjson::Value& theme : themes.GetArray())
  {
    if (theme.HasMember("ignore"))
      if (theme["ignore"].GetBool())
        continue;
    Deserialize(theme, url, index++);
  }

  return true;
}

void RemoteThemes::Deserialize(rapidjson::Value& themeNode, const String& url, int index)
{
  // Extract main data
  rapidjson::Value* properties;
  if (!GetObject(themeNode, "properties", properties, url, index, false)) return;
  String folder; if (!DeserializeStringMandatory(*properties, "folder", folder, url, index)) return;
  String name;   if (!DeserializeStringMandatory(*properties, "name", name, url, index)) return;
  String author; DeserializeStringOptional(*properties, "author", author, "Unknown");
  String description; DeserializeStringOptional(*properties, "description", description, "No description");
  String version; DeserializeStringOptional(*properties, "version", version, "1.0");
  String fromrecalbox; DeserializeStringOptional(*properties, "fromrecalbox", fromrecalbox, "9.2");

  // Create remote theme object
  RemoteTheme theme(folder, author, name, description, version, fromrecalbox, url);

  // Extract images lists
  rapidjson::Value* illustrations;
  if (!GetObject(themeNode, "illustrations", illustrations, url, index, false)) return;
  RemoteTheme::ImageLists Hdmi;     DeserializeImages(*illustrations, "hdmi", Hdmi, url, index);
  RemoteTheme::ImageLists HdmiTate; DeserializeImages(*illustrations, "tate", HdmiTate, url, index);
  RemoteTheme::ImageLists Crt;      DeserializeImages(*illustrations, "crt", Crt, url, index);
  RemoteTheme::ImageLists CrtTate;  DeserializeImages(*illustrations, "crttate", CrtTate, url, index);
  if (Hdmi.IsEmpty() && HdmiTate.IsEmpty() && Crt.IsEmpty() && CrtTate.IsEmpty())
  { LOG(LogError) << "[RemoteThemes] No illustration available in theme at index " << index << " in " << url; return; }
  theme.SetImageLists(std::move(Hdmi), std::move(HdmiTate), std::move(Crt), std::move(CrtTate));

  // Extract sub theme data
  rapidjson::Value* data;
  if (!GetArray(themeNode, "data", data, url, index)) return;
  for(rapidjson::Value& object : data->GetArray())
  {
    String subfolder;           if (!DeserializeStringMandatory(object, "folder", subfolder, url, index)) return;
    String compatibilityString; if (!DeserializeStringMandatory(object, "compatibility", compatibilityString, url, index)) return;
    String resolutionsString;   if (!DeserializeStringMandatory(object, "resolutions", resolutionsString, url, index)) return;
    ThemeData::Compatibility compatibility = ThemeData::ConvertCompatibility(compatibilityString);
    ThemeData::Resolutions resolutions = ThemeData::ConvertResolutions(resolutionsString);
    theme.AddSubTheme(subfolder, compatibility, resolutions);
  }

  // Store
  if (theme.SubThemeCount() != 0)
    mRemoteThemeList.push_back(theme);
}

bool RemoteThemes::DeserializeImages(rapidjson::Value& object, const char* name, RemoteTheme::ImageLists& imageLists, const String& url, int index)
{
  rapidjson::Value* lists;
  if (!GetObject(object, name, lists, url, index, true)) return false;
  if (!DeserializeArrayOfString(*lists, "systemlist", imageLists.mSystemListImages, url, index)) return false;
  if (!DeserializeArrayOfString(*lists, "gamelist", imageLists.mGameListImages, url, index)) return false;
  if (!DeserializeArrayOfString(*lists, "menu", imageLists.mMenuImages, url, index)) return false;
  return true;
}

bool RemoteThemes::GetObject(rapidjson::Value& theme, const char* name, rapidjson::Value*& object, const String& url, int index, bool optional)
{
  if (!theme.HasMember(name))
  {
    if (!optional)
    {
      if (index >= 0) { LOG(LogError) << "[RemoteThemes] Cannot find object " << name << " in theme array index " << index << " in " << url; }
      else { LOG(LogError) << "[RemoteThemes] Cannot find object " << name << " in " << url; }
    }
    return false;
  }
  object = &theme[name];
  if (!object->IsObject())
  {
    if (index >= 0) { LOG(LogError) << "[RemoteThemes] Item " << name << " is not an object, in theme array index " << index << " in " << url; }
    else { LOG(LogError) << "[RemoteThemes] Item " << name << " is not an object, in " << url; }
    object = nullptr;
    return false;
  }
  return true;
}

bool RemoteThemes::GetArray(rapidjson::Value& object, const char* name, rapidjson::Value*& array, const String& url, int index)
{
  if (!object.HasMember(name))
  {
    if (index >= 0) { LOG(LogError) << "[RemoteThemes] Cannot find array " << name << " in theme array index " << index << " in " << url; }
    else { LOG(LogError) << "[RemoteThemes] Cannot find array " << name << " in " << url; }
    return false;
  }
  array = &object[name];
  if (!array->IsArray())
  {
    if (index >= 0) { LOG(LogError) << "[RemoteThemes] Item " << name << " is not an array, in theme array index " << index << " in " << url; }
    else { LOG(LogError) << "[RemoteThemes] Item " << name << " is not an array, in " << url; }
    array = nullptr;
    return false;
  }
  return true;
}

bool RemoteThemes::DeserializeStringMandatory(rapidjson::Value& object, const char* name, String& string, const String& url, int index)
{
  if (!object.HasMember(name))
  {
    if (index >= 0) { LOG(LogError) << "[RemoteThemes] Cannot find string " << name << " in theme array index " << index << " in " << url; }
    else { LOG(LogError) << "[RemoteThemes] Cannot find string " << name << " in " << url; }
    return false;
  }
  string = object[name].GetString();
  return true;
}

bool RemoteThemes::DeserializeStringOptional(rapidjson::Value& object, const char* name, String& string, const String& defaultValue)
{
  string = object.HasMember(name) ? object[name].GetString() : defaultValue;
  return true;
}

bool RemoteThemes::DeserializeArrayOfString(rapidjson::Value& object, const char* name, String::List& list, const String& url, int index)
{
  if (!object.HasMember(name))
  {
    if (index >= 0) { LOG(LogError) << "[RemoteThemes] Cannot find array " << name << " in theme array index " << index << " in " << url; }
    else { LOG(LogError) << "[RemoteThemes] Cannot find array " << name << " in " << url; }
    return false;
  }
  rapidjson::Value& array = object[name];
  if (!array.IsArray())
  {
    if (index >= 0) { LOG(LogError) << "[RemoteThemes] Item " << name << " is not an array, in theme array index " << index << " in " << url; }
    else { LOG(LogError) << "[RemoteThemes] Item " << name << " is not an array, in " << url; }
    return false;
  }
  for(const rapidjson::Value& value : array.GetArray())
    list.push_back(value.GetString());
  return true;
}

void RemoteThemes::FetchIllustration(int themeIndex, bool crt, bool tate)
{
  if ((unsigned int)themeIndex < mRemoteThemeList.size())
  {
    const RemoteTheme::ImageLists& imageLists = mRemoteThemeList[themeIndex].SelectImageLists(crt, tate);
    PushImageListsToDownloadQueue(imageLists, themeIndex);
  }
  else { LOG(LogError) << "[RemoteThemes] Out of range theme index. Index " << themeIndex << " out of " << mRemoteThemeList.size(); }
}

const RemoteThemes::RemoteTheme::ImageLists& RemoteThemes::RemoteTheme::SelectImageLists(bool crt, bool tate)
{
  if (tate && crt && !mImagesCrtTate.IsEmpty()) return mImagesCrtTate;
  if (crt && !mImagesCrt.IsEmpty()) return mImagesCrt;
  if (tate && !mImagesHdmiTate.IsEmpty()) return mImagesHdmiTate;
  return mImagesHdmi;
}

void RemoteThemes::PushImageListsToDownloadQueue(const RemoteThemes::RemoteTheme::ImageLists& imageList, int themeIndex)
{
  if ((unsigned int)themeIndex < mRemoteThemeList.size())
  {
    // Get base url
    const RemoteTheme& theme = mRemoteThemeList[themeIndex];
    String url = theme.BaseUrl();
    url.Append('/').Append(theme.ThemeFolder());

    int imageIndex = 0;
    // SystemList first
    for(const String& illustration : imageList.mSystemListImages)
      PushImageToDownloadQueue(url, illustration, themeIndex, RemoteIllustrationType::SystemList, imageIndex++);
    // Gamelist then
    for(const String& illustration : imageList.mGameListImages)
      PushImageToDownloadQueue(url, illustration, themeIndex, RemoteIllustrationType::GameList, imageIndex++);
    // Menu last
    for(const String& illustration : imageList.mMenuImages)
      PushImageToDownloadQueue(url, illustration, themeIndex, RemoteIllustrationType::Menu, imageIndex++);
  }
  else { LOG(LogError) << "[RemoteThemes] Out of range theme index. Index " << themeIndex << " out of " << mRemoteThemeList.size(); }
}

void RemoteThemes::PushImageToDownloadQueue(const String& baseUrl, const String& filename, int themeIndex, RemoteIllustrationType type, int index)
{
  // Get base path
  Path targetPath = Path(sDownloadFolder) / filename;
  // Check
  if (targetPath.Exists())
  {
    if (mNotifier != nullptr) mNotifier->IllustrationReady(themeIndex, type, index, targetPath);
    return;
  }

  // Enqueue
  mLocker.Lock();
  mQueue.push_back(Download(std::move(String(baseUrl).Append('/').Append(filename)), std::move(targetPath), themeIndex, type, index));
  mSignal.Fire();
  mLocker.UnLock();
}

void RemoteThemes::Run()
{
  while(IsRunning())
  {
    mSignal.WaitSignal();

    while(IsRunning())
    {
      // Get next download
      Download download;
      {
        Mutex::AutoLock locker(mLocker);
        if (mQueue.empty()) break; /// Exit download loop
        download = mQueue.back();
        mQueue.pop_back();
      }

      // Download
      HttpClient client;
      bool ok = download.FilePath().Exists();
      if (!ok)
        for (int i = 3; --i >= 0; Thread::Sleep(500))
          if (client.Execute(download.Url(), download.FilePath()))
          {
            ok = true;
            break;
          }

      // Ok ? Push to complete list & send message
      if (ok)
      {
        Mutex::AutoLock locker(mLocker);
        mCompleted.push_back(download);
      }
    }
  }
}

void RemoteThemes::ReceiveSyncMessage()
{
  // Move complete download to local storage
  std::vector<Download> local;
  {
    Mutex::AutoLock locker(mLocker);
    local = mCompleted;
    mCompleted.clear();
  }

  // Notify of illustratoon availability
  if (mNotifier != nullptr)
    for(const Download& download : local)
       mNotifier->IllustrationReady(download.ThemeIndex(),
                                    download.Type(),
                                    download.Index(),
                                    download.FilePath());
}


