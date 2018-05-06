#pragma once
#include "GameScene.h"

// For landscape
class CGameSceneEx : public CGameScene
{
public:
	// Create scene
	static Scene* CreateScene(const TGameSceneContext* pContext = nullptr);

	// create
	static LayerColor* create(const TGameSceneContext* pContext = nullptr);

	// Init ui
	virtual void InitUI();

	// Update level or speed
	virtual void UpdateLevelOrSpeed(Vector<Sprite*> vecSpr, int& nOldVal, int nNewVal);

	// Adjust clicked button id
	virtual bool AdjustClickBtnID(Vec2 pos, int& nBtnID);

private:
	// Initialize all bricks
	void __InitAllBricksEx(float& fBottomY, float& fTopY);

	// Initialize controller
	void __InitControllerEx(float fBottomCenterY, float fTopY);

	// Initialize menu
	void __InitBottomMenuEx(float& fBottomY);

	// Initialize all labels
	void __InitRightUIEx(float fBottomCenterY, float fTopY);

	// Apply right hand mode
	void __ApplyRightHandModeEx();
};