#pragma once
#include "SceneBase.h"

class CChooseGame : public CSceneBase
{
public:
	CChooseGame();
	~CChooseGame();

	//---------------------    CSceneBase    ----------------------
	//初始化
	void Init();
	
	//游戏进行，返回false时表示游戏结束
	bool Play();

	//获取当前Brick状态
	bool GetBrickState(int iRowIndex, int iColIndex);

	//获取游戏类型
	SCENE_INDEX GetSceneType();

	//获取每次执行完Play后等待的时间
	float GetRefreshTime();

	//---------------------    CSceneBase    ----------------------

private:
	typedef vector<int> TVECTOR_ANIMDATA;

	enum 
	{
		ANIM_NUM = 4,
	};

private:
	TVECTOR_ANIMDATA* m_pAnimData;		//指向动画数据的指针

	int m_iAnimIndex;					//当前动画索引

	bool m_bFirstAnim;					//第一个动画无需等待
};

