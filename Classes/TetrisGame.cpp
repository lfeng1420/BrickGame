#include "TetrisGame.h"

//方块各类型对应状态
const bool TETRIS_SHAPE[][4][4] = 
{
	{
		{false, true, true, false},		//0110
		{ true, true, false, false},	//1100
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, true, false, false },	//1100
		{ false, true, false, false },	//0100
		{ false, false, false, false },
	},

	{
		{ true, true, false, false },	//1100
		{ false, true, true, false },	//0110
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ false, true, false, false },	//0100
		{ true, true, false, false },	//1100
		{ true, false, false, false },	//1000
		{ false, false, false, false },
	},

	{
		{false, true, false, false},	//0100
		{true, true, true, false},		//1110
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, true, false, false },	//1100
		{ true, false, false, false },	//1000
		{ false, false, false, false },
	},
	{
		{ true, true, true, false },	//1110
		{ false, true, false, false },	//0100
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ false, true, false, false },	//0100
		{ true, true, false, false },	//1100
		{ false, true, false, false },	//0100
		{ false, false, false, false },
	},

	{
		{ true, true, false, false },	//1100
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ false, false, false, false },
	},
	{
		{ true, true, true, false },	//1110
		{ false, false, true, false },	//0010
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ false, true, false, false },	//0100
		{ false, true, false, false },	//0100
		{ true, true, false, false },	//1100
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, true, true, false },	//1110
		{ false, false, false, false },
		{ false, false, false, false },
	},

	{
		{ true, true, false, false },	//1100
		{ false, true, false, false },	//0100
		{ false, true, false, false },	//0100
		{ false, false, false, false },
	},
	{
		{ false, false, true, false },	//0010
		{ true, true, true, false },	//1110
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ true, true, false, false },	//1100
		{ false, false, false, false },
	},
	{
		{ true, true, true, false },	//1110
		{ true, false, false, false },	//1000
		{ false, false, false, false },
		{ false, false, false, false },
	},

	{
		{ true, true, false, false },	//1100
		{ true, true, false, false },	//1100
		{ false, false, false, false },
		{ false, false, false, false },
	},

	{
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
	},
	{
		{ true, true, true, true },		//1111
		{ false, false, false, false },
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{true, false, false, false},	//1000
		{ true, false, false, false },	//1000
		{ false, false, false, false },	
		{ false, false, false, false },
	},
	{
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ false, false, false, false },
		{ false, false, false, false },
	},
	{
		{ true, false, false, true },	//1001
		{ false, true, true, false },	//0110
		{ false, true, true, false },	//0110
		{ true, false, false, true },	//1001
	},
};

//偏移调整
const int TETRIS_COLOFFSET[] = { 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0 };

//纵向长度
const int TETRIS_ROWCOUNT[] = { 2, 3, 2, 3, 2, 3, 2, 3, 3, 2, 3, 2, 3, 2, 3, 2, 2, 4, 1, 2, 2, 4 };

//横向长度
const int TETRIS_COLCOUNT[] = { 3, 2, 3, 2, 3, 2, 3, 2, 2, 3, 2, 3, 2, 3, 2, 3, 2, 1, 4, 1, 1, 4 };

//同类型开始索引
const int TETRIS_TYPE_START_IDX[] = { 0, 2, 4, 8, 12, 16, 17, 19, 20, 21 };


CTetrisGame::CTetrisGame(CGameScene* pGameScene, bool bMode) : CSceneBase(pGameScene), m_bExtraMode(bMode)
{
}


CTetrisGame::~CTetrisGame()
{
}


void CTetrisGame::Init()
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


void CTetrisGame::Play(float dt)
{
	if (m_enGameState == GAMESTATE_RUNNING)
	{
		if (!BrickMove(dt))
		{
			return;
		}
	}

	if (m_enGameState == GAMESTATE_OVER)
	{
		m_pGameScene->RunScene(SCENE_GAMEOVER);
		return;
	}
	
	m_pGameScene->UpdateBricks();
}


