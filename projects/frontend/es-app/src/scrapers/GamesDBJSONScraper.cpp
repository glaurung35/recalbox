#include <exception>
#include <map>

#include "scrapers/GamesDBJSONScraper.h"
#include "scrapers/GamesDBJSONScraperResources.h"

#include "FileData.h"
#include "Log.h"
#include "PlatformId.h"
#include "Settings.h"
#include "SystemData.h"
#include <pugixml/pugixml.hpp>

/* When raspbian will get an up to date version of rapidjson we'll be
   able to have it throw in case of error with the following:
#ifndef RAPIDJSON_ASSERT
#define RAPIDJSON_ASSERT(x)                                                    \
  if (!(x)) {                                                                  \
	throw std::runtime_error("rapidjson internal assertion failure: " #x);     \
  }
#endif // RAPIDJSON_ASSERT
*/

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

using namespace PlatformIds;
using namespace rapidjson;

namespace
{
  TheGamesDBJSONRequestResources resources;
}

const std::map<PlatformId, std::string> gamesdb_new_platformid_map {{AMSTRAD_CPC,               "4914"},
                                                                    {APPLE_II,                  "4942"},
                                                                    {APPLE_MACOS,               "37"},
                                                                    {ARCADE,                    "23"},
                                                                    {ATARI_8BITS,               "4943"},
                                                                    {ATARI_2600,                "22"},
                                                                    {ATARI_5200,                "26"},
                                                                    {ATARI_7800,                "27"},
                                                                    {ATARI_LYNX,                "4924"},
                                                                    {ATARI_ST,                  "4937"},
                                                                    {ATARI_JAGUAR,              "28"},
                                                                    {ATARI_JAGUAR_CD,           "29"},
                                                                    {BANDAI_WONDERSWAN,         "4925"},
                                                                    {BANDAI_WONDERSWAN_COLOR,   "4926"},
                                                                    {COLECOVISION,              "31"},
                                                                    {COMMODORE_64,              "40"},
                                                                    {COMMODORE_AMIGA,           "4911"},
                                                                    {FAIRCHILD_CHANNELF,        "4928"},
  //{ GAMEENGINE_DAPHNE,         },
  //{ GAMEENGINE_LUTRO,          },
  //{ GAMEENGINE_OPENBOR,        },
  //{ GAMEENGINE_SCUMMVM,        },
                                                                    {GCE_VECTREX,               "4939"},
                                                                    {IBM_PC,                    "1"},
                                                                    {MAGNAVOX_ODYSSEY2,         "4927"},
                                                                    {MATTEL_INTELLIVISION,      "32"},
                                                                    {MICROSOFT_XBOX,            "14"},
                                                                    {MICROSOFT_XBOX_360,        "15"},
                                                                    {MICROSOFT_MSX,             "4929"},
                                                                    {MICROSOFT_MSX1,            "4929"},
                                                                    {MICROSOFT_MSX2,            "4929"},
  //{ NEC_PCENGINE,              },
  //{ NEC_PCENGINE_CD,           },
                                                                    {NEC_SUPERGRAFX,            "34"},
                                                                    {NEOGEO,                    "24"},
                                                                    {NEOGEO_CD,                 "24"},
                                                                    {NEOGEO_POCKET,             "4922"},
                                                                    {NEOGEO_POCKET_COLOR,       "4923"},
                                                                    {NINTENDO_3DS,              "4912"},
                                                                    {NINTENDO_64,               "3"},
                                                                    {NINTENDO_DS,               "8"},
                                                                    {NINTENDO_FDS,              "4936"},
  //{ NINTENDO_GAME_AND_WATCH,   },
                                                                    {NINTENDO_GAMEBOY,          "4"},
                                                                    {NINTENDO_GAMEBOY_ADVANCE,  "5"},
                                                                    {NINTENDO_GAMEBOY_COLOR,    "41"},
                                                                    {NINTENDO_GAMECUBE,         "2"},
                                                                    {NINTENDO_NES,              "7"},
                                                                    {NINTENDO_POKEMINI,         "4957"},
                                                                    {NINTENDO_SATELLAVIEW,      "6"}, // No specific platform
                                                                    {NINTENDO_SNES,             "6"},
                                                                    {NINTENDO_SUFAMITURBO,      "6"}, // No specific platform
                                                                    {NINTENDO_VIRTUAL_BOY,      "4918"},
                                                                    {NINTENDO_WII,              "9"},
                                                                    {NINTENDO_WII_U,            "38"},
  //{ OSH_UZEBOX,                },
                                                                    {PANASONIC_3DO,             "25"},
                                                                    {PORT_PRBOOM,               "1"},
                                                                    {PORT_CAVE_STORY,           "1"},
                                                                    {SEGA_32X,                  "33"},
                                                                    {SEGA_CD,                   "21"},
                                                                    {SEGA_DREAMCAST,            "16"},
                                                                    {SEGA_GAME_GEAR,            "20"},
                                                                    {SEGA_GENESIS,              "18"},
                                                                    {SEGA_MASTER_SYSTEM,        "35"},
                                                                    {SEGA_MEGA_DRIVE,           "36"},
                                                                    {SEGA_SATURN,               "17"},
                                                                    {SEGA_SG1000,               "4949"},
                                                                    {SHARP_X68000,              "4931"},
                                                                    {SONY_PLAYSTATION,          "10"},
                                                                    {SONY_PLAYSTATION_2,        "11"},
                                                                    {SONY_PLAYSTATION_3,        "12"},
                                                                    {SONY_PLAYSTATION_4,        "4919"},
                                                                    {SONY_PLAYSTATION_VITA,     "39"},
                                                                    {SONY_PLAYSTATION_PORTABLE, "13"},
                                                                    {SINCLAIR_ZX_SPECTRUM,      "4913"},
  //{ SINCLAIR_ZX_81,            },
  //{ STREAM_MOONLIGHT,          },
  //{ TANDERINE_ORICATMOS,       },
  //{ THOMSON_MOTO,              },

  //{ TRS80_COLOR_COMPUTER, "4941" },
  //{ TANDY, "4941" },
};

