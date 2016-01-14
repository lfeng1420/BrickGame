#pragma once
#include "cocos2d.h"
#include "GlobalDef.h"

class CGameBase;

class CGameScene : public cocos2d::LayerColor
{
public:
	CGameScene();
	~CGameScene();

	static cocos2d::Scene* CreateScene();

	virtual bool init();

	//初始化数据
	void InitData();

	//初始化Brick
	void InitBrick();

	//根据指定的高度上限创建控制按钮
	void InitCotroller();

	//创建各个游戏对象
	void CreateGameObj();

	void update(float dt);

	//更新单个Brick状态
	void UpdateBrick(int iRowIndex, int iColIndex, bool bShowFlag);

	//更新所有Brick状态
	void UpdateBricks();

	CREATE_FUNC(CGameScene);

private:
	typedef std::map<int, CGameBase*> TMAP_GAMEOBJ;
	typedef TMAP_GAMEOBJ::iterator TMAP_GAMEOBJ_ITER;

private:
	cocos2d::Sprite* m_pBrick[ROW_NUM][COLUMN_NUM];		//Sprite数组

	cocos2d::Size m_visibleSize;						//屏幕大小

	TMAP_GAMEOBJ m_mapGameObj;							//各个游戏对象指针

	int m_iGameIndex;									//当前游戏索引
};

