#pragma once
#include "SceneBase.h"
class CTetrisGame : public CSceneBase
{
public:
	CTetrisGame(CGameScene* pGameScene, bool bMode);
	~CTetrisGame();

	//---------------------    CSceneBase    ----------------------
	//初始化
	void Init();

	//更新
	void Play(float dt);

	//获取当前Brick状态
	bool GetBrickState(int iRowIndex, int iColIndex);

	//获取小方块序列中的方块状态
	bool GetSmallBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	SCENE_INDEX GetSceneType();

	//左按下
	void OnLeftBtnPressed();

	//左释放
	void OnLeftBtnReleased();

	//右按下
	void OnRightBtnPressed();

	//右释放
	void OnRightBtnReleased();

	//下按下
	void OnDownPressed();

	//下释放
	void OnDownReleased();

	//Fire按下
	void OnFireBtnPressed();

	//---------------------    CSceneBase    ----------------------


public:
	//初始化数据，与Init不同，Init在切换到该场景时调用，InitData在重置时调用
	void InitData();

	//随机产生新的方块形状
	void RandNewShape();

	//方块移动
	bool BrickMove(float dt);

	//检查位置是否有效
	bool CheckBrickPos(int iShapeIdx, int iSrcRowIdx, int iSrcColIdx);

	//消行
	void DeleteLine(bool bEnd);

	//消除单行
	void DeleteSingleLine(int iRowIdx);

	//获取可用形状数量
	inline int GetShapeCount();

	//获取指定列的方块数量
	int GetNextAddOrSubRowIdx(int iColIdx);

	//更新自己的显示状态
	bool UpdateSelfState(float dt);

private:
	enum 
	{
		BRICK_MOVE_INTERVAL = 500,			//方块移动等待时间

		DELETE_LINE_ADD_SCORE = 10,			//消除行加分

		BTN_CHECK_INTERVAL = 100,			//按钮检查时间间隔

		BOOM_SHAPE_DELETE_LINE_COUNT = 2,	//爆炸方块消除的行数

		SELF_FLASH_INTERVAL = 70,			//闪烁刷新时间
	};

private:
	int m_iSpeed;								//速度

	int m_iLevel;								//关卡

	int m_iScore;								//分数

	bool m_arrBrick[ROW_NUM][COLUMN_NUM];		//方块状态序列

	int m_iNextShape;							//下一个类型

	int m_iCurShape;							//当前类型

	POSITION m_stCurPos;						//当前方块位置

	GAME_STATE m_enGameState;					//游戏状态

	float m_fMoveDownTime;						//下移刷新时间

	float m_fBtnCheckTime;						//按钮检查等待时间

	float m_fSelfFlashTime;						//闪烁等待时间

	bool m_bSelfShowFlag;						//显示标记

	bool m_bFlashFlag;							//是否打开闪烁

	bool m_bLeftBtnPressed;						//左方向按钮按下状态

	bool m_bRightBtnPressed;					//右方向按钮按下状态

	bool m_bFastMoveDown;						//是否加速下降

	bool m_bExtraMode;							//是否开启附加模式
};