void thegamesdb_generate_json_scraper_requests(const ScraperSearchParams& params,
                                               std::queue<std::unique_ptr<ScraperRequest>>& requests,
                                               std::vector<ScraperSearchResult>& results)
{
  resources.prepare();
  std::string path = "https://api.thegamesdb.net";
  bool usingGameID = false;
  const std::string apiKey = std::string("apikey=") + resources.getApiKey();
  std::string cleanName = params.nameOverride;
  if (!cleanName.empty() && cleanName.substr(0, 3) == "id:")
  {
    std::string gameID = cleanName.substr(3);
    path += "/Games/ByGameID?" + apiKey + "&fields=players,publishers,genres,overview,last_updated,rating,"
                                          "platform,coop,youtube,os,processor,ram,hdd,video,sound,alternates&"
                                          "include=boxart&id=" + HttpReq::urlEncode(gameID);
    usingGameID = true;
  }
  else
  {
    if (cleanName.empty())
      cleanName = params.game->getCleanName();
    path += "/Games/ByGameName?" + apiKey + "&fields=players,publishers,genres,overview,last_updated,rating,"
                                            "platform,coop,youtube,os,processor,ram,hdd,video,sound,alternates&"
                                            "include=boxart&name=" + HttpReq::urlEncode(cleanName);
  }

  if (usingGameID)
  {
    // if we have the ID already, we don't need the GetGameList request
    requests.push(std::unique_ptr<ScraperRequest>(new TheGamesDBJSONRequest(results, path)));
  }
  else
  {
    std::string platformQueryParam;
    auto& platforms = params.system->getPlatformIds();
    if (!platforms.empty())
    {
      bool first = true;
      platformQueryParam += "&filter%5Bplatform%5D=";
      for (auto platform : platforms)
      {
        auto mapIt = gamesdb_new_platformid_map.find(platform);
        if (mapIt != gamesdb_new_platformid_map.cend())
        {
          if (!first)
          {
            platformQueryParam += ",";
          }
          platformQueryParam += HttpReq::urlEncode(mapIt->second);
          first = false;
        }
        else
        {
          LOG(LogWarning) << "TheGamesDB scraper warning - no support for platform " << getPlatformName(platform);
        }
      }
      path += platformQueryParam;
    }

    requests.push(std::unique_ptr<ScraperRequest>(new TheGamesDBJSONRequest(requests, results, path)));
  }
}

namespace
{

  std::string getStringOrThrow(const Value& v, const std::string& key)
  {
    if (!v.HasMember(key.c_str()) || !v[key.c_str()].IsString())
    {
      throw std::runtime_error("rapidjson internal assertion failure: missing or non string key:" + key);
    }
    return v[key.c_str()].GetString();
  }

  int getIntOrThrow(const Value& v, const std::string& key)
  {
    if (!v.HasMember(key.c_str()) || !v[key.c_str()].IsInt())
    {
      throw std::runtime_error("rapidjson internal assertion failure: missing or non int key:" + key);
    }
    return v[key.c_str()].GetInt();
  }

  int getIntOrThrow(const Value& v)
  {
    if (!v.IsInt())
    {
      throw std::runtime_error("rapidjson internal assertion failure: not an int");
    }
    return v.GetInt();
  }

  std::string getBoxartImage(const Value& v)
  {
    if (!v.IsArray() || v.Size() == 0)
    {
      return "";
    }
    for (int i = 0; i < (int) v.Size(); ++i)
    {
      auto& im = v[i];
      std::string type = getStringOrThrow(im, "type");
      std::string side = getStringOrThrow(im, "side");
      if (type == "boxart" && side == "front")
      {
        return getStringOrThrow(im, "filename");
      }
    }
    return getStringOrThrow(v[0], "filename");
  }

