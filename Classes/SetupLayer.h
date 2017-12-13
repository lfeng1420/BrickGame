#pragma once
#include "BGGlobal.h"

class CGameScene;

class CSetupLayer : public cocos2d::Layer
{
public:
	CSetupLayer();
	~CSetupLayer();

	//init
	bool Init(CGameScene* pGameScene);

	//更新单个菜单
	void UpdateSingleMenu(int nMenuIdx);

	CREATE_FUNC(CSetupLayer);

private:
	//创建菜单
	void initAllMenu();
	
	//获取设置
	int getValueByMenuIdx(int nMenuIdx);

	//点击菜单触发
	void onClickMenu(Ref* pSender, int nMenuIdx);

	//菜单枚举转换为StrID
	int getStrIDByMenuIdx(int nMenuIdx);

	//主菜单枚举转换为StrID
	int getStrIDByMainMenuIdx(int nMainMenuIdx);

	//调整菜单和标签位置(Port)
	void adjustMenuPosForPortrait();

	//调整菜单和标签位置(Land)
	void adjustMenuPosForLandscape();

	//调整菜单和标签位置
	void adjustMenuPos();

	//更新菜单
	void updateSingleMenu(MenuItem* pMenuItem, int nMenuIdx);

	//更新菜单和标签
	void updateAllMenu();

	//根据主菜单索引获取菜单数量
	int getMenuCountByMainMenuIdx(int nMainMenuIdx);

public:
	enum
	{
		MENU_X_PADDING = 35,
		MENU_Y_PADDING = 10,

		TITLE_FONT_SIZE = 32,
		NORMAL_FONT_SIZE = 26,

		TITLE_TAG = 1420,
		BARRIER_TAG = 1421,
	};

private:
	Vector<Label*>				m_vecLabel;		//标签列表
	
	Vector<MenuItem*>			m_vecMenuItem;	//菜单列表

	CGameScene*					m_pGameScene;	//CGameScene
};