bool CTetrisGame::GetBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex >= m_stCurPos.m_iRowIdx && iRowIndex < m_stCurPos.m_iRowIdx + 4
		&& iColIndex >= m_stCurPos.m_iColIdx && iColIndex < m_stCurPos.m_iColIdx + 4)
	{
		int iActRowIdx = iRowIndex - m_stCurPos.m_iRowIdx;
		int iActColIdx = iColIndex - m_stCurPos.m_iColIdx;
		return TETRIS_SHAPE[m_iCurShape][iActRowIdx][iActColIdx] || m_arrBrick[iRowIndex][iColIndex];
	}
	else
	{
		return m_arrBrick[iRowIndex][iColIndex];
	}
}


bool CTetrisGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	return TETRIS_SHAPE[m_iNextShape][iRowIndex][iColIndex];
}


SCENE_INDEX CTetrisGame::GetSceneType()
{
	return SCENE_TETRIS;
}


void CTetrisGame::OnLeftBtnPressed()
{
	//音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//移动一次
	m_fWaitTime = BTN_CHECK_INTERVAL;

	//开启持续移动状态
	m_bLeftBtnPressed = true;
}


void CTetrisGame::OnLeftBtnReleased()
{
	m_bLeftBtnPressed = false;
}


void CTetrisGame::OnRightBtnPressed()
{
	//音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//移动一次
	m_fWaitTime = BTN_CHECK_INTERVAL;

	//开启持续移动状态
	m_bRightBtnPressed = true;
}


void CTetrisGame::OnRightBtnReleased()
{
	m_bRightBtnPressed = false;
}

void CTetrisGame::OnDownPressed()
{
	PLAY_EFFECT(EFFECT_CHANGE2);
	m_bFastMoveDown = true;
}


void CTetrisGame::OnDownReleased()
{
	m_bFastMoveDown = false;
}


void CTetrisGame::OnFireBtnPressed()
{
	PLAY_EFFECT(EFFECT_CHANGE2);

	//处理特殊方块
	if (m_iCurShape == 19 || m_iCurShape == 20)
	{
		bool bAddFlag = (m_iCurShape == 19);

		//获取当前位置对应列包含的方块数量
		int iRowIdx = GetNextAddOrSubColIdx(m_stCurPos.m_iColIdx, bAddFlag);
		if (iRowIdx < 0 || iRowIdx > ROW_NUM - 1)
		{
			return;
		}
		int iColIdx = m_stCurPos.m_iColIdx;

		//设置状态
		m_arrBrick[iRowIdx][iColIdx] = bAddFlag;
		m_pGameScene->UpdateBrick(iRowIdx, iColIdx, false, bAddFlag);

		//如果是加方块，需要检查是否可以消除行
		if (bAddFlag)
		{
			DeleteLine();
		}

		return;
	}


	//检查当前形状所属类型
	int iAllTypeCount = sizeof(TETRIS_TYPE_START_IDX) / sizeof(int);
	int iStartIdx = TETRIS_TYPE_START_IDX[iAllTypeCount - 1];
	int iEndIdx = GetShapeCount();
	for (int i = 0; i < iAllTypeCount; ++i)
	{
		if (m_iCurShape < TETRIS_TYPE_START_IDX[i])
		{
			iStartIdx = TETRIS_TYPE_START_IDX[i - 1];
			iEndIdx = TETRIS_TYPE_START_IDX[i];
			break;
		}
	}
	
	int iShape = m_iCurShape;
	if (++iShape >= iEndIdx)
	{
		iShape = iStartIdx;
	}

	//检查是否可以变换
	if (CheckBrickPos(iShape, m_stCurPos.m_iRowIdx, m_stCurPos.m_iColIdx))
	{
		//更新当前形状
		m_iCurShape = iShape;

		//位置修正
		if (m_stCurPos.m_iColIdx + TETRIS_COLCOUNT[m_iCurShape] > COLUMN_NUM)
		{
			m_stCurPos.m_iColIdx = COLUMN_NUM - TETRIS_COLCOUNT[m_iCurShape];
		}

		//更新显示
		m_pGameScene->UpdateBricks(m_stCurPos.m_iRowIdx, m_stCurPos.m_iColIdx,
			m_stCurPos.m_iRowIdx + 4, m_stCurPos.m_iColIdx + 4);
	}
}