  std::string getDeveloperString(const Value& v)
  {
    if (!v.IsArray())
    {
      return "";
    }
    std::string out;
    bool first = true;
    for (int i = 0; i < (int) v.Size(); ++i)
    {
      auto mapIt = resources.gamesdb_new_developers_map.find(getIntOrThrow(v[i]));
      if (mapIt == resources.gamesdb_new_developers_map.cend())
      {
        continue;
      }
      if (!first)
      {
        out += ", ";
      }
      out += mapIt->second;
      first = false;
    }
    return out;
  }

  std::string getPublisherString(const Value& v)
  {
    if (!v.IsArray())
    {
      return "";
    }
    std::string out;
    bool first = true;
    for (int i = 0; i < (int) v.Size(); ++i)
    {
      auto mapIt = resources.gamesdb_new_publishers_map.find(getIntOrThrow(v[i]));
      if (mapIt == resources.gamesdb_new_publishers_map.cend())
      {
        continue;
      }
      if (!first)
      {
        out += ", ";
      }
      out += mapIt->second;
      first = false;
    }
    return out;
  }

  std::string getGenreString(const Value& v)
  {
    if (!v.IsArray())
    {
      return "";
    }
    std::string out;
    bool first = true;
    for (int i = 0; i < (int) v.Size(); ++i)
    {
      auto mapIt = resources.gamesdb_new_genres_map.find(getIntOrThrow(v[i]));
      if (mapIt == resources.gamesdb_new_genres_map.cend())
      {
        continue;
      }
      if (!first)
      {
        out += ", ";
      }
      out += mapIt->second;
      first = false;
    }
    return out;
  }

  void processGame(const Value& game, const Value& boxart, std::vector<ScraperSearchResult>& results)
  {
    std::string baseImageUrlThumb = getStringOrThrow(boxart["base_url"], "thumb");
    std::string baseImageUrlLarge = getStringOrThrow(boxart["base_url"], "large");

    ScraperSearchResult result;

    result.mdl.SetName(getStringOrThrow(game, "game_title"));
    if (game.HasMember("overview") && game["overview"].IsString())
    {
      result.mdl.SetDescription(game["overview"].GetString());
    }
    if (game.HasMember("release_date") && game["release_date"].IsString())
    {
      DateTime dt;
      if (DateTime::ParseFromString("%yyyy-%MM-%dd", game["release_date"].GetString(), dt))
        result.mdl.SetReleaseDate(dt);
    }
    if (game.HasMember("developers") && game["developers"].IsArray())
    {
      result.mdl.SetDeveloper(getDeveloperString(game["developers"]));
    }
    if (game.HasMember("publishers") && game["publishers"].IsArray())
    {
      result.mdl.SetPublisher(getPublisherString(game["publishers"]));
    }
    if (game.HasMember("genres") && game["genres"].IsArray())
    {
      result.mdl.SetGenre(getGenreString(game["genres"]));
    }
    if (game.HasMember("players") && game["players"].IsInt())
    {
      int p = game["players"].GetInt();
      result.mdl.SetPlayers(p, p);
    }

    if (boxart.HasMember("data") && boxart["data"].IsObject())
    {
      std::string id = std::to_string(getIntOrThrow(game, "id"));
      if (boxart["data"].HasMember(id.c_str()))
      {
        std::string image = getBoxartImage(boxart["data"][id.c_str()]);
        result.thumbnailUrl = baseImageUrlThumb + "/" + image;
        result.imageUrl = baseImageUrlLarge + "/" + image;
      }
    }

    results.push_back(result);
  }
} // namespace

void TheGamesDBJSONRequest::process(const std::unique_ptr<HttpReq>& req, std::vector<ScraperSearchResult>& results)
{
  assert(req->status() == HttpReq::REQ_SUCCESS);

  Document doc;
  doc.Parse(req->getContent().c_str());

  if (doc.HasParseError())
  {
    std::string err = std::string("TheGamesDBJSONRequest - Error parsing JSON. \n\t") +
                      GetParseError_En(doc.GetParseError());
    setError(err);
    LOG(LogError) << err;
    return;
  }

  if (!doc.HasMember("data") || !doc["data"].HasMember("games") || !doc["data"]["games"].IsArray())
  {
    std::string warn = "TheGamesDBJSONRequest - Response had no game data.\n";
    LOG(LogWarning) << warn;
    return;
  }
  const Value& games = doc["data"]["games"];

  if (!doc.HasMember("include") || !doc["include"].HasMember("boxart"))
  {
    std::string warn = "TheGamesDBJSONRequest - Response had no include boxart data.\n";
    LOG(LogWarning) << warn;
    return;
  }

  const Value& boxart = doc["include"]["boxart"];

  if (!boxart.HasMember("base_url") || !boxart.HasMember("data") || !boxart.IsObject())
  {
    std::string warn = "TheGamesDBJSONRequest - Response include had no usable boxart data.\n";
    LOG(LogWarning) << warn;
    return;
  }

  resources.ensureResources();


  for (int i = 0; i < (int) games.Size(); ++i)
  {
    auto& v = games[i];
    try
    {
      processGame(v, boxart, results);
    }
    catch (std::runtime_error& e)
    {
      LOG(LogError) << "Error while processing game: " << e.what();
    }
  }
}
