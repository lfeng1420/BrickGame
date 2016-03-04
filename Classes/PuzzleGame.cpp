#include "PuzzleGame.h"

const bool TETRIS_SHAPE[][3][3] =
{
	{
		{false, true, true },	//011
		{ true, true, false },	//110
		{ false, false, false },//000
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

const int TETRIS_SHAPE_COUNT = sizeof(TETRIS_SHAPE) / sizeof(bool) / 9;

const int TETRIS_ROW_COUNT[] = {2, 3, 2, 3, 2, 3, 2, 3, 3, 2, 3, 2, 3, 2, 3, 2, 2};

const int TETRIS_COL_COUNT[] = { 3, 2, 3, 2, 3, 2, 3, 2, 2, 3, 2, 3, 2, 3, 2, 3, 2 };


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

	//初始化当前分数
	m_iScore = 0;

	//更新界面，分数、等级和生命
	m_pGameScene->UpdateScore(m_iScore, false);
	m_pGameScene->UpdateLevel(m_iLevel);
	

	InitData();
}


//更新
void CPuzzleGame::Play(float dt)
{
	if (!BrickGroupMove(dt))
	{
		return;
	}

	m_pGameScene->UpdateBricks();
}


//获取当前Brick状态
bool CPuzzleGame::GetBrickState(int iRowIndex, int iColIndex)
{
	//显示在界面中
	if (iRowIndex >= ROW_NUM - 3 && iRowIndex < ROW_NUM && iColIndex >= m_iCurColIdx && iColIndex < m_iCurColIdx + 3)
	{
		return TETRIS_SHAPE[m_iCurShape][iRowIndex - (ROW_NUM - 3)][iColIndex - m_iCurColIdx];
	}

	if (iRowIndex == ROW_NUM - 4)
	{
		return true;
	}

	return m_arrBricks[iRowIndex][iColIndex];
}


//生命数
bool CPuzzleGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 3 || iColIndex == 3)
	{
		return false;
	}

	if (iRowIndex == ROW_NUM - 4)
	{
		return true;
	}

	return TETRIS_SHAPE[m_iNextShape][iRowIndex][iColIndex];
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
	for (int i = 0; i < ROAD_COUNT; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_arrBrickGroup[i][j].m_iColCount = -1;
		}
	}

	//初始化所有方块以及显示状态
	for (int i = 0; i < ROW_NUM; ++i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrBricks[i][j] = false;
			m_pGameScene->UpdateBrick(i, j, false, false);
		}
	}

	//初始化移动行数
	m_iMoveRowCount = BRICK_GROUP_MAX_HEIGHT - 1;

	//时间相关
	m_fBrickMoveTime = 0;

	//随机下一形状
	m_iNextShape = Random(0, TETRIS_SHAPE_COUNT);

	//设置当前所在列
	m_iCurColIdx = COLUMN_NUM / 2 - 1;

	RandShape();
}


//在某个空闲位置创建方块组
void CPuzzleGame::CreateBrickGroup()
{
	int iIndex = GetBlankIndex();
	if (iIndex == -1)
	{
		return;
	}

	for (int i = 0; i < ROAD_COUNT; ++i)
	{
		//清空无效数字序列
		ClearInvalidNums();

		BRICKGROUP& refGroup = m_arrBrickGroup[i][iIndex];
		refGroup.m_iRowIdx = 0;
		refGroup.m_iColCount = Random(BRICK_GROUP_MIN_WIDTH, BRICK_GROUP_MAX_WIDTH + 1);

		//随机每一列
		for (int j = 5 * i; j < 5 * i + refGroup.m_iColCount; ++j)
		{
			//随机一个行数
			int iRowCount = Random(BRICK_GROUP_MIN_HEIGHT, BRICK_GROUP_MAX_HEIGHT + 1);
			
			//设置方块状态
			for (int k = 0; k < iRowCount; ++k)
			{
				m_arrBricks[k][j] = true;
			}
			
			//下一次不再随机该值
			AddInvalidNum(iRowCount);
		}
	}
}


//方块组移动
bool CPuzzleGame::BrickGroupMove(float dt)
{
	m_fBrickMoveTime += dt;
	if (m_fBrickMoveTime < BRICK_GROUP_MOVE_INTERVAL)
	{
		return false;
	}

	//重置
	m_fBrickMoveTime = 0;

	for (int i = 0; i < ROAD_COUNT; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			BRICKGROUP& refGroup = m_arrBrickGroup[i][j];
			if (refGroup.m_iColCount > 0)
			{
				++refGroup.m_iRowIdx;
			}
		}
	}

	//所有方块下移一格
	for (int i = ROW_NUM - 1; i >= 0; --i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrBricks[i][j] = ((i == 0) ? false : m_arrBricks[i - 1][j]);
		}
	}

	if (++m_iMoveRowCount >= BRICK_GROUP_MAX_HEIGHT)
	{
		m_iMoveRowCount = 0;
		CreateBrickGroup();
	}

	return true;
}


int CPuzzleGame::GetBlankIndex()
{
	for (int i = 0; i < 3; ++i)
	{
		bool bValidFlag = true;
		for (int j = 0; j < ROAD_COUNT; ++j)
		{
			if (m_arrBrickGroup[j][i].m_iColCount >= 0)
			{
				bValidFlag = false;
				break;
			}
		}

		if (bValidFlag)
		{
			return i;
		}
	}

	return -1;
}


void CPuzzleGame::RandShape()
{
	m_iCurShape = m_iNextShape;

	//随机下一个
	m_iNextShape = Random(0, TETRIS_SHAPE_COUNT);
	m_pGameScene->UpdateSmallBricks();
}
