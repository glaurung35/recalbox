//
// Created by gugue_u on 04/01/2023.
//

#include <components/ComponentList.h>
#include "components/ComponentGrid.h"
#include <components/NinePatchComponent.h>
#include <components/OptionListComponent.h>
#include <themes/MenuThemeData.h>
#include "systems/SystemManager.h"
#include "games/SaveState.h"

class GuiSaveStates : public Gui
{
  public:
    GuiSaveStates(WindowManager& window, SystemManager& systemManager, FileData& game, const std::function<void(const std::string& slot)>& func, bool fromMenu);

    bool ProcessInput(const InputCompactEvent& event) override;

    bool getHelpPrompts(Help& help) override;

    void onSizeChanged() override;

    void Update(int deltaTime) override;

    void Render(const Transform4x4f& parentTrans) override;

    void PopulateGrid();

    void updateInformations();

    void launch(const String& slot);

  private:

    enum class Sort
    {
      Ascending,
      Descending,
    };

    SystemManager& mSystemManager;

    NinePatchComponent mBackground;
    ComponentGrid mGrid;
    std::shared_ptr<TextListComponent<SaveState*>> mList;
    std::shared_ptr<ImageComponent> mThumbnail;
    std::vector<SaveState> mSaveStates;
    FileData& mGame;
    float mTitleHeight;
    bool mIsLibretro;
    bool mFromMenu;
    SaveState mCurrentState;
    Sort mSort;
    std::function<void(const std::string& slot)> mFunc;

    void Delete();

    static bool AscendingComparer(const SaveState& first, const SaveState& second);

    static bool DescendingComparer(const SaveState& first, const SaveState& second);

    std::function<void()> func;

};