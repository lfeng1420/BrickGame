#pragma once

#include "GlobalDef.h"

class CDataManager
{
public:
	CDataManager();
	~CDataManager();

	static CDataManager* getInstance();

	//初始化·
	void Init();

	//加载动画
	bool LoadGameAnim();

	vector<int>* GetAnimData(int iGameIndex, int iAnimIndex);

private:
	typedef vector<int> TVECTOR_ANIMDATA;
	typedef TVECTOR_ANIMDATA::iterator TVECTOR_ANIMDATA_ITER;

	typedef map<int, TVECTOR_ANIMDATA> TMAP_GAMEANIM;			//选择游戏界面的动画
	typedef TMAP_GAMEANIM::iterator TMAP_GAMEANIM_ITER;

	//枚举
	enum
	{
		ANIM_NUM = 4,		//每个游戏动画个数
		GAME_NUM = 1,		//游戏数量
	};


private:
	TMAP_GAMEANIM m_mapGameAnim;			//保存每个游戏对应的动画

};

