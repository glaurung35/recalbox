//
// Created by bkg2k on 03/04/2020.
// Last modification by Maksthorr on 28/04/2023
//

#include <pistache/include/pistache/router.h>
#include <utils/Log.h>
#include <emulators/EmulatorList.h>
#include <systems/SystemDeserializer.h>
#include <rapidjson/document.h>
#include <utils/Files.h>
#include "RequestHandlerTools.h"
#include "Mime.h"
#include <LibretroRatio.h>
#include <utils/Zip.h>
#include <systems/arcade/ArcadeVirtualSystems.h>
#include <systems/SystemManager.h>
#include <audio/AudioController.h>
#include "ResolutionAdapter.h"

using namespace Pistache;

void RequestHandlerTools::GetJSONMediaList(Pistache::Http::ResponseWriter& response)
{
  Path mediaPath("/recalbox/share/screenshots");
  Path::PathList list = mediaPath.GetDirectoryContent();

  static String imagesExtensions(".jpg|.jpeg|.png|.gif");
  static String videosExtensions(".mkv|.avi|.mp4");

  JSONBuilder result;
  result.Open()
        .Field("mediaPath", mediaPath.ToString())
        .OpenObject("mediaList");
  for(const Path& path : list)
  {
    bool ok = false;
    result.OpenObject(path.MakeRelative(mediaPath, ok).ToChars());
    String ext = path.Extension().LowerCase();
    if (imagesExtensions.find(ext) != String::npos) result.Field("type", "image");
    else if (videosExtensions.find(ext) != String::npos) result.Field("type", "video");
    else result.Field("type", "unknown");
    result.CloseObject();
  }
  result.CloseObject()
        .Close();
  RequestHandlerTools::Send(response, Http::Code::Ok, result, Mime::Json);
}

JSONBuilder RequestHandlerTools::BuildPartitionObject(const DeviceInfo& info, const String& recalboxtype)
{
  JSONBuilder result;
  result.Open()
        .Field("recalbox", recalboxtype)
        .Field("devicebus", info.Bus)
        .Field("devicetype", info.Type)
        .Field("devicemodel", info.Model)
        .Field("mount", info.Mount)
        .Field("filesystem", info.FileSystem)
        .Field("filesystemtype", info.FileSystemType)
        .Field("size", info.Size << 10)
        .Field("used", info.Used << 10)
        .Close();
  return result;
}

void RequestHandlerTools::GetDevicePropertiesOf(DeviceInfo& info)
{
  // Get info from udevadm
  String::List strings = RequestHandlerTools::OutputLinesOf(String("/sbin/udevadm info --query=property --name=").Append(info.FileSystem));
  for(const String& string : strings)
  {
    int pos = string.Find('=');
    if (pos < 0) continue;
    if (string.StartsWith(LEGACY_STRING("ID_BUS=")))
      info.Bus = string.SubString(pos + 1);
    else if (string.StartsWith(LEGACY_STRING("ID_VENDOR=")))
      info.Model.Insert(0, string.SubString(pos + 1) + ' ');
    else if (string.StartsWith(LEGACY_STRING("ID_MODEL=")))
      info.Model.Append(string.SubString(pos + 1));
    else if (string.StartsWith(LEGACY_STRING("ID_TYPE=")))
      info.Type = string.SubString(pos + 1);
    else if (string.StartsWith(LEGACY_STRING("ID_NAME=")))
      info.Model = string.SubString(pos + 1);
    else if (string.StartsWith(LEGACY_STRING("ID_FS_TYPE=")))
      info.FileSystemType = string.SubString(pos + 1);
  }
  // Try to get missing info
  if (info.Mount.StartsWith("/dev/mmcblk"))
  {
    info.Bus = "sdio";
    info.Type = "sd";
  }
  info.Model.Replace('_', ' ');
}

void RequestHandlerTools::GetSystemResourcePath(Path& regionPath, Path& basePath, const String& system, const String& region, const char* resourceFileName)
{
  String regionString(sSystemResourceRegionPath);
  regionString.Replace("%SYSTEM%", system)
              .Replace("%REGION%", region)
              .Replace("%FILE%", resourceFileName);
  regionPath = regionString;

  String baseString(sSystemResourceBasePath);
  baseString.Replace("%SYSTEM%", system)
            .Replace("%FILE%", resourceFileName);
  basePath = baseString;
}

JSONBuilder RequestHandlerTools::SerializeBiosToJSON(const Bios& bios)
{
  return JSONBuilder().Open()
                      .Field("mandatory", bios.IsMandatory())
                      .Field("hashMatchingMandatory", bios.IsHashMatchingMandatory())
                      .Field("displayFileName", bios.Filename(false))
                      .Field("notes", bios.Notes())
                      .Field("currentMd5", bios.CurrentMD5())
                      .Field("md5List", bios.MD5List())
                      .Field("cores", bios.Cores().Split(','))
                      .Field("lightStatus", bios.LightStatusAsString())
                      .Field("realStatus", bios.BiosStatusAsString())
                      .Close();
}

JSONBuilder RequestHandlerTools::SerializeBiosListToJSON(const BiosList& biosList)
{
  JSONBuilder systemJson;
  systemJson.Open()
            .Field("fullName", biosList.FullName())
            .OpenObject("scanResult")
            .Field("total", biosList.BiosCount())
            .Field("ok", biosList.TotalBiosOk())
            .Field("ko", biosList.TotalBiosKo())
            .Field("unsafe", biosList.TotalBiosUnsafe())
            .Field("notFound", biosList.TotalFileNotFound())
            .Field("hashOk", biosList.TotalHashMatching())
            .Field("hashKo", biosList.TotalHashNotMatching())
            .CloseObject()
            .OpenObject("biosList");
  for(int biosIndex = biosList.BiosCount(); --biosIndex >= 0; )
  {
    const Bios& bios = biosList.BiosAt(biosIndex);
    systemJson.Field(bios.Filepath().ToChars(), SerializeBiosToJSON(bios));
  }
  systemJson.CloseObject()
            .Close();

  return systemJson;
}

