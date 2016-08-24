#pragma once
#include "BGGlobal.h"

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
	void UpdateBricks(int iStartRowIdx = -1, int iStartColIdx = -1, int iEndRowIdx = -1, int iEndColIdx = -1);

	//重置所有Brick
	void ResetBricks();

	//显示新场景
	void RunScene(int iSceneIndex);

	//更新分数显示
	void UpdateScore(int iScore, bool bPlayEffect = true);

	//更新最高分显示
	void UpdateHighScore(int iGameIdx, int iHighScore = 0);

	//更新等级显示
	void UpdateLevel(int iLevel);

	//更新速度显示
	void UpdateSpeed(int iSpeed);

	//更新小方块序列
	void UpdateSmallBricks();

	//重置所有小Brick
	void ResetSmallBricks();

	//显示提示
	void ShowTips(TIPS_TYPE enTipType);


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

	//创建按键监听器
	void CreateKeyListener();

	//创建各个游戏对象
	void CreateGameObj();

	//帧更新
	void update(float dt);

	//按钮响应  iBtnIndex 对应BTN_INDEX索引
	void OnButtonEvent(Ref* pSender, Widget::TouchEventType enType, int iBtnIndex);

	//按钮按下
	void OnButtonPressed(int iBtnIndex);

	//按钮释放
	void OnButtonReleased(int iBtnIndex);
	
	//按钮按下 iBtnIndex 对应BTN_INDEX索引
	void OnButtonClick(Ref* pSender, int iBtnIndex);

	//Change the background image
	void ChangeBGPic();

	//Change the play button state when game state changed
	void ChangePlayState(bool bPlay);

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
		BTN_GIVESCORE,
	};

	enum 
	{
		NUM_WIDTH = 22,				//数字宽度

		NUM_HEIGHT = 36,			//数字高度

		NUM_PADDING = 2,			//数字间距

		BGPIC_COUNT = 2,			//背景图片数量

		CHANGEBG_INTERVAL = 1000,	//更改背景的间隔

		CLICK_INTERVAL = 2000,		//2秒

		BTN_HEIGHT = 95,			//按钮高度
	};

private:
	Sprite* m_pArrBrick[ROW_NUM][COLUMN_NUM];			//Sprite数组

	Size m_visibleSize;									//屏幕大小

	TMAP_GAMEOBJ m_mapGameObj;							//各个游戏对象指针

	int m_iSceneIndex;									//当前游戏索引

	bool m_arrBrickState[ROW_NUM][COLUMN_NUM];			//保存的当前所有Brick状态

	Sprite* m_pArrSmallBrick[4][4];						//小方块序列

	//Label* m_pLevelLabel;								//等级文本

	//Label* m_pSpeedLabel;								//速度文本

	//Label* m_pScoreLabel;								//分数文本

	//Label* m_pHighScoreLabel;							//最高分文本

	Sprite* m_pArrSpeed[2];								//速度Sprite序列

	Sprite* m_pArrLevel[2];								//等级Sprite序列

	Sprite* m_pArrScore[6];								//分数Sprite序列

	Sprite* m_pArrHighScore[6];							//最高分Sprite序列

	Sprite* m_pPauseSpr;								//暂停图标

	Sprite* m_pBgSpr;									//背景图片

	Sprite* m_pTipSpr;									//提示

	MenuItemToggle* m_pStartBtn;						//Start Button

	bool m_bGamePause;									//暂停标志

	int m_iBgColor;										//当前背景颜色序号，0白色，>=1自定义

	double m_fClickTime;								//Click love btn time, two:change the background with WP

	double m_lfClickExitTime;							//上一次点击退出时间

	double m_lfClickResetTime;							//上一次点击重置时间

	TIPS_TYPE m_enTipType;								//当前提示类型
};