void CTetrisGame::InitData()
{
	//初始化所有方块状态
	for (int i = 0; i < ROW_NUM; ++i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrBrick[i][j] = false;
		}
	}

	//随机下一个方块类型
	m_iNextShape = Random(0, GetShapeCount());

	//产生新的方块
	RandNewShape();

	//游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//时间相关
	m_fMoveDownTime = 0;
	m_fWaitTime = 0;

	//标记
	m_bLeftBtnPressed = false;
	m_bRightBtnPressed = false;
	m_bFastMoveDown = false;

	//更新界面
	m_pGameScene->UpdateBricks();
}


void CTetrisGame::RandNewShape()
{
	//设置当前方块类型，重新随机下一个方块类型
	int iShape = m_iNextShape;
	m_iNextShape = Random(17, GetShapeCount());

	//更新小方块区域显示
	m_pGameScene->UpdateSmallBricks();

	//检查位置是否有效
	int iColIdx = COLUMN_NUM / 2 - 2 + TETRIS_COLOFFSET[iShape];
	if (!CheckBrickPos(iShape, 0, iColIdx))
	{
		m_enGameState = GAMESTATE_OVER;
	}
	else
	{
		m_iCurShape = iShape;
		m_stCurPos.m_iRowIdx = 0;
		m_stCurPos.m_iColIdx = iColIdx;
	}
}


bool CTetrisGame::BrickMove(float dt)
{
	//下一个位置
	int iNextRowIdx = m_stCurPos.m_iRowIdx;
	int iNextColIdx = m_stCurPos.m_iColIdx;

	//按钮状态检查
	m_fWaitTime += dt;
	if (m_fWaitTime >= BTN_CHECK_INTERVAL)
	{
		m_fWaitTime = 0;

		if (m_bLeftBtnPressed)
		{
			if (--iNextColIdx < 0)
			{
				iNextColIdx = 0;
			}
		}
		else if (m_bRightBtnPressed)
		{
			int iMaxColIdx = COLUMN_NUM - TETRIS_COLCOUNT[m_iCurShape];
			if (++iNextColIdx > iMaxColIdx)
			{
				iNextColIdx = iMaxColIdx;
			}
		}
	}

	//下移
	m_fMoveDownTime += dt;
	if (m_fMoveDownTime >= (m_bFastMoveDown ? 30 : (BRICK_MOVE_INTERVAL - 40 * m_iSpeed)))
	{
		m_fMoveDownTime = 0;
		++iNextRowIdx;
	}

	//没有发生位置变更，不更新
	if (iNextRowIdx == m_stCurPos.m_iRowIdx && iNextColIdx == m_stCurPos.m_iColIdx)
	{
		return false;
	}

	//检查下一个位置是否有效
	bool bNextPosValidFlag = CheckBrickPos(m_iCurShape, iNextRowIdx, iNextColIdx);
	if (!bNextPosValidFlag)
	{
		if (m_stCurPos.m_iRowIdx == 0 && iNextRowIdx == 1)
		{
			//如果没有移动过，但此时已无法再往下移动则游戏结束
			m_enGameState = GAMESTATE_OVER;
			return true;
		}

		//恢复列位置，再次检查位置是否有效
		iNextColIdx = m_stCurPos.m_iColIdx;
		if (!CheckBrickPos(m_iCurShape, iNextRowIdx, iNextColIdx))
		{
			//处理非特殊方块
			if (m_iCurShape < 19)
			{
				//已无法再往下移动或已到达最后一行，保存当前方块
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						if (TETRIS_SHAPE[m_iCurShape][i][j])
						{
							m_arrBrick[m_stCurPos.m_iRowIdx + i][m_stCurPos.m_iColIdx + j] = true;
						}
					}
				}
			}
			else
			{
				//如果是爆炸方块，消除底下的指定行数
				if (m_iCurShape == 21)
				{
					int iStartRowIdx = m_stCurPos.m_iRowIdx;
					int iEndRowIdx = iStartRowIdx + TETRIS_ROWCOUNT[m_iCurShape] + BOOM_SHAPE_DELETE_LINE_COUNT;
					iEndRowIdx = (iEndRowIdx > ROW_NUM ? ROW_NUM : iEndRowIdx);
					for (int i = iStartRowIdx; i < iEndRowIdx; ++i)
					{
						for (int j = m_stCurPos.m_iColIdx; j < m_stCurPos.m_iColIdx + TETRIS_COLCOUNT[m_iCurShape]; ++j)
						{
							m_arrBrick[i][j] = false;
							m_pGameScene->UpdateBrick(i, j, false, false);
						}
					}
					
				}
			}

			//重置加速下落标记
			m_bFastMoveDown = false;

			//检查消行
			DeleteLine();

			//产生新的方块
			RandNewShape();
			return true;
		}
	}

	//更新位置
	m_stCurPos.m_iRowIdx = iNextRowIdx;
	m_stCurPos.m_iColIdx = iNextColIdx;

	return true;
}


