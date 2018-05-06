#pragma once

class CLoadScene : public LayerColor
{
public:
	SCENE_COMMON_FUNC(CLoadScene);

private:
	// Init ui
	void __InitUI();

	//Switch to the game scene
	void __SwitchToGameScene(float dt);

private:
	enum
	{
		UI_BRICKS_ROW_COUNT = 5,
		UI_BRICKS_COLUMN_COUNT = COLUMN_COUNT - 1,
	};
};

