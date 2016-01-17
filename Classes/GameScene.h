#pragma once
#include "cocos2d.h"
#include "GlobalDef.h"

class CSceneBase;

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

	//重置所有Brick
	void ResetAllBricks();

	//按钮响应
	void OnBtnClick(Ref* pSender, int iBtnIndex);


	CREATE_FUNC(CGameScene);

private:
	typedef map<int, CSceneBase*> TMAP_GAMEOBJ;
	typedef TMAP_GAMEOBJ::iterator TMAP_GAMEOBJ_ITER;

	//按钮索引
	enum BTN_INDEX
	{
		BTN_UP,
		BTN_RIGHT,
		BTN_DOWN,
		BTN_LEFT,
		BTN_FIRE,
		BTN_START,
		BTN_SOUND,
		BTN_RESET,
	};

private:
	cocos2d::Sprite* m_pBrick[ROW_NUM][COLUMN_NUM];		//Sprite数组

	cocos2d::Size m_visibleSize;						//屏幕大小

	TMAP_GAMEOBJ m_mapGameObj;							//各个游戏对象指针

	int m_iGameIndex;									//当前游戏索引

	bool m_arrBrickState[ROW_NUM][COLUMN_NUM];			//保存的当前所有Brick状态

	float m_fCurTime;									//当前距离上一次触发Play的时间
};

