#include "PuzzleGame.h"

const bool TETRIS_SHAPE[] = 
{
	{
		{false, true, true },	//011
		{ true, true, false },	//110
		{ false, false, false },
	},
	{
		{ true, false, false },	//100
		{ true, true, false },	//110
		{ false, true, false },	//010
	},

	{
		{ true, true, false },	//110
		{ false, true, true },	//011
		{ false, false, false },//000
	},
	{
		{ false, true, false },	//010
		{ true, true, false },	//110
		{ true, false, false },	//100
	},

	{
		{false, true, false },	//010
		{true, true, true },	//111
		{ false, false, false },//000
	},
	{
		{ true, false, false },	//100
		{ true, true, false },	//110
		{ true, false, false },	//100
	},
	{
		{ true, true, true },	//111
		{ false, true, false },	//010
		{ false, false, false },//000
	},
	{
		{ false, true, false },	//010
		{ true, true, false },	//110
		{ false, true, false },	//010
	},

	{
		{ true, true, false },	//110
		{ true, false, false },	//100
		{ true, false, false },	//100
	},
	{
		{ true, true, true },	//111
		{ false, false, true },	//001
		{ false, false, false },//000
	},
	{
		{ false, true, false },	//010
		{ false, true, false },	//010
		{ true, true, false },	//110
	},
	{
		{ true, false, false },	//100
		{ true, true, true },	//111
		{ false, false, false },//000
	},

	{
		{ true, true, false },	//110
		{ false, true, false },	//010
		{ false, true, false },	//010
	},
	{
		{ false, false, true },	//001
		{ true, true, true },	//111
		{ false, false, false },//000
	},
	{
		{ true, false, false },	//100
		{ true, false, false },	//100
		{ true, true, false },	//110
	},
	{
		{ true, true, true },	//111
		{ true, false, false },	//100
		{ false, false, false },//000
	},

	{
		{ true, true, false },	//110
		{ true, true, false },	//110
		{ false, false, false },//000
	},
};



CPuzzleGame::CPuzzleGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CPuzzleGame::~CPuzzleGame()
{
}


//初始化
void CPuzzleGame::Init()
{
	//获取在选择游戏界面设置的速度和等级
	m_iSpeed = GET_INTVALUE("SPEED", 0);
	m_iLevel = GET_INTVALUE("LEVEL", 0);

	//默认生命数为4
	m_iLife = GET_INTVALUE("LIFE", 4);

	//初始化当前分数
	m_iScore = 0;

	//更新界面，分数、等级和生命
	m_pGameScene->UpdateScore(m_iScore, false);
	m_pGameScene->UpdateLevel(m_iLevel);
	m_pGameScene->UpdateSmallBricks();

	InitData();
}


//更新
void CPuzzleGame::Play(float dt)
{
	

	m_pGameScene->UpdateBricks();
}


//获取当前Brick状态
bool CPuzzleGame::GetBrickState(int iRowIndex, int iColIndex)
{
	return false;
}


//生命数
bool CPuzzleGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

	return false;
}


//获取游戏类型
SCENE_INDEX CPuzzleGame::GetSceneType()
{
	return SCENE_FLAPPYBIRD;
}


//上按下
void CPuzzleGame::OnLeftBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);
}


//Fire按下
void CPuzzleGame::OnRightBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);
}


//Fire按下
void CPuzzleGame::OnFireBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);
}


//初始化数据、变量等
void CPuzzleGame::InitData()
{
	//初始化游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//初始化所有方块组数据
	for (int i = 0; i < 10; ++i)
	{
		m_arrBrickGroup[i].m_iType = -1;	//初始化组类型即可
	}
}


//在某个空闲位置创建方块组
void CPuzzleGame::CreateBrickGroup()
{
	

}


//方块组移动
bool CPuzzleGame::BrickGroupMove()
{

}