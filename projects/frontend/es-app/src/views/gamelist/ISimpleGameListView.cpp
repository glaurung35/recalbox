#include <Log.h>
#include <RecalboxConf.h>
#include "views/gamelist/ISimpleGameListView.h"
#include "ThemeData.h"
#include "SystemData.h"
#include "Window.h"
#include "views/ViewController.h"
#include "Sound.h"
#include "Settings.h"
#include "Gamelist.h"
#include "Locale.h"

ISimpleGameListView::ISimpleGameListView(Window* window, FileData* root) : IGameListView(window, root),
mHeaderText(window), mHeaderImage(window), mBackground(window), mFavoriteChange(false)
{
	mHeaderText.setText("Logo Text");
	mHeaderText.setSize(mSize.x(), 0);
	mHeaderText.setPosition(0, 0);
	mHeaderText.setAlignment(ALIGN_CENTER);
	mHeaderText.setDefaultZIndex(50);
	
	mHeaderImage.setResize(0, mSize.y() * 0.185f);
	mHeaderImage.setOrigin(0.5f, 0.0f);
	mHeaderImage.setPosition(mSize.x() / 2, 0);
	mHeaderImage.setDefaultZIndex(50);

	mBackground.setResize(mSize.x(), mSize.y());
	mBackground.setDefaultZIndex(0);

	addChild(&mHeaderText);
	addChild(&mBackground);
}

void ISimpleGameListView::onThemeChanged(const std::shared_ptr<ThemeData>& theme)
{
	using namespace ThemeFlags;
	mBackground.applyTheme(theme, getName(), "background", ALL);
	mHeaderImage.applyTheme(theme, getName(), "logo", ALL);
	mHeaderText.applyTheme(theme, getName(), "logoText", ALL);
	// Remove old theme extras
	for (auto extra : mThemeExtras)
	{
		removeChild(extra);
		delete extra;
	}
	mThemeExtras.clear();
	
	// Add new theme extras
	mThemeExtras = ThemeData::makeExtras(theme, getName(), mWindow);
	for (auto extra : mThemeExtras)
	{
		addChild(extra);
	}

	if(mHeaderImage.hasImage())
	{
		removeChild(&mHeaderText);
		addChild(&mHeaderImage);
	}else{
		addChild(&mHeaderText);
		removeChild(&mHeaderImage);
	}
}

void ISimpleGameListView::onFileChanged(FileData* file, FileChangeType change)
{
	// we could be tricky here to be efficient;
	// but this shouldn't happen very often so we'll just always repopulate
	FileData* cursor = getCursor();
	int index = getCursorIndex();
	populateList(getRoot()->getChildren());
	setCursorIndex(index);

    if(change == FileChangeType::FILE_REMOVED) {
        bool favorite = file->metadata.get("favorite") == "true";
        delete file;
        if (favorite) {
            ViewController::get()->setInvalidGamesList(SystemData::getFavoriteSystem());
            ViewController::get()->getSystemListView()->manageFavorite();
        }
    }
	/* Favorite */
	if(file->getType() == GAME){
		SystemData * favoriteSystem = SystemData::getFavoriteSystem();
		bool isFavorite = file->metadata.get("favorite") == "true";
		bool foundInFavorite = false;
		/* Removing favortie case : */
		for(auto gameInFavorite = favoriteSystem->getRootFolder()->getChildren().begin();
			gameInFavorite != favoriteSystem->getRootFolder()->getChildren().end();
			gameInFavorite ++){
				if((*gameInFavorite) == file){
					if(!isFavorite){
						favoriteSystem->getRootFolder()->removeAlreadyExisitingChild(file);
						ViewController::get()->setInvalidGamesList(SystemData::getFavoriteSystem());
						ViewController::get()->getSystemListView()->manageFavorite();
					}
					foundInFavorite = true;
					break;
				}
		}
		/* Adding favorite case : */
		if(!foundInFavorite && isFavorite){
			favoriteSystem->getRootFolder()->addAlreadyExisitingChild(file);
			ViewController::get()->setInvalidGamesList(SystemData::getFavoriteSystem());
			ViewController::get()->getSystemListView()->manageFavorite();
		}
	}
}

bool ISimpleGameListView::input(InputConfig* config, Input input)
{
	bool hideSystemView = RecalboxConf::getInstance()->get("emulationstation.hidesystemview") == "1";

	if(input.value != 0)
	{
		if(config->isMappedTo("b", input))
		{
			FileData* cursor = getCursor();
			if(cursor->getType() == GAME)
			{
				//Sound::getFromTheme(getTheme(), getName(), "launch")->play();
				launch(cursor);
			}else{
				// it's a folder
				if(cursor->getChildren().size() > 0)
				{
					mCursorStack.push(cursor);
					populateList(cursor->getChildren());
					setCursor(getCursor());
				}
			}
				
			return true;
		}else if(config->isMappedTo("a", input))
		{
			if(mCursorStack.size())
			{
				populateList(getRoot()->getChildren());
				setCursor(mCursorStack.top());
				mCursorStack.pop();
				//Sound::getFromTheme(getTheme(), getName(), "back")->play();
			}else if (!hideSystemView) {
				onFocusLost();

				if (mFavoriteChange)
				{
					ViewController::get()->setInvalidGamesList(getRoot()->getSystem());
					mFavoriteChange = false;
				}

				ViewController::get()->goToSystemView(getRoot()->getSystem());
			}

			return true;
		}else if (config->isMappedTo("y", input))
		{
			FileData* cursor = getCursor();
			if (!ViewController::get()->getState().getSystem()->isFavorite() && cursor->getSystem()->getHasFavorites())
			{
				if (cursor->getType() == GAME)
				{
					mFavoriteChange = true;
					MetaDataList* md = &cursor->metadata;
					std::string value = md->get("favorite");
					bool removeFavorite = false;
					SystemData *favoriteSystem = SystemData::getFavoriteSystem();
					if (value.compare("false") == 0)
					{
					  md->set("favorite", "true");
						if(favoriteSystem != NULL) {
						  favoriteSystem->getRootFolder()->addAlreadyExisitingChild(cursor);
						}
					}
					else
					{
						md->set("favorite", "false");
						if(favoriteSystem != NULL) {
						  favoriteSystem->getRootFolder()->removeAlreadyExisitingChild(cursor);
						}
						removeFavorite = true;
					}
					if(favoriteSystem != NULL) {
					  ViewController::get()->setInvalidGamesList(favoriteSystem);
					  ViewController::get()->getSystemListView()->manageFavorite();
					}
					int cursorPlace = getCursorIndex();
					populateList(cursor->getParent()->getChildren());
					setCursorIndex(cursorPlace + (removeFavorite ? -1 : 1));
					updateInfoPanel();
				}
			}
		}else if(config->isMappedTo("right", input))
		{
			if(Settings::getInstance()->getBool("QuickSystemSelect") && !hideSystemView)
			{
				onFocusLost();
				if (mFavoriteChange)
				{
					ViewController::get()->setInvalidGamesList(getCursor()->getSystem());
					mFavoriteChange = false;
				}
				ViewController::get()->goToNextGameList();
				return true;
			}
		}else if(config->isMappedTo("left", input))
		{
			if(Settings::getInstance()->getBool("QuickSystemSelect") && !hideSystemView)
			{
				onFocusLost();
				if (mFavoriteChange)
				{
					ViewController::get()->setInvalidGamesList(getCursor()->getSystem());
					mFavoriteChange = false;
				}
				ViewController::get()->goToPrevGameList();
				return true;
			}
		}
	}

	return IGameListView::input(config, input);
}
