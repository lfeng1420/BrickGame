#pragma once
#include "GlobalDef.h"

class CSceneBase;

class CGameScene : public LayerColor
{
public:
	CGameScene();
	~CGameScene();

	static Scene* CreateScene();

	virtual bool init();

	//更新单个Brick状态，bSmallBrickFlag 是否更新小方块， bShowFlag显示或隐藏
	void UpdateBrick(int iRowIndex, int iColIndex, bool bSmallBrickFlag, bool bShowFlag);

	//更新所有Brick状态
	void UpdateBricks();

	//重置所有Brick
	void ResetBricks();

	//显示新场景
	void ShowNewScene(int iSceneIndex);

	//更新分数显示
	void UpdateScore(int iScore);

	//更新等级显示
	void UpdateLevel(int iLevel);

	//更新速度显示
	void UpdateSpeed(int iSpeed);

	//更新小方块序列
	void UpdateSmallBricks();

	//重置所有小Brick
	void ResetSmallBricks();

	CREATE_FUNC(CGameScene);

private:
	//初始化数据
	void InitData();

	//初始化Brick
	void InitBrick();

	//初始化UI:分数、等级等
	void InitUI();

	//根据指定的高度上限创建控制按钮
	void InitCotroller();

	//创建各个游戏对象
	void CreateGameObj();

	//帧更新
	void update(float dt);

	//按钮响应  iBtnIndex 对应BTN_INDEX索引
	void OnBtnClick(Ref* pSender, int iBtnIndex);

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
	Sprite* m_pArrBrick[ROW_NUM][COLUMN_NUM];				//Sprite数组

	Size m_visibleSize;									//屏幕大小

	TMAP_GAMEOBJ m_mapGameObj;							//各个游戏对象指针

	int m_iSceneIndex;									//当前游戏索引

	bool m_arrBrickState[ROW_NUM][COLUMN_NUM];			//保存的当前所有Brick状态

	Label* m_pScoreLabel;								//分数序列

	Label* m_pHighScoreLabel;							//分数序列

	Sprite* m_pArrSmallBrick[4][4];						//小方块序列

	Label* m_pLevelLabel;								//等级

	Label* m_pSpeedLabel;								//速度

	Sprite* m_pPauseSpr;								//暂停图标
};