String RequestHandlerTools::OutputOf(const String& command)
{
  String output;
  FILE* pipe = popen(command.c_str(), "r");
  if (pipe != nullptr)
  {
    char buffer[1024];
    while (feof(pipe) == 0)
      if (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        output.Append(buffer);
    pclose(pipe);
  }
  return output;
}

String::List RequestHandlerTools::OutputLinesOf(const String& command)
{
  return OutputOf(command).Split('\n');
}

void RequestHandlerTools::Error404(Http::ResponseWriter& response)
{
  LOG(LogError) << "Error 404";
  SetHeaders(response);
  Send(response, Http::Code::Not_Found, "404 - Not found", Mime::PlainText);
}

JSONBuilder RequestHandlerTools::SerializeEmulatorsAndCoreToJson(const EmulatorList& emulatorlist)
{
  JSONBuilder emulators;
  emulators.Open();
  for(int i = emulatorlist.Count(); --i >= 0; )
  {
    const EmulatorDescriptor& emulator = emulatorlist.EmulatorAt(i);
    JSONBuilder CoreJson;
    CoreJson.Open();
    for(int c = emulator.CoreCount(); --c >= 0; )
      CoreJson.Field(String((int)emulator.CorePriorityAt(c)).c_str(), emulator.CoreAt(c).mName);
    CoreJson.Close();
    emulators.Field(emulator.Name().c_str(), CoreJson);
  }
  emulators.Close();
  return emulators;
}

void RequestHandlerTools::LogRoute(const Pistache::Rest::Request& request, const char* methodName)
{
  LOG(LogDebug) << methodName << ": " << MethodToString(request.method()) << ' ' << request.resource();
}

const char* RequestHandlerTools::MethodToString(Pistache::Http::Method method)
{
  switch(method)
  {
    case Http::Method::Options: return "OPTIONS";
    case Http::Method::Get: return "GET";
    case Http::Method::Post: return "POST";
    case Http::Method::Head: return "HEAD";
    case Http::Method::Put: return "PUT";
    case Http::Method::Patch: return "PATCH";
    case Http::Method::Delete: return "DELETE";
    case Http::Method::Trace: return "TRACE";
    case Http::Method::Connect: return "CONNECT";

    // Avoid enum Warning
    case Http::Method::Acl:
    case Http::Method::BaselineControl:
    case Http::Method::Bind:
    case Http::Method::Checkin:
    case Http::Method::Checkout:
    case Http::Method::Copy:
    case Http::Method::Label:
    case Http::Method::Link:
    case Http::Method::Lock:
    case Http::Method::Merge:
    case Http::Method::Mkactivity:
    case Http::Method::Mkcalendar:
    case Http::Method::Mkcol:
    case Http::Method::Mkredirectref:
    case Http::Method::Mkworkspace:
    case Http::Method::Move:
    case Http::Method::Orderpatch:
    case Http::Method::Pri:
    case Http::Method::Propfind:
    case Http::Method::Proppatch:
    case Http::Method::Rebind:
    case Http::Method::Report:
    case Http::Method::Search:
    case Http::Method::Unbind:
    case Http::Method::Uncheckout:
    case Http::Method::Unlink:
    case Http::Method::Unlock:
    case Http::Method::Update:
    case Http::Method::Updateredirectref:
    case Http::Method::VersionControl:
    default: break;
  }
  return "UNKNOWN";
}

/*
 * Send
 */


void RequestHandlerTools::SetHeaders(Http::ResponseWriter& response)
{
  response.headers().add<Pistache::Http::Header::AccessControlAllowMethods>("GET, POST, OPTIONS, DELETE");
  response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
  response.headers().add<Pistache::Http::Header::AccessControlAllowHeaders>("*");
}

void
RequestHandlerTools::Send(Pistache::Http::ResponseWriter& response, Pistache::Http::Code code, const String& body,
                          const Pistache::Http::Mime::MediaType& mime)
{
  SetHeaders(response);
  response.send(code, body, mime);
}

Pistache::Async::Promise<ssize_t> RequestHandlerTools::Send(Pistache::Http::ResponseWriter& response, Pistache::Http::Code code)
{
  SetHeaders(response);
  return response.send(code, nullptr, 0, Pistache::Http::Mime::MediaType());
}

void RequestHandlerTools::SendResource(const Path& resourcepath, Http::ResponseWriter& response, const Pistache::Http::Mime::MediaType& mimeType)
{
  if (resourcepath.Exists())
  {
    SetHeaders(response);
    Http::serveFile(response, resourcepath.ToString(), mimeType);
  }
  else
    Error404(response);
}

void RequestHandlerTools::SendResource(const Path& preferedpath, const Path& fallbackpath, Http::ResponseWriter& response, const Http::Mime::MediaType& mimetype)
{
  if (preferedpath.Exists())
  {
    SetHeaders(response);
    Http::serveFile(response, preferedpath.ToString(), mimetype);
  }
  else if (fallbackpath.Exists())
  {
    SetHeaders(response);
    Http::serveFile(response, fallbackpath.ToString(), mimetype);
  }
  else
    Error404(response);
}

const HashMap<String, Validator>& RequestHandlerTools::SelectConfigurationKeySet(const String& _namespace)
{
  enum class Namespace
  {
    Unknown,
    System,
    EmulationStation,
    Scraper,
    Kodi,
    Hyperion,
    Audio,
    Wifi,
    Wifi2,
    Wifi3,
    Controllers,
    Updates,
    Global,   //<! All emulators settings
    Specific, //<! Specific emulators settings
    Patron,
    Music,
    Hat,
  };

  static HashMap<String, Namespace> sConverter
  ({
     { "system", Namespace::System },
     { "emulationstation", Namespace::EmulationStation },
     { "scraper", Namespace::Scraper },
     { "kodi", Namespace::Kodi },
     { "hyperion", Namespace::Hyperion },
     { "audio", Namespace::Audio },
     { "wifi", Namespace::Wifi },
     { "wifi2", Namespace::Wifi2 },
     { "wifi3", Namespace::Wifi3 },
     { "controllers", Namespace::Controllers },
     { "updates", Namespace::Updates },
     { "global", Namespace::Global },
     { "specific", Namespace::Global },
     { "patron", Namespace::Patron },
     { "music", Namespace::Music },
     { "hat", Namespace::Hat },
  });

  Namespace* pns = sConverter.try_get(_namespace);
  Namespace ns = (pns != nullptr) ? *pns : Namespace::Unknown;

  switch(ns)
  {
    case Namespace::System:
    {
      static HashMap<String, Validator> sList
      ({
         { "power.switch"                          , Validator(false, { "ATX_RASPI_R2_6", "MAUSBERRY", "REMOTEPIBOARD_2003", "REMOTEPIBOARD_2005", "WITTYPI", "PIN56ONOFF", "PIN56PUSH", "PIN356ONOFFRESET", "PIN356PUSHRESET" }) },
         { "fbcp.enabled"                          , Validator(true) },
         { "splash.length"                         , Validator(-1, 300) },
         { "splash.select"                         , Validator(false, { "all", "recalbox", "custom" }) },
         { "manager.enabled"                       , Validator(true) },
         { "api.enabled"                           , Validator(true) },
         { "es.videomode"                          , Validator(GetAvailableFrontEndResolutions(), false) },
         { "emulators.specialkeys"                 , Validator(false, { "default", "nomenu", "none" }) },
         { "hostname"                              , Validator("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-") },
         { "samba.enabled"                         , Validator(true) },
         { "virtual-gamepads.enabled"              , Validator(true) },
         { "ssh.enabled"                           , Validator(true) },
         { "language"                              , Validator(GetAvailableLanguages(), false) },
         { "kblayout"                              , Validator(GetAvailableKeyboardLayout(), false) },
         { "timezone"                              , Validator(GetAvailableTimeZone(), false) },
         { "secondminitft.enabled"                 , Validator(true) },
         { "secondminitft.type"                    , Validator(false, { "overlay", "default" }) },
         { "secondminitft.resolution"              , Validator(false, { "240p", "320p" }) },
         { "secondminitft.imagestretchenabled"     , Validator(true) },
         { "secondminitft.imageenlargeenabled"     , Validator(true) },
         { "secondminitft.imagealphaenabled"       , Validator(true) },
         { "secondminitft.imageignoreaspectenabled", Validator(true) },
         { "secondminitft.disablevideoines"        , Validator(true) },
         { "secondminitft.backlightcontrol"        , Validator(0, 100) },
         { "secondminitft.usemarquee"              , Validator(true) },
         { "secondminitft.sleepenabled"            , Validator(true) },
         { "overscan"                              , Validator(true) },
         { "externalscreen.prefered"               , Validator("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_") },
         { "externalscreen.forceresolution"        , Validator("0123456789x") },
         { "externalscreen.forcefrequency"         , Validator("0123456789.") },
       });

      return sList;
    }
    case Namespace::EmulationStation:
    {
      static HashMap<String, Validator> sList
      ({
         { "menu"                        , Validator(false, { "default", "bartop", "none" }) },
         { "selectedsystem"              , Validator(GetSupportedSystemList(), false) },
         { "bootongamelist"              , Validator(true) },
         { "hidesystemview"              , Validator(true) },
         { "gamelistonly"                , Validator(true) },
         { "forcebasicgamelistview"      , Validator(true) },
         { "filteradultgames"            , Validator(true) },
         { "clock"                       , Validator(true) },
         { "favoritesonly"               , Validator(true) },
         { "screensaver.time"            , Validator(0, 30) },
         { "screensaver.type"            , Validator(false, { "dim", "black", "demo", "gameclip" }) },
         { "showgameclipclippingitem"    , Validator(true) },
         { "showgamecliphelpitems"       , Validator(true) },
         { "theme.folder"                , Validator(GetAvailableThemes(), false) },
         { "collection.allgames"         , Validator(true) },
         { "collection.multiplayer"      , Validator(true) },
         { "collection.lastplayed"       , Validator(true) },
         { "collection.tate"             , Validator(true) },
         { "videosnaps.delay"            , Validator(0, 15000) },
         { "videosnaps.loop"             , Validator(0, 300) },
         { "virtualarcade"               , Validator(true) },
         { "virtualarcade.includeneogeo" , Validator(true) },
         { "virtualarcade.hideoriginals" , Validator(true) },
         { "collection.ports.hide"       , Validator(true) },
         { "collection.lightgun.hide"    , Validator(true) },
         { "quicksystemselect"           , Validator(true) },
         { "showhelp"                    , Validator(true) },
         { "popoup.help"                 , Validator(0, 10) },
         { "popoup.music"                , Validator(0, 10) },
         { "popoup.netplay"              , Validator(0, 10) },
         { "systemsorting"               , Validator(false, { "default", "name", "releasedate", "1type2name", "1type2releasedate", "1manufacturer2name", "1manufacturer2releasedate", "1type2manufacturer3name", "1type2manufacturer3releasedate" }) },
         { "theme.carousel"              , Validator(true) },
         { "theme.transition"            , Validator(false, { "slide", "instant", "fade" }) },
         { "brightness"                  , Validator(0, 8) },
         { "showhidden"                  , Validator(true) },
         { "showonlylatestversion"       , Validator(true) },
         { "hidenogames"                 , Validator(true) },
         { "arcade.view.hidenonworking"  , Validator(true) },
         { "arcade.view.enhanced"        , Validator(true) },
         { "arcade.view.hideclones"      , Validator(true) },
         { "arcade.view.hidebios"        , Validator(true) },
         { "arcade.view.usedatabasenames", Validator(true) },
         { "virtualarcade.manufacturers" , Validator(GetArcadeManufacturerList(), true) },
         { "pad0"                        , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopwrstuvwxyz0123456789: ") },
         { "pad1"                        , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopwrstuvwxyz0123456789: ") },
         { "pad2"                        , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopwrstuvwxyz0123456789: ") },
         { "pad3"                        , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopwrstuvwxyz0123456789: ") },
         { "pad4"                        , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopwrstuvwxyz0123456789: ") },
         { "pad5"                        , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopwrstuvwxyz0123456789: ") },
         { "pad6"                        , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopwrstuvwxyz0123456789: ") },
         { "pad7"                        , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopwrstuvwxyz0123456789: ") },
         { "pad8"                        , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopwrstuvwxyz0123456789: ") },
         { "debuglogs"                   , Validator(true) },
         { "pads.osd"                    , Validator(true) },
         { "pads.osd.type"               , Validator(GetAvailableOsdTypes(), false) },
       });

      return sList;
    }
    case Namespace::Scraper:
    {
      static HashMap<String, Validator> sList
      ({
        { "extractregionfromfilename"   , Validator(true) },
        { "getnamefrom"                 , Validator(0, 2) },
        { "source"                      , Validator(false, { "ScreenScraper", "Recalbox" }) },
        { "auto"                        , Validator(true) },
        { "screenscraper.region"        , Validator(false, { "eu", "us", "jp", "wor" }) },
        { "screenscraper.language"      , Validator(false, { "en", "es", "pt", "fr", "de", "it", "nl", "ja", "zh", "ko", "ru", "da", "fi", "sv", "hu", "no", "pl", "cz", "sk", "tr" }) },
        { "screenscraper.media"         , Validator(false, { "screenshot", "title", "logo", "marquee", "box2d", "box3d", "mixv1", "mixv2" }) },
        { "screenscraper.thumbnail"     , Validator(false, { "screenshot", "title", "logo", "marquee", "box2d", "box3d", "mixv1", "mixv2" }) },
        { "screenscraper.video"         , Validator(false, { "none", "OriginalVideo", "NormalizedVideo" }) },
        { "screenscraper.manual"        , Validator(true) },
        { "screenscraper.maps"          , Validator(true) },
        { "screenscraper.p2k"           , Validator(true) },
        { "screenscraper.user"          , Validator() },
        { "screenscraper.password"      , Validator() },
        { "screenscraper.regionPriority", Validator(false, { "DetectedRegion", "favoriteRegion" }) },
      });

      return sList;
    }
    case Namespace::Kodi:
    {
      static HashMap<String, Validator> sList
      ({
        { "enabled"         , Validator(true) },
        { "atstartup"       , Validator(true) },
        { "xbutton"         , Validator(true) },
        { "videomode"       , Validator(GetAvailableFrontEndResolutions(), false) },
        { "network.waitmode", Validator(false, { "required", "wish", "nonce" }) },
        { "network.waittime", Validator(0, INT32_MAX) },
        { "network.waithost", Validator("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-") },
      });

      return sList;
    }
    case Namespace::Hyperion:
    {
      static HashMap<String, Validator> sList
      ({
        { "enabled"         , Validator(true) },
      });

      return sList;
    }
    case Namespace::Wifi:
    {
      static HashMap<String, Validator> sList
      ({
        { "enabled"    , Validator(true) },
        { "region"     , Validator(false, { "US", "CA", "JP", "DE", "NL", "IT", "PT", "LU", "NO", "FI", "DK", "CH", "CZ", "ES", "GB", "KR", "CN", "FR", "HK", "SG", "TW", "BR", "IL", "SA", "LB", "AE", "ZA", "AR", "AU", "AT", "BO", "CL", "GR", "IS", "IN", "IE", "KW", "LI", "LT", "MX", "MA", "NZ", "PL", "PR", "SK", "SI", "TH", "UY", "PA", "RU", "KW", "LI", "LT", "MX", "MA", "NZ", "PL", "PR", "SK", "SI", "TH", "UY", "PA", "RU", "EG", "TT", "TR", "CR", "EC", "HN", "KE", "UA", "VN", "BG", "CY", "EE", "MU", "RO", "CS", "ID", "PE", "VE", "JM", "BH", "OM", "JO", "BM", "CO", "DO", "GT", "PH", "LK", "SV", "TN", "PK", "QA", "DZ" }) },
        { "ssid"       , Validator() },
        { "key"        , Validator() },
        { "ip"         , Validator("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-") },
        { "gateway"    , Validator("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-") },
        { "netmask"    , Validator("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-") },
        { "nameservers", Validator("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-") },
        { "priority"   , Validator(0, 20) },
      });

      return sList;
    }
    case Namespace::Wifi2:
    {
      static HashMap<String, Validator> sList
      ({
        { "ssid"       , Validator() },
        { "key"        , Validator() },
      });

      return sList;
    }
    case Namespace::Wifi3:
    {
      static HashMap<String, Validator> sList
      ({
        { "ssid"       , Validator() },
        { "key"        , Validator() },
      });

      return sList;
    }
    case Namespace::Audio:
    {
      static HashMap<String, Validator> sList
      ({
        { "device" , Validator(GetAvailableSoundDevices(), false) },
        { "volume" , Validator(0, 100) },
        { "music.volume", Validator(0, 100) },
        { "bgmusic", Validator(true) },
        { "mode"   , Validator(false, { "musicxorvideosound", "musicandvideosound", "musiconly", "videosoundonly", "none"}) },
      });

      return sList;
    }
    case Namespace::Controllers:
    {
      static HashMap<String, Validator> sList
      ({
        { "bluetooth.enabled"    , Validator(true) },
        { "bluetooth.ertm"       , Validator(true) },
        { "ps3.enabled"          , Validator(true) },
        { "ps3.driver"           , Validator(false, { "bluez", "official", "shanwan" }) },
        { "gpio.enabled"         , Validator(true) },
        { "gpio.args"            , Validator() },
        { "steam.enabled"        , Validator(true) },
        { "db9.enabled"          , Validator(true) },
        { "db9.args"             , Validator() },
        { "gamecon.enabled"      , Validator(true) },
        { "gamecon.args"         , Validator() },
        { "xarcade.enabled"      , Validator(true) },
        { "joycond.enabled"      , Validator(true) },
        { "swapvalidateandcancel", Validator(true) },
      });

      return sList;
    }
    case Namespace::Updates:
    {
      static HashMap<String, Validator> sList
      ({
        { "enabled", Validator(true) },
        { "type"   , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345789") },
      });

      return sList;
    }
    case Namespace::Global:
    {
      static HashMap<String, Validator> sList
      ({
        { "videomode"                 , Validator(GetAvailableGlobalResolutions(), false) },
        { "shaderset"                 , Validator({ "none", "crtcurved", "scanlines", "retro", "custom" }, false) },
        { "shaderset.file"            , Validator(GetAvailableShaders(), false) },
        { "integerscale"              , Validator(true) },
        { "shaders"                   , Validator(true) },
        { "ratio"                     , Validator(GetSupportedRatioList(), false) },
        { "smooth"                    , Validator(true) },
        { "rewind"                    , Validator(true) },
        { "autosave"                  , Validator(true) },
        { "retroachievements"         , Validator(true) },
        { "retroachievements.hardcore", Validator(true) },
        { "retroachievements.username", Validator() },
        { "retroachievements.password", Validator() },
        { "inputdriver"               , Validator(false, { "auto", "udev", "sdl2"}) },
        { "configfile"                , Validator() },
        { "demo.systemlist"           , Validator(GetSupportedSystemList(), true) },
        { "demo.duration"             , Validator(30, 600) },
        { "demo.infoscreenduration"   , Validator(5, 30) },
        { "translate"                 , Validator(true) },
        { "translate.from"            , Validator(false, {"auto", "EN", "ES", "FR", "IT", "DE", "JP", "NL", "CS", "DA", "SV", "HR", "KO", "ZH_CN", "ZH_TW", "CA", "BG", "BN", "EU", "AZ", "AR", "SQ", "AF", "EO", "ET", "TL", "FI", "GL", "KA", "EL", "GU", "HT", "IW", "HI", "HU", "IS", "ID", "GA", "KN", "LA", "LV", "LT", "MK", "MS", "MT", "NO", "FA", "PL", "PT", "RO", "RU", "SR", "SK", "SL", "SW", "TA", "TE", "TH", "TR", "UK", "UR", "VI", "CY", "YI"}) },
        { "translate.to"              , Validator(false, {"auto", "EN", "ES", "FR", "IT", "DE", "JP", "NL", "CS", "DA", "SV", "HR", "KO", "ZH_CN", "ZH_TW", "CA", "BG", "BN", "EU", "AZ", "AR", "SQ", "AF", "EO", "ET", "TL", "FI", "GL", "KA", "EL", "GU", "HT", "IW", "HI", "HU", "IS", "ID", "GA", "KN", "LA", "LV", "LT", "MK", "MS", "MT", "NO", "FA", "PL", "PT", "RO", "RU", "SR", "SK", "SL", "SW", "TA", "TE", "TH", "TR", "UK", "UR", "VI", "CY", "YI"}) },
        { "translate.apikey"          , Validator() },
        { "translate.url"             , Validator() },
        { "netplay"                   , Validator(true) },
        { "netplay.active"            , Validator(true) },
        { "netplay.nickname"          , Validator("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'_-") },
        { "netplay.port"              , Validator(1, 65535) },
        { "netplay.relay"             , Validator(false, { "none", "nyc", "madrid", "montreal", "saopaulo" }) },
        { "netplay.lobby"             , Validator() },
        { "recalboxoverlays"          , Validator(true) },
        { "softpatching"              , Validator(false, { "auto", "select", "disable" }) },
        { "showfps"                   , Validator(true) },
        { "hidepreinstalledgames"     , Validator(true) },
        { "show.savestate.before.run" , Validator(true) },
        { "quitpresstwice"            , Validator(true) },
      });

      return sList;
    }
    case Namespace::Specific:
    {
      static HashMap<String, Validator> sList
        ({
           { "videomode"                 , Validator(GetAvailableFrontEndResolutions(), false) },
           { "shaderset"                 , Validator({ "none", "crtcurved", "scanlines", "retro", "custom" }, false) },
           { "shaderset.file"            , Validator(GetAvailableShaders(), false) },
           { "integerscale"              , Validator(true) },
           { "shaders"                   , Validator(true) },
           { "ratio"                     , Validator(GetSupportedRatioList(), false) },
           { "smooth"                    , Validator(true) },
           { "rewind"                    , Validator(true) },
           { "autosave"                  , Validator(true) },
           { "retroachievements"         , Validator(true) },
           { "retroachievements.hardcore", Validator(true) },
           { "retroachievements.username", Validator() },
           { "retroachievements.password", Validator() },
           { "inputdriver"               , Validator(false, { "auto", "udev", "sdl2"}) },
           { "configfile"                , Validator("abcdefghijklmnopqrstuvwxyz./-_") }, //<! Path to file for specific config or "dummy" to cancel any config from configgen
           { "translate"                 , Validator(true) },
           { "translate.from"            , Validator(false, {"auto", "EN", "ES", "FR", "IT", "DE", "JP", "NL", "CS", "DA", "SV", "HR", "KO", "ZH_CN", "ZH_TW", "CA", "BG", "BN", "EU", "AZ", "AR", "SQ", "AF", "EO", "ET", "TL", "FI", "GL", "KA", "EL", "GU", "HT", "IW", "HI", "HU", "IS", "ID", "GA", "KN", "LA", "LV", "LT", "MK", "MS", "MT", "NO", "FA", "PL", "PT", "RO", "RU", "SR", "SK", "SL", "SW", "TA", "TE", "TH", "TR", "UK", "UR", "VI", "CY", "YI"}) },
           { "translate.to"              , Validator(false, {"auto", "EN", "ES", "FR", "IT", "DE", "JP", "NL", "CS", "DA", "SV", "HR", "KO", "ZH_CN", "ZH_TW", "CA", "BG", "BN", "EU", "AZ", "AR", "SQ", "AF", "EO", "ET", "TL", "FI", "GL", "KA", "EL", "GU", "HT", "IW", "HI", "HU", "IS", "ID", "GA", "KN", "LA", "LV", "LT", "MK", "MS", "MT", "NO", "FA", "PL", "PT", "RO", "RU", "SR", "SK", "SL", "SW", "TA", "TE", "TH", "TR", "UK", "UR", "VI", "CY", "YI"}) },
           { "translate.apikey"          , Validator() },
           { "translate.url"             , Validator() },
           { "supergameboy"              , Validator(false, { "gb", "sgb", "ask" }) }, //<! For GB only
           { "ignore"                    , Validator(true) }, //<! To not show a system in systems list
           { "realgamecubepads"          , Validator(true) }, //<! For GameCube only, play w/ gc pads w/ adapters
           { "realwiimotes"              , Validator(true) }, //<! For Wii only, use real wiimotes in Dolphin
           { "emulatedwiimotes"          , Validator(true) }, //<! For Wii only, emulate wiimotes w/ std pads
           { "sensorbar.position"        , Validator(true) }, //<! For Wii only, specify Dolphin Bar position around screen. 1: top, 0: bottom
         });

      return sList;
    }
    case Namespace::Patron:
    {
      static HashMap<String, Validator> sList
        ({
          { "privatekey", Validator("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-") },
        });

      return sList;
    }
    case Namespace::Music:
    {
      static HashMap<String, Validator> sList
        ({
          { "remoteplaylist.enable", Validator(true) },
        });

      return sList;
    }
    case Namespace::Hat:
    {
      static HashMap<String, Validator> sList
        ({
          { "wpaf.enabled", Validator(true) },
          { "wpaf.board"  , Validator(false, { "wspoehatb", "argonforty", "piboy", "rpipoeplus", "fanshim" }) }
        });

      return sList;
    }
    case Namespace::Unknown:
    default: break;
  }

  static HashMap<String, Validator> sList;
  return sList;
}

IniFile RequestHandlerTools::LoadConfiguration()
{
  return IniFile(Path(sConfiguration), false);
}

void RequestHandlerTools::SaveConfiguration(IniFile& configuration)
{
  configuration.Save();
}

void RequestHandlerTools::GetKeyValues(const String& domain, const HashMap<String, Validator>& keys, Http::ResponseWriter& response)
{
  IniFile configuration = RequestHandlerTools::LoadConfiguration();
  JSONBuilder result;
  result.Open();
  for(const auto& key : keys)
  {
    String k(domain); k.Append('.'); k.Append(key.first);
    JSONBuilder value;
    value.Open()
         .Field("exist", configuration.Exists(k));
    switch (key.second.Type())
    {
      case Validator::Types::StringFree:
      case Validator::Types::StringConstrained:
      case Validator::Types::StringPicker: value.Field("value", configuration.AsString(k)); break; // FIX 21/04/2023
      case Validator::Types::StringMultiPicker: value.Field("value", configuration.AsStringList(k)); break;
      case Validator::Types::IntRange: value.Field("value", configuration.AsInt(k, 0)); break;
      case Validator::Types::Bool: value.Field("value", configuration.AsBool(k, false)); break;
      default: LOG(LogError) << "Unknown type " << (int) key.second.Type() << " for " << key.first;
    }
    value.Close();
    result.Field(key.first.c_str(), value);
  }
  result.Close();
  RequestHandlerTools::Send(response, Http::Code::Ok, result, Mime::Json);
}

void RequestHandlerTools::GetKeyValueOptions(const HashMap<String, Validator>& keys, Http::ResponseWriter& response)
{
  IniFile configuration = RequestHandlerTools::LoadConfiguration();
  JSONBuilder result;
  result.Open();
  for(const auto& key : keys)
  {
    JSONBuilder properties;
    properties.Open()
              .Field("type", key.second.TypeAsString());
    switch (key.second.Type())
    {
      case Validator::Types::StringFree: break;
      case Validator::Types::StringConstrained: properties.Field("allowedChars", key.second.StringConstraint()); break;
      case Validator::Types::StringPicker:
      case Validator::Types::StringMultiPicker: properties.Field("allowedStringList", key.second.StringList()); break;
      case Validator::Types::IntRange: properties.Field("lowerValue", key.second.Lower()).Field("higherValue", key.second.Higher()); break;
      case Validator::Types::Bool: break;
      default: LOG(LogError) << "Unknown type " << (int) key.second.Type() << " for " << key.first;
    }
    if (key.second.HasDisplay())
      properties.Field("displayableStringList", key.second.DisplayList());
    properties.Close();
    result.Field(key.first.c_str(), properties);
  }
  result.Close();
  RequestHandlerTools::Send(response, Http::Code::Ok, result, Mime::Json);
}

void RequestHandlerTools::SetKeyValues(const String& domain, const HashMap<String, Validator>& keys, const String& jsonString, Http::ResponseWriter& response)
{
  rapidjson::Document json;
  json.Parse(jsonString.c_str());
  if (!json.HasParseError())
  {
    IniFile configuration = RequestHandlerTools::LoadConfiguration();
    bool changed = false;
    if (json.GetType() == rapidjson::Type::kObjectType)
      for(const auto& key : json.GetObject())
      {
        String shortKey(key.name.GetString());
        String value;
        switch(key.value.GetType())
        {
          case rapidjson::kFalseType: value = "0"; break;
          case rapidjson::kTrueType: value = "1"; break;
          case rapidjson::kStringType: value = key.value.GetString(); break;
          case rapidjson::kNumberType: value = String(key.value.GetInt()); break;

          case rapidjson::kArrayType:
          {
              auto myArray = key.value.GetArray();
              value = "";
              for (unsigned int i = 0; i < myArray.Size(); i++)
              {
                  const auto& val = myArray[i];
                  if (val.IsString())
                  {
                      value +=  String(val.GetString()) + ',';
                  }
              }
              if (!myArray.Empty())
              {
                  value.pop_back();
              }

              break;
          }

          case rapidjson::kNullType:
          case rapidjson::kObjectType:
          {
            RequestHandlerTools::Send(response, Http::Code::Bad_Request, "Key '" + shortKey + "' has an invalid value!", Mime::PlainText);
            return;
          }
        }
        Validator* validator = keys.try_get(shortKey);
        if (validator != nullptr)
        {
          if (validator->Validate(value))
          {
            String longKey(domain); longKey.Append('.'); longKey.Append(shortKey);
            configuration.SetString(longKey, value);
            changed = true;
          }
          else
          {
            RequestHandlerTools::Send(response, Http::Code::Bad_Request, "Key '" + shortKey + "' has an invalid value!", Mime::PlainText);
            return;
          }
        }
        else
        {
          RequestHandlerTools::Send(response, Http::Code::Bad_Request, "Unknown Key: " + shortKey, Mime::PlainText);
          return;
        }
      }
    if (changed)
      RequestHandlerTools::SaveConfiguration(configuration);
    GetKeyValues(domain, keys, response);
    return;
  }
  RequestHandlerTools::Send(response, Http::Code::Bad_Request, "JSON Parsing error", Mime::PlainText);
}

void RequestHandlerTools::DeleteKeyValues(const String& domain, const HashMap<String, Validator>& keys, const String& jsonString, Http::ResponseWriter& response)
{
  rapidjson::Document json;
  json.Parse(jsonString.c_str());
  if (!json.HasParseError())
  {
    IniFile configuration = RequestHandlerTools::LoadConfiguration();
    bool changed = false;
    if (json.GetType() == rapidjson::Type::kArrayType)
      for(const auto& key : json.GetArray())
      {
        String shortKey(key.GetString());
        if (keys.contains(shortKey))
        {
          String longKey(domain); longKey.Append('.'); longKey.Append(shortKey);
          configuration.Delete(longKey);
          changed = true;
        }
        else
        {
          RequestHandlerTools::Send(response, Http::Code::Bad_Request, "Unknown Key: " + shortKey, Mime::PlainText);
          return;
        }
      }
    if (changed)
      RequestHandlerTools::SaveConfiguration(configuration);
    GetKeyValues(domain, keys, response);
    return;
  }
  RequestHandlerTools::Send(response, Http::Code::Bad_Request, "JSON Parsing error", Mime::PlainText);
}

const String::List& RequestHandlerTools::GetSupportedSystemList()
{
  static String::List result;

  if (result.empty())
  {
    SystemDeserializer deserializer;
    deserializer.LoadSystems();
    for(int i = deserializer.Count(); --i >= 0; )
    {
      SystemDescriptor descriptor;
      deserializer.Deserialize(i, descriptor);
      result.push_back(descriptor.Name());
    }
    // PATCH
    result.push_back("favorites");
  }

  return result;
}

HashMap<String, String> RequestHandlerTools::GetArcadeManufacturerList()
{
  static HashMap<String, String> result;

  if (result.empty())
  {
    for(const String& rawIdentifier : ArcadeVirtualSystems::GetVirtualArcadeSystemList())
    {
      String identifier(SystemManager::sArcadeManufacturerPrefix);
      identifier.Append(rawIdentifier).Replace('\\', '-');
      result.insert_unique({ identifier, String(rawIdentifier).Replace('\\', " - ") });
    }
  }

  return result;
}

const String::List& RequestHandlerTools::GetSupportedRatioList()
{
  static String::List result;

  if (result.empty())
    for(const auto& ratio : LibretroRatio::GetRatio())
      result.push_back(ratio.second);

  return result;
}

const String& RequestHandlerTools::GetArchitecture()
{
  static String sArchitecture(Files::LoadFile(Path("/recalbox/recalbox.arch")));
  return sArchitecture;
}

String RequestHandlerTools::GetCommandOutput(const String& command)
{
  String output;
  char buffer[10240];
  FILE* pipe = popen(command.c_str(), "r");
  if (pipe != nullptr)
  {
    while (feof(pipe) == 0)
      if (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        output.Append(buffer);
    pclose(pipe);
  }
  return output;
}

HashMap<String, String> RequestHandlerTools::GetAvailableGlobalResolutions()
{
  static HashMap<String, String> sResolutions;
  ResolutionAdapter resolutionAdapter;

  if (sResolutions.empty())
  {
    sResolutions.insert_unique("default", "NATIVE");
    for(const ResolutionAdapter::Resolution& resolution : resolutionAdapter.Resolutions(true))
    {
      String reso = resolution.ToRawString();
      sResolutions.insert_unique({ reso, resolution.ToString() });
    }
  }

  return sResolutions;
}

HashMap<String, String> RequestHandlerTools::GetAvailableFrontEndResolutions()
{
  static HashMap<String, String> sResolutions;
  ResolutionAdapter resolutionAdapter;

  if (sResolutions.empty())
  {
    sResolutions.insert_unique("", "USE GLOBAL");
    sResolutions.insert_unique("default", "NATIVE");
    for(const ResolutionAdapter::Resolution& resolution : resolutionAdapter.Resolutions(true))
    {
      String reso = resolution.ToRawString();
      sResolutions.insert_unique({ reso, resolution.ToString() });
    }
  }

  return sResolutions;
}

HashMap<String, String> RequestHandlerTools::GetAvailableSoundDevices()
{
  static HashMap<String, String> result;

  if (result.empty())
  {
    IAudioController::DeviceList playbackList = AudioController::Instance().GetPlaybackList();

    for(const auto& playback : playbackList)
    {
      result.insert_unique({ playback.InternalName, playback.DisplayableName });
    }
  }

  return result;
}

const String::List& RequestHandlerTools::GetAvailableTimeZone()
{
  static String::List output;

  if (output.empty())
  {
    String zones = Files::LoadFile(Path("/usr/share/zoneinfo/zone.tab"));
    String::List list = zones.Split('\n');
    for (const String& line : list)
      if (!line.empty() && line[0] != '#')
      {
        int start = line.Find('\t');
        if (start < 0) continue;
        start = line.Find('\t', start + 1);
        if (start < 0) continue;
        int stop = line.Find('\t', ++start);
        String tz = line.SubString(start, stop - start);
        output.push_back(tz);
      }
  }

  return output;
}

HashMap<String, String> RequestHandlerTools::GetAvailableOsdTypes()
{
  static HashMap<String, String> sOSDs
  ({
    {"snes", "Super Nintendo"},
    {"nintendo64", "Nintendo 64"},
    {"megadrive", "Megadrive"},
    {"dreamcast", "Dreamcast"},
    {"playstation", "Playstation"},
    {"xbox", "Xbox"},
  });

  return sOSDs;
}

HashMap<String, String> RequestHandlerTools::GetAvailableLanguages()
{
  static HashMap<String, String> sLanguages
  ({
     { "eu_ES", "EUSKARA"       },
     { "zh_TW", "正體中文"        },
     { "zh_CN", "简体中文"        },
     { "de_DE", "DEUTSCH"       },
     { "en_US", "ENGLISH"       },
     { "es_ES", "ESPAÑOL"       },
     { "fr_FR", "FRANÇAIS"      },
     { "it_IT", "ITALIANO"      },
     { "pt_BR", "PORTUGUES"     },
     { "sv_SE", "SVENSKA"       },
     { "tr_TR", "TÜRKÇE"        },
     { "ca_ES", "CATALÀ"        },
     { "ar_YE", "اللغة العربية" },
     { "nl_NL", "NEDERLANDS"    },
     { "el_GR", "ελληνικά"      },
     { "ko_KR", "한국어"         },
     { "nn_NO", "NORSK"         },
     { "nb_NO", "BOKMAL"        },
     { "pl_PL", "POLSKI"        },
     { "ja_JP", "日本語"         },
     { "ru_RU", "Русский"       },
     { "hu_HU", "MAGYAR"        },
     { "cs_CZ", "čeština"       },
     { "lv_LV", "latviešu"      },
     { "lb_LU", "Lëtzebuergesch"},
  });

  return sLanguages;
}

HashMap<String, String> RequestHandlerTools::GetAvailableKeyboardLayout()
{
  static HashMap<String, String> kbLayouts;

  if (kbLayouts.empty())
  {
    static Path layouts("/usr/share/keymaps/i386");
    static String ext(".map.gz");
    for (const char* sub : { "azerty", "bepo", "qwerty", "qwertz" })
      for(const Path& filePath : (layouts / sub).GetDirectoryContent())
      {
        String fileName = filePath.Filename();
        if (fileName.EndsWith(ext))
        {
          fileName = fileName.SubString(0, fileName.Count() - ext.Count());
          kbLayouts.insert_unique(String(fileName), String(sub) + ":" + fileName);
        }
      }
  }

  return kbLayouts;
}

void RequestHandlerTools::GetAvailableShadersIn(const Path& rootPath, String::List& results)
{
  Path::PathList pathList = rootPath.GetDirectoryContent();
  for(const Path& path : pathList)
    if (path.IsDirectory())
      GetAvailableShadersIn(path, results);
    else if (path.IsFile())
      if (path.Extension() == ".glslp")
        results.push_back(path.ToString());
}

String::List RequestHandlerTools::GetAvailableShaders()
{
  String::List result;
  static Path shaderPath("/recalbox/share/shaders");

  GetAvailableShadersIn(shaderPath, result);
  int commonPartLength = (int)shaderPath.ToString().length() + 1;
  for(String& path : result)
    path.erase(0, commonPartLength);

  return result;
}

String::List RequestHandlerTools::GetAvailableThemes()
{
  auto themeSets = ThemeData::getThemeSets();
  String::List result;
  for (const auto& theme : themeSets)
  {
    result.push_back(theme.first);
  }

  return result;
}

void RequestHandlerTools::GetEmbeddedBios(const Path& base, HashMap<String, bool>& results)
{
  // Read all embedded path. Bas must be /recalbox/share/ as bios is already in path of every line
  String::List pathlist = Files::LoadFile(Path("/recalbox/share_init/embedded.list")).Split('\n');
  results.clear();
  for(const String& path : pathlist)
    results.insert_unique(String((base / path).ToString()), true);
}

void RequestHandlerTools::GetAvailableBios(const Path& rootPath, Path::PathList& results, const HashMap<String, bool>& embedded)
{
  Path::PathList pathList = rootPath.GetDirectoryContent();
  for(const Path& path : pathList)
    if (path.IsDirectory())
      GetAvailableBios(path, results, embedded);
    else if (path.IsFile())
      if (!embedded.contains(path.ToString()))
        results.push_back(path);
}

Path RequestHandlerTools::GetCompressedBiosFolder()
{
  Path baseFolder("/recalbox/share/bios");

  // Get embedded list
  HashMap<String, bool> embedded;
  GetEmbeddedBios(baseFolder.Directory(), embedded);

  // Get diff list embedded/user bios
  Path::PathList biosPath;
  GetAvailableBios(baseFolder, biosPath, embedded);

  Path destination("/recalbox/share/system/bios_backup.zip");
  {
    Zip zip(destination, true);
    for(const Path& path : biosPath)
      zip.Add(path, baseFolder);
  }

  return destination;
}

bool RequestHandlerTools::IsValidSystem(const String& system)
{
  String::List systemList = RequestHandlerTools::GetSupportedSystemList();
  for(const String& s : systemList)
    if (s == system)
      return true;

  return false;
}

bool
RequestHandlerTools::GetUploadedFile(const Rest::Request& request, String& filename, int& startOffset, int& size)
{
  #define GUF_DOUBLE_EOF "\r\n\r\n"
  #define GUF_BOUNDARY "boundary="
  #define GUF_FILENAME "filename=\""

  // Get barrier
  const auto* contentType = request.headers().tryGet<Http::Header::ContentType>().get();
  if (contentType == nullptr) return false;
  String boundary = contentType->mime().raw();
  size_t start = boundary.find(GUF_BOUNDARY);
  if (start == String::npos) return false;
  boundary.erase(0, start + (sizeof(GUF_BOUNDARY) - 1));
  boundary.Insert(0, "--", 2);

  LOG(LogError) << "BOUNDARY: " << boundary;

  // Lookup filename
  start = request.body().find(boundary, 0);
  if (start != String::npos)
    for(;;)
    {
      // Find start/stop
      size_t end = request.body().find(boundary, start + boundary.size());
      if (end == String::npos) break;

      // Get EOL
      size_t headerStart = request.body().find('\n', start + boundary.size());
      if (headerStart != String::npos && headerStart < end)
      {
        // Get double EOL
        size_t headerStop = request.body().find(GUF_DOUBLE_EOF, ++headerStart);
        if (headerStop != String::npos && headerStop < end)
        {
          // locate filename
          size_t fileNameStart = request.body().find(GUF_FILENAME, headerStart);
          if (headerStart != String::npos && headerStart < headerStop)
          {
            fileNameStart += (sizeof(GUF_FILENAME) - 1);
            size_t fileNameStop = request.body().find('"', fileNameStart);
            if (fileNameStop != String::npos && fileNameStop < headerStop)
            {
              // Extract filename
              filename = request.body().substr(fileNameStart, fileNameStop - fileNameStart);
              // Store indexes
              startOffset = (int)(headerStop + sizeof(GUF_DOUBLE_EOF) - 1);
              size = (int)(end - startOffset) - 2 /* size of last EOF */;
              return true;
            }
          }
        }
      }
      // Restart from next boundary
      start = end;
    }

  #undef GUF_FILENAME
  #undef GUF_BOUNDARY
  #undef GUF_DOUBLE_EOF

  return false;
}

bool RequestHandlerTools::ExtractArray(const Rest::Request& request, String::List& result)
{
  rapidjson::Document json;
  json.Parse(request.body().c_str());
  if (!json.HasParseError())
    if (json.GetType() == rapidjson::Type::kArrayType)
    {
      for (const auto& key : json.GetArray())
        result.push_back(key.GetString());
      return true;
    }

  return false;
}

