#pragma once
#include "BGGlobal.h"

class CSceneBase;

class CGameScene : public LayerColor
{
public:
	CGameScene();
	~CGameScene();

	static Scene* CreateScene();

	//init
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

	//初始化背景
	void InitBgLayer();

	//初始化UI:分数、等级等
	void InitPortUI();

	//初始化横向UI
	void InitLandUI();

	//根据指定的高度上限创建控制按钮
	void InitPortController();

	//根据指定的高度上限创建控制按钮
	void InitLandController();

	//获取方块Size
	Size GetBrickSize(bool bLandVisible);

	//获取数字Size
	Size GetNumSize();

	//创建按键监听器
	void CreateKeyListener();

	//创建各个游戏对象
	void CreateGameObj();

	//帧更新
	void update(float dt);

	//按钮响应  iBtnIndex 对应BTN_INDEX索引
	void OnButtonEvent(Ref* pSender, Widget::TouchEventType enType, int iBtnIndex);

	//根据位置调整触摸索引
	bool AdjustClickIndex(Vec2 pos, int& nIndex);

	//按钮按下
	void OnButtonPressed(int iBtnIndex);

	//按钮释放
	void OnButtonReleased(int iBtnIndex);
	
	//按钮按下 iBtnIndex 对应BTN_INDEX索引
	void OnButtonClick(Ref* pSender, int iBtnIndex);

	//Change the background image
	void ChangeColorMode();

	//Change the play button state when game state changed
	void ChangePlayState(bool bPlay);

	//获取方块Sprite
	Sprite* GetBrickSprite(int nRowIdx, int nColIdx, bool bSmallFlag, bool bLandFlag);

	//初始化提示
	void InitTips();

	//改变Sprite
	void ChangeSprite(Sprite* pSprite, bool bNightMode);

	//改变MenuItemSprite
	void ChangeMenuItemSprite(MenuItemSprite* pMenuItemSpr, bool bNightMode);

	//改变MenuItemToggle
	void ChangeMenuItemToggle(MenuItemToggle* pMenuItemToggle, bool bNightMode);

	//改变Button
	void ChangeButton(Button* pButton, bool bNightMode);

	//根据模式返回sprite名字
	string GetSpriteNameByMode(const char* szName);

private:
	typedef map<int, CSceneBase*> TMAP_GAMEOBJ;
	typedef TMAP_GAMEOBJ::iterator TMAP_GAMEOBJ_ITER;

	//按钮索引
	enum BTN_INDEX
	{
		BTN_INVALID = -1,
		BTN_UP,
		BTN_RIGHT,
		BTN_DOWN,
		BTN_LEFT,
		BTN_DIRMAX,
		BTN_FIRE = BTN_DIRMAX,
		BTN_START,
		BTN_SOUND,
		BTN_RESET,
		BTN_GIVESCORE,
	};

	enum 
	{
		NUM_PADDING = 2,			//数字间距

		BGPIC_COUNT = 1,			//背景图片数量

		CHANGEBG_INTERVAL = 1000,	//更改背景的间隔

		CLICK_INTERVAL = 1200,		//1.2秒

		BTN_HEIGHT = 95,			//按钮高度
	};

private:
	Node* m_pLandNode;									//横向屏幕Node

	Node* m_pPortNode;									//纵向屏幕Node

	Sprite* m_pArrBrick[ROW_NUM][COLUMN_NUM];			//Sprite数组

	Sprite* m_pArrBrickLand[ROW_NUM][COLUMN_NUM];		//横向屏幕下的方块Sprite数组

	Size m_visibleSize;									//屏幕大小

	TMAP_GAMEOBJ m_mapGameObj;							//各个游戏对象指针

	int m_iSceneIndex;									//当前游戏索引

	bool m_arrBrickState[ROW_NUM][COLUMN_NUM];			//保存的当前所有Brick状态

	Sprite* m_pArrSmallBrick[4][4];						//小方块序列

	Sprite* m_pArrSmallBrickLand[4][4];					//横向小方块序列

	Sprite* m_pArrSpeed[2];								//速度Sprite序列

	Sprite* m_pArrSpeedLand[2];							//速度Sprite序列

	Sprite* m_pArrLevel[2];								//等级Sprite序列

	Sprite* m_pArrLevelLand[2];							//等级Sprite序列

	Sprite* m_pArrScore[6];								//分数Sprite序列

	Sprite* m_pArrScoreLand[6];							//分数Sprite序列

	Sprite* m_pArrHighScore[6];							//最高分Sprite序列

	Sprite* m_pArrHighScoreLand[6];						//最高分Sprite序列

	Sprite* m_pPauseSpr;								//暂停图标

	Sprite* m_pPauseSprLand;							//横屏暂停图标

	Sprite* m_pTipSpr;									//提示

	Sprite* m_pTipSprLand;								//提示

	MenuItemToggle* m_pStartBtn;						//Start Button

	MenuItemToggle* m_pStartBtnLand;					//Start Button

	MenuItemToggle* m_pSoundBtn;						//Sound Button

	MenuItemToggle* m_pSoundBtnLand;					//Sound Button

	bool m_bGamePause;									//暂停标志

	int m_iBgColor;										//当前背景颜色序号，0白色，>=1自定义

	double m_fClickLoveTime;							//点击心形按钮时间

	double m_lfClickExitTime;							//上一次点击退出时间

	double m_lfClickSndTime;							//上一次点击声音时间

	double m_lfClickResetTime;							//上一次点击重置时间

	TIPS_TYPE m_enTipType;								//当前提示类型

	Vec2 m_oControllerCenterPos;						//控制器中心位置

	Size m_oControllerCenterSize;						//控制器中心大小

	Vec2 m_oControllerLandCenterPos;					//控制器中心位置

	Size m_oControllerLandCenterSize;					//控制器中心大小

	int m_nRecordBtnIdx;								//记录按钮索引

	Size m_oBrickSize;									//方块大小

	Size m_oNumSize;									//方块大小

	LayerColor* m_pBgLayer;								//背景颜色
};

