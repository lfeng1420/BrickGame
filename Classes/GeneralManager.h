#pragma once

#include "BGGlobal.h"

class CGeneralManager
{
public:
	CGeneralManager();
	~CGeneralManager();

	static CGeneralManager* getInstance();

	//初始化·
	void Init();

	//加载动画
	bool LoadGameAnim();

	//获取最高分
	bool LoadHighScore();

	//保存最高分
	bool SaveHighScoreToFile();

	//加载声效
	void LoadSound();

	//根据游戏索引和动画索引获取动画数据
	vector<int>* GetAnimData(int iGameIndex, int iAnimIndex);

	//播放背景音乐
	void PlayMusic(const char* strName, bool bLoop = false);

	//播放音效
	void PlayEffect(const char* strName);

	//设置声音状态
	void SetSoundState(bool bState);

	//获取声音状态
	bool GetSoundState();

	//获取游戏最高分
	int GetHighScore(int iGameIdx);

	//设置最高分
	void SetHighScore(int iGameIdx, int iScore);

private:
	typedef vector<int> TVECTOR_ANIMDATA;
	typedef TVECTOR_ANIMDATA::iterator TVECTOR_ANIMDATA_ITER;

	typedef map<int, TVECTOR_ANIMDATA> TMAP_GAMEANIM;			//选择游戏界面的动画
	typedef TMAP_GAMEANIM::iterator TMAP_GAMEANIM_ITER;


	typedef map<int, int> TMAP_HIGHSCORE;
	typedef TMAP_HIGHSCORE::iterator TMAP_HIGHSCORE_ITER;

	//枚举
	enum
	{
		ANIM_NUM = 4,		//每个游戏动画个数
	};


private:
	TMAP_GAMEANIM m_mapGameAnim;			//保存每个游戏对应的动画

	TMAP_HIGHSCORE m_mapHighScore;			//最高分

	bool m_bSoundOn;						//是否打开音乐
};

