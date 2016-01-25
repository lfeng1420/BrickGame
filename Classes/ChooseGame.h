#pragma once
#include "SceneBase.h"

class CChooseGame : public CSceneBase
{
public:
	CChooseGame(CGameScene* pGameScene);
	~CChooseGame();

	//---------------------    CSceneBase    ----------------------
	//初始化
	void Init();
	
	//更新
	void Play(float dt);

	//获取当前Brick状态
	bool GetBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	SCENE_INDEX GetSceneType();

	//左按下
	void OnLeftBtnPressed();

	//右按下
	void OnRightBtnPressed();

	//上按下
	void OnUpBtnPressed();

	//下按下
	void OnDownPressed();

	//Fire按下
	void OnFireBtnPressed();

	//开始
	void OnStart();

	//---------------------    CSceneBase    ----------------------

private:
	typedef vector<int> TVECTOR_ANIMDATA;

	enum 
	{
		ANIM_NUM = 4,

		REFRESH_INTERVAL = 1000,		//刷新间隔
	};

private:
	TVECTOR_ANIMDATA* m_pAnimData;		//指向动画数据的指针

	int m_iGameIndex;					//游戏索引

	int m_iAnimIndex;					//当前动画索引

	bool m_bFirstAnim;					//第一个动画无需等待

	int m_iSpeed;						//速度

	int m_iLevel;						//等级

	float m_fCurTime;					//当前时间
};

