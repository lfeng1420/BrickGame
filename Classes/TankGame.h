#pragma once
#include "SceneBase.h"

class CTank;
class CBullet;
struct TANK_POS;

class CTankGame : public CSceneBase
{
public:
	CTankGame(CGameScene* pGameScene);
	~CTankGame();

	//---------------------    CSceneBase    ----------------------
	//初始化
	void Init();

	//更新
	void Play(float dt);

	//提供给每次更新单个Brick游戏（当前只有游戏结束）使用，获取当前改变的Brick行列索引
	void GetCurPos(int& iRowIndex, int& iColIndex);

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

	//上按下
	void OnUpBtnPressed();

	//上释放
	void OnUpBtnReleased();

	//下按下
	void OnDownPressed();

	//下释放
	void OnDownReleased();

	//Fire按下
	void OnFireBtnPressed();

	//Fire释放
	void OnFireBtnReleased();

	//---------------------    CSceneBase    ----------------------

private:
	//初始化数据，与Init不同，Init在切换到该场景时调用，InitData在重置时调用
	void InitData();

	//检查两个坦克位置之间是否有重叠
	bool CheckTankPos(const TANK_POS& stSrcPos, int iSrcDir, const TANK_POS& stDestPos, int iDestDir);

	//更新坦克位置
	void UpdateTankPos();

	//创建坦克
	void CreateTank();

private:
	enum
	{
		TANK_MAXNUM = 4,			//坦克最大数量
	};

private:
	CTank* m_pArrTank[5];						//坦克列表

	CBullet* m_pArrBullet[5];					//子弹列表

	int m_iSpeed;								//速度

	int m_iLevel;								//关卡

	int m_iLife;								//剩余生命

	int m_iScore;								//分数
};

