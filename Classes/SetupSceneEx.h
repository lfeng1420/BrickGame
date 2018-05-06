#pragma once
#include "SetupScene.h"

// For landscape
class CSetupSceneEx : public CSetupScene
{
public:
	// Create scene
	static Scene* CreateScene(const TGameSceneContext* pContext = nullptr);

	// create
	static Layer* create(const TGameSceneContext* pContext = nullptr);

	// Init ui
	virtual void InitMainUI();

	// On orientation change
	virtual void OnOrientationChange();

	// Show volume layer
	virtual void ShowVolumeLayer();

	// Add menu item
	virtual void AddMenuItem(int nStrID, int nStatusStrID = STRID_MAX, const char* szStr = nullptr);
};