#pragma once

#include "views/gamelist/IGameListView.h"
#include "views/SystemView.h"

class SystemData;

// Used to smoothly transition the camera between multiple views (e.g. from system to system, from gamelist to gamelist).
class ViewController : public GuiComponent
{
public:
	static ViewController& Instance() { return *sInstance; };
	//inline static Window* getWindow(){return sInstance->mWindow;}

	explicit ViewController(Window* window);
  ~ViewController() override;

	// Try to completely populate the GameListView map.
	// Caches things so there's no pauses during transitions.
	void preload();

	/*!
	 * @brief Wake up the system if it is in a sleeping state
	 */
	void WakeUp() { mWindow->doWake(); }

	// If a basic view detected a metadata change, it can request to recreate
	// the current gamelist view (as it may change to be detailed).
	bool reloadGameListView(IGameListView* gamelist, bool reloadTheme = false);
	inline bool reloadGameListView(SystemData* system, bool reloadTheme = false) { return reloadGameListView(getGameListView(system).get(), reloadTheme); }
  void deleteAndReloadAll();
	void reloadAll(); // Reload everything with a theme.  Used when the "ThemeSet" setting changes.
	void setInvalidGamesList(SystemData* system);
	void setAllInvalidGamesList(SystemData* systemExclude);

	// Navigation.
	void goToNextGameList();
	void goToPrevGameList();
	bool goToGameList(std::string& systemName);
	void goToGameList(SystemData* system);
	void goToSystemView(SystemData* system);
	void goToStart();

	void onFileChanged(FileData* file, FileChangeType change);

	void updateFavorite(SystemData* system, FileData* file);

	// Plays a nice launch effect and launches the game at the end of it.
	// Once the game terminates, plays a return effect.
	void launch(FileData* game, Vector3f centerCameraOn = Vector3f(Renderer::getDisplayWidthAsFloat() / 2.0f, Renderer::getDisplayHeightAsFloat() / 2.0f, 0),
			const std::string& netplay = "", const std::string& core = "", const std::string& ip = "", const std::string& port = "");

	bool ProcessInput(const InputCompactEvent& event) override;
	void update(int deltaTime) override;
	void render(const Transform4x4f& parentTrans) override;

	enum class ViewMode
	{
		None,
		SplashScreen,
		SystemList,
		GameList,
	};

	struct State
	{
		ViewMode viewing;

		inline SystemData* getSystem() const { assert(viewing == ViewMode::GameList || viewing == ViewMode::SystemList); return system; }

	private:
		friend ViewController;
		SystemData* system;
	};

	inline const State& getState() const { return mState; }
	inline bool isViewing(ViewMode viewing) const { return mState.viewing == viewing; }

	std::vector<HelpPrompt> getHelpPrompts() override;
	HelpStyle getHelpStyle() override;

	std::shared_ptr<IGameListView> getGameListView(SystemData* system);
	std::shared_ptr<SystemView> getSystemListView();

private:
	static ViewController* sInstance;

	void playViewTransition();
	static int getSystemId(SystemData* system);

	std::shared_ptr<GuiComponent> mCurrentView;
	std::map< SystemData*, std::shared_ptr<IGameListView> > mGameListViews;
	std::shared_ptr<SystemView> mSystemListView;

	std::map<SystemData*, bool> mInvalidGameList;
	
	Transform4x4f mCamera;
	float mFadeOpacity;
	bool mLockInput;
	bool mFavoritesOnly;

	State mState;
  Window* mWindow;
};
