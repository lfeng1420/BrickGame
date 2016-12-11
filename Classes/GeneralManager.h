#pragma once

#include "BGGlobal.h"

class CGeneralManager
{
public:
	CGeneralManager();
	~CGeneralManager();

	static CGeneralManager* getInstance();

	//根据游戏索引和动画索引获取动画数据
	vector<int>* GetAnimData(int iGameIndex, int iAnimIndex);

	//播放背景音乐
	void PlayMusic(const char* strName, bool bLoop = false);

	//播放音效
	void PlayEffect(const char* strName);

	//获取游戏最高分
	int GetHighScore(int iGameIdx);

	//设置最高分
	void SetHighScore(int iGameIdx, int iScore);

	//保存俄罗斯方块数据
	void SaveTetrisData(const bool(&arrState)[ROW_NUM][COLUMN_NUM]);

	//加载俄罗斯方块数据
	void LoadTetrisData(bool(&arrState)[ROW_NUM][COLUMN_NUM]);

	//保存最高分
	bool SaveHighScoreToFile();

	//获取菜单名字
	const char* GetStringByID(int nLangID, int nStrID);

private:
	//初始化·
	void Init();

	//加载动画
	bool LoadGameAnim();

	//获取最高分
	bool LoadHighScore();

	//加载声效
	void LoadSound();

	//加载字符串
	void LoadStrings();

private:
	typedef vector<int> TVECTOR_ANIMDATA;
	typedef TVECTOR_ANIMDATA::iterator TVECTOR_ANIMDATA_ITER;

	typedef map<int, TVECTOR_ANIMDATA> TMAP_GAMEANIM;			//选择游戏界面的动画
	typedef TMAP_GAMEANIM::iterator TMAP_GAMEANIM_ITER;


	typedef map<int, int> TMAP_HIGHSCORE;
	typedef TMAP_HIGHSCORE::iterator TMAP_HIGHSCORE_ITER;


	typedef map<int, std::string> TMAP_STRING;
	typedef map<int, TMAP_STRING>	TMAP_LANGUAGESTRING;

private:
	TMAP_GAMEANIM m_mapGameAnim;			//保存每个游戏对应的动画

	TMAP_HIGHSCORE m_mapHighScore;			//最高分

	TMAP_LANGUAGESTRING	m_mapStrings;		//字符串列表
};