bool CTetrisGame::CheckBrickPos(int iShapeIdx, int iSrcRowIdx, int iSrcColIdx)
{
	if (iSrcRowIdx > ROW_NUM - TETRIS_ROWCOUNT[iShapeIdx])
	{
		return false;
	}

	for (int i = 0; i < 4; ++i)
	{
		int iRowIdx = iSrcRowIdx + i;
		for (int j = 0; j < 4; ++j)
		{
			int iColIdx = iSrcColIdx + j;
			if (m_arrBrick[iRowIdx][iColIdx] && TETRIS_SHAPE[iShapeIdx][i][j])
			{
				return false;
			}
		}
	}

	return true;
}


void CTetrisGame::DeleteLine()
{
	int iDelCount = 0;

	for (int i = m_stCurPos.m_iRowIdx; i < m_stCurPos.m_iRowIdx + TETRIS_ROWCOUNT[m_iCurShape]; ++i)
	{
		bool bCanDeleteFlag = true;
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			if (!m_arrBrick[i][j])
			{
				bCanDeleteFlag = false;
				break;
			}
		}

		//消除单行
		if (bCanDeleteFlag)
		{
			DeleteSingleLine(i);
			++iDelCount;
		}
	}

	if (iDelCount == 0)
	{
		return;
	}

	//加分
	m_iScore += iDelCount * iDelCount * DELETE_LINE_ADD_SCORE;
	m_pGameScene->UpdateScore(m_iScore, false);
	PLAY_EFFECT(EFFECT_DELETE);
}


void CTetrisGame::DeleteSingleLine(int iRowIdx)
{
	for (int i = iRowIdx; i > 0; --i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrBrick[i][j] = m_arrBrick[i - 1][j];
		}
	}

	for (int j = 0; j < COLUMN_NUM; ++j)
	{
		m_arrBrick[0][j] = false;
	}
}


int CTetrisGame::GetShapeCount()
{
	int iCount = sizeof(TETRIS_SHAPE) / sizeof(bool) / 16;
	return (m_bExtraMode ? iCount : iCount - 3);
}


int CTetrisGame::GetNextAddOrSubColIdx(int iColIdx, bool bAddFlag)
{
	int iIndex = ROW_NUM;
	for (int i = 0; i < ROW_NUM; ++i)
	{
		if (m_arrBrick[i][iColIdx])
		{
			iIndex = i;
			break;
		}
	}

	return bAddFlag ? (iIndex - 1) : iIndex;
}


