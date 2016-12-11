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

	//更新单个标签
	void UpdateSingleMenuAndLabel(int nMenuIdx);

	CREATE_FUNC(CSetupLayer);

private:
	//创建菜单和标签
	void CreateAllMenuAndLabel();
	
	//更新菜单和标签
	void UpdateAllMenuAndLabel();

	//调整菜单和标签位置
	void AdjustMenuLabelPos();

	//获取设置
	int GetValueByMenuIdx(int nMenuIdx);

	//点击菜单触发
	void OnClickMenu(Ref* pSender, int nMenuIdx);

	//菜单枚举转换为StrID
	int GetStrIDByMenuIdx(int nMenuIdx);

	//检查菜单是否为标题
	bool CheckMenuTitleFlag(int nMenuIdx);

private:
	enum
	{
		MENU_X_PADDING = 10,
		MENU_Y_PADDING = 10,

#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8
		TITLE_FONT_SIZE = 36,
		NORMAL_FONT_SIZE = 29,
#else
		TITLE_FONT_SIZE = 36,
		NORMAL_FONT_SIZE = 29,
#endif
	};

private:
	Vector<Label*>				m_vecLabel;		//标签列表
	
	Vector<MenuItem*>			m_vecMenuItem;	//菜单列表

	CGameScene*					m_pGameScene;	//CGameScene
};

