#pragma once

class CSetupScene : public Layer
{
public:
	virtual ~CSetupScene();

	// Create scene
	static Scene* CreateScene(const TGameSceneContext* pContext = nullptr);

	// create
	static Layer* create(const TGameSceneContext* pContext = nullptr);

	// Init
	virtual bool init();

	// Init ui
	virtual void InitMainUI();

	// On orientation change
	virtual void OnOrientationChange();

	// Show volume layer
	virtual void ShowVolumeLayer();

	// Add menu item
	virtual void AddMenuItem(int nStrID, int nStatusStrID = STRID_MAX, const char* szStr = nullptr);

public:
	// Import game scene context
	void ImportGameSceneContext(const TGameSceneContext* pContext = nullptr);

	// Scorll view callback
	void ListViewCallback(Ref* pNode, ui::ListView::EventType enEventType);

protected:
	// Get menu item state
	bool __GetItemState(int nMenuID);

	// Get menu item state
	int __GetItemStateStrID(int nMenuID);

	// Get menu item value
	void __GetItemStateStr(int nMenuID, string& strText);

	// Update item state
	void __UpdateItemState(int nMenuID);

	// Update one menu item
	void __UpdateOneMenuItem(int nMenuIdx, const char* szText, bool bUpdateColorFlag = false);

	// Show tips
	void __ShowTips(int nStrID);

	// Create key event listener
	void __CreateKeyListener();

	// On night mode change
	void __OnNightModeChange();

	// On click back menu
	void __OnClickBackMenu();

	// On click save menu
	void __OnClickSaveMenu();

	// StrID to MenuID
	int __StrID2MenuID(int nStrID);

protected:
	enum
	{
		TITLE_FONT = 40,
		NORMAL_FONT = 30,
		MENU_SPR_HEIGHT = 55,
		MENU_ITEM_MARGIN = 10,
		MENU_LABEL_PADDING = 20,
		MENU_STATUS_TAG = 1000,
		MENU_LABEL_TAG = 1001,
		TIPS_LABEL_SIZE = 36,
		BRICKS_OFFSET_MAX = 40,
		DIRBTN_SCALE_MIN = 80,
	};

	enum _EnMenu
	{
		enMenu_Min,
		enMenu_GlobalSetting = enMenu_Min,
		enMenu_Vibration,
		enMenu_Sound,
		enMenu_Orientation,
		enMenu_NightMode,
		enMenu_RHMode,
		enMenu_BricksOffset,
		enMenu_DirBtnScale,
		enMenu_TetrisSetting,
		enMenu_AutoRecover,
		enMenu_SaveNow,
		enMenu_UpBtn,
		enMenu_OtherSetting,
		enMenu_Author,
		enMenu_Rate,
		enMenu_More,
		enMenu_Back,
		enMenu_Line,
		enMenu_Max,
	};

	typedef vector<int>		TVec_Menu;

protected:
	// Game scene context
	TGameSceneContext		m_stGameSceneContext;

	// ListView
	ListView*				m_pListView;

	// White layer
	LayerColor*				m_pLayer;

	// Tips label
	Label*					m_pTipsLabel;

	// Menu
	TVec_Menu				m_vecMenu;
};