#include "Sound.h"
#include "AudioManager.h"
#include "Log.h"
#include "Settings.h"
#include "themes/ThemeData.h"

std::map< std::string, std::shared_ptr<Sound> > Sound::sMap;

std::shared_ptr<Sound> Sound::get(const std::string& path)
{
	auto it = sMap.find(path);
	if(it != sMap.end())
		return it->second;

	std::shared_ptr<Sound> sound = std::shared_ptr<Sound>(new Sound(path));
	AudioManager::getInstance()->registerSound(sound);
	sMap[path] = sound;
	return sound;
}

std::shared_ptr<Sound> Sound::getFromTheme(const std::shared_ptr<ThemeData>& theme, const std::string& view, const std::string& element)
{
	LOG(LogInfo) << " req sound [" << view << "." << element << "]";

	const ThemeData::ThemeElement* elem = theme->getElement(view, element, "sound");
	if((elem == nullptr) || !elem->has("path"))
	{
		LOG(LogInfo) << "   (missing)";
		return get("");
	}

	return get(elem->get<std::string>("path"));
}

Sound::Sound(const std::string & path)
  : mSampleData(nullptr),
    playing(false)
{
	loadFile(path);
}

Sound::~Sound()
{
	deinit();
}

void Sound::loadFile(const std::string & path)
{
	mPath = path;
	init();
}

void Sound::init()
{
	if(mSampleData != nullptr)
		deinit();

	if(mPath.empty())
		return;

	//load wav file via SDL
	mSampleData = Mix_LoadWAV(mPath.c_str());
	if(mSampleData == nullptr) {
		LOG(LogError) << "Error loading sound \"" << mPath << "\"!\n" << "	" << SDL_GetError();
		return;
	}
}

void Sound::deinit()
{
	playing = false;

	if(mSampleData != nullptr)
	{
            Mix_FreeChunk( mSampleData );
	}
}

void Sound::play()
{
	if(mSampleData == nullptr)
		return;
	

	if (!playing)
	{
		//flag our sample as playing
		playing = true;
	}
	Mix_PlayChannel( -1, mSampleData, 0 );
}

void Sound::stop()
{
	playing = false;
	
}
