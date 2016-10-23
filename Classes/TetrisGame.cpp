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
		{ true, false, false, false },	//1000
		{ true, false, false, false },	//1000
		{ true, false, false, false },
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
	{
		{ true, false, false, false },	//1000
		{ false, false, false, false },	//0000
		{ false, false, false, false },
		{ false, false, false, false },
	},
};

//偏移调整
const int TETRIS_COLOFFSET[] = { 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0 };

//纵向长度
const int TETRIS_ROWCOUNT[] = { 2, 3, 2, 3, 2, 3, 2, 3, 3, 2, 3, 2, 3, 2, 3, 2, 2, 4, 1, 3, 2, 4, 1 };

//横向长度
const int TETRIS_COLCOUNT[] = { 3, 2, 3, 2, 3, 2, 3, 2, 2, 3, 2, 3, 2, 3, 2, 3, 2, 1, 4, 1, 1, 4, 1 };

//同类型开始索引
const int TETRIS_TYPE_START_IDX[] = { 0, 2, 4, 8, 12, 16, 17, 19, 20, 21, 22 };


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

	//更新等级和速度
	m_pGameScene->UpdateLevel(m_iLevel);
	m_pGameScene->UpdateSpeed(m_iSpeed);

	InitData();
}


void CTetrisGame::Play(float dt)
{
	if (m_enGameState == GAMESTATE_PAUSE)
	{
		//目前只有炸弹需要持续显示一段时间
		if (UpdateSelfState(dt))
		{
			if (++m_iSelfFlashCount >= BOMB_BOOM_FLASH_COUNT)
			{
				//切换进行中状态
				m_enGameState = GAMESTATE_RUNNING;

				//产生新方块
				RandNewShape();
			}
		}
	}
	else if (m_enGameState == GAMESTATE_RUNNING)
	{
		//闪烁状态更新
		bool bUpdateFlag = UpdateSelfState(dt);

		//方块移动
		bUpdateFlag = BrickMove(dt) || bUpdateFlag;

		if (!bUpdateFlag)
		{
			return;
		}
	}
	else if (m_enGameState == GAMESTATE_OVER)
	{
		//标记存档无效
		SET_BOOLVALUE("RECORD_VALID", false);

		m_pGameScene->RunScene(SCENE_GAMEOVER);
		return;
	}
	
	m_pGameScene->UpdateBricks();
}


bool CTetrisGame::GetBrickState(int iRowIndex, int iColIndex)
{
	//小方块特殊处理
	if (m_iCurShape == 22)
	{
		if (iRowIndex == m_stCurPos.m_iRowIdx && iColIndex == m_stCurPos.m_iColIdx)
		{
			return m_bSelfShowFlag;
		}
	}

	bool bBrickState = m_arrBrick[iRowIndex][iColIndex];

	if (iRowIndex >= m_stCurPos.m_iRowIdx && iRowIndex < m_stCurPos.m_iRowIdx + 4
		&& iColIndex >= m_stCurPos.m_iColIdx && iColIndex < m_stCurPos.m_iColIdx + 4)
	{
		int iActRowIdx = iRowIndex - m_stCurPos.m_iRowIdx;
		int iActColIdx = iColIndex - m_stCurPos.m_iColIdx;

		//如果开启了闪烁效果，则需要根据自身状态决定是否显示
		bool bState = TETRIS_SHAPE[m_iCurShape][iActRowIdx][iActColIdx];
		if (m_bFlashFlag)
		{
			if (iActRowIdx < TETRIS_ROWCOUNT[m_iCurShape] && iActColIdx < TETRIS_COLCOUNT[m_iCurShape])
			{
				bState = m_bSelfShowFlag && bState;
			}
		}
		
		return bState || bBrickState;
	}

	return bBrickState;
}


bool CTetrisGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	return TETRIS_SHAPE[m_iNextShape][iRowIndex][iColIndex];
}


SCENE_INDEX CTetrisGame::GetSceneType()
{
	return SCENE_TETRIS;
}


//上按下
void CTetrisGame::OnUpBtnPressed()
{
	TRACE("%s", __FUNCTION__);
	SaveGameData();

	m_pGameScene->ShowTips(TIPS_SAVEOK);
}


void CTetrisGame::OnLeftBtnPressed()
{
	//音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//移动一次
	m_fBtnCheckTime = BTN_CHECK_INTERVAL;

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
	m_fBtnCheckTime = BTN_CHECK_INTERVAL;

	//开启持续移动状态
	m_bRightBtnPressed = true;
}


void CTetrisGame::OnRightBtnReleased()
{
	m_bRightBtnPressed = false;
}

void CTetrisGame::OnDownBtnPressed()
{
	PLAY_EFFECT(EFFECT_CHANGE2);
	m_bFastMoveDown = true;
}


void CTetrisGame::OnDownBtnReleased()
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
		int iRowIdx = GetNextAddOrSubRowIdx(m_stCurPos.m_iColIdx);
		
		//要操作的方块位置
		iRowIdx += (bAddFlag ? -1 : 0);
		if (iRowIdx < m_stCurPos.m_iRowIdx + TETRIS_ROWCOUNT[m_iCurShape] || iRowIdx > ROW_NUM - 1)
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
			DeleteLine(false);
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

	//初始化分数
	m_iScore = 0;

	//加载存档
	bool bRecordValidFlag = GET_BOOLVALUE("TETRIS_RECORD_VALID", false);
	if (bRecordValidFlag)
	{
		CGeneralManager::getInstance()->LoadTetrisData(m_arrBrick);
		m_iScore = GET_INTVALUE("SCORE_RECORD", 0);
	}

	//更新显示分数
	m_pGameScene->UpdateScore(m_iScore, false);

	//随机下一个方块类型
	m_iNextShape = Random(0, GetShapeCount());

	//产生新的方块
	RandNewShape();

	//游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//时间相关
	m_fMoveDownTime = 0;
	m_fBtnCheckTime = 0;
	m_fSelfFlashTime = 0;

	//炸弹闪烁次数
	m_iSelfFlashCount = 0;

	//标记
	m_bLeftBtnPressed = false;
	m_bRightBtnPressed = false;
	m_bFastMoveDown = false;
	m_bSelfShowFlag = true;

	m_pGameScene->UpdateBricks();
}


void CTetrisGame::RandNewShape()
{
	//重置加速下落标记
	m_bFastMoveDown = false;

	//设置当前方块类型，重新随机下一个方块类型
	int iShape = m_iNextShape;
	m_iNextShape = Random(0, GetShapeCount());

	//更新小方块区域显示
	m_pGameScene->UpdateSmallBricks();

	//检查位置是否有效  2 每个形状都是4*4 一半即是2
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

	//特殊方块闪烁显示
	m_bFlashFlag = (m_iCurShape >= 19);
}


bool CTetrisGame::BrickMove(float dt)
{
	//下一个位置
	int iNextRowIdx = m_stCurPos.m_iRowIdx;
	int iNextColIdx = m_stCurPos.m_iColIdx;

	//按钮状态检查
	m_fBtnCheckTime += dt;
	if (m_fBtnCheckTime >= BTN_CHECK_INTERVAL)
	{
		m_fBtnCheckTime = 0;

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
			//处理非特殊方块以及点特殊方块
			if (m_iCurShape < 19 || m_iCurShape == 22)
			{
				if (m_iCurShape == 22)
				{
					//设置rowidx
					m_stCurPos.m_iRowIdx = GetEmptyPosRowIdx();
				}

				//已无法再往下移动或已到达最后一行，保存当前方块
				for (int i = 0; i < 4 && m_stCurPos.m_iRowIdx + i < ROW_NUM; ++i)
				{
					for (int j = 0; j < 4 && m_stCurPos.m_iColIdx + j < COLUMN_NUM; ++j)
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
					int iEndRowIdx = iStartRowIdx + TETRIS_ROWCOUNT[m_iCurShape] + BOMB_DELETE_LINE_COUNT + (m_bFastMoveDown ? m_iSpeed / 4 + 1 : 0);
					iEndRowIdx = (iEndRowIdx > ROW_NUM ? ROW_NUM : iEndRowIdx);
					for (int i = iStartRowIdx; i < iEndRowIdx; ++i)
					{
						for (int j = m_stCurPos.m_iColIdx; j < m_stCurPos.m_iColIdx + TETRIS_COLCOUNT[m_iCurShape]; ++j)
						{
							m_arrBrick[i][j] = false;
							m_pGameScene->UpdateBrick(i, j, false, false);
						}
					}
					
					//音效
					PLAY_EFFECT(EFFECT_BOOM);

					//更新炸弹位置，需要显示在最底下消除的那一行
					//修改：炸弹改为3行，但爆炸时需要显示四行，所以需要上移一行
					m_stCurPos.m_iRowIdx = iEndRowIdx - TETRIS_ROWCOUNT[m_iCurShape] - 1;

					//进入暂停状态
					m_enGameState = GAMESTATE_PAUSE;
					m_iSelfFlashCount = 0;

					return true;
				}
			}

			//检查消行
			if (!DeleteLine(true) && m_iCurShape < 19)
			{
				PLAY_EFFECT(EFFECT_ADD);
			}

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

	if (iSrcColIdx > ROW_NUM - TETRIS_COLCOUNT[iShapeIdx])
	{
		return false;
	}

	//特殊方块“点”处理
	if (iShapeIdx == 22)
	{
		return iSrcRowIdx != GetEmptyPosRowIdx();
	}

	for (int i = 0; i < 4; ++i)
	{
		int iRowIdx = iSrcRowIdx + i;
		if (iRowIdx >= ROW_NUM)
		{
			continue;
		}

		for (int j = 0; j < 4; ++j)
		{
			int iColIdx = iSrcColIdx + j;
			if (iColIdx >= COLUMN_NUM)
			{
				continue;
			}

			if (m_arrBrick[iRowIdx][iColIdx] && TETRIS_SHAPE[iShapeIdx][i][j])
			{
				return false;
			}
		}
	}

	return true;
}


bool CTetrisGame::DeleteLine(bool bEnd)
{
	int iDelCount = 0;

	if (!bEnd)
	{
		//仅需检查一行
		int iRowIdx = GetNextAddOrSubRowIdx(m_stCurPos.m_iColIdx);
		for (int i = 0; i < COLUMN_NUM; ++i)
		{
			if (!m_arrBrick[iRowIdx][i])
			{
				return false;
			}
		}

		DeleteSingleLine(iRowIdx);
		iDelCount = 1;
	}
	else
	{
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
			return false;
		}
	}

	//加分
	m_iScore += iDelCount * iDelCount * DELETE_LINE_ADD_SCORE;
	m_pGameScene->UpdateScore(m_iScore, false);
	PLAY_EFFECT(EFFECT_DELETE);

	return true;
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
	return (m_bExtraMode ? iCount : iCount - 4);
}


int CTetrisGame::GetNextAddOrSubRowIdx(int iColIdx)
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

	return iIndex;
}


bool CTetrisGame::UpdateSelfState(float dt)
{
	if (m_iCurShape < 19)
	{
		return false;
	}

	m_fSelfFlashTime += dt;
	if (m_fSelfFlashTime < SELF_FLASH_INTERVAL)
	{
		return false;
	}

	//重置
	m_fSelfFlashTime = 0;

	//更新状态
	m_bSelfShowFlag = !m_bSelfShowFlag;

	return true;
}


int CTetrisGame::GetEmptyPosRowIdx()
{
	//先找到第一个非空白位置
	int iRowIdx = 0;
	for (; iRowIdx < m_stCurPos.m_iRowIdx; ++iRowIdx)
	{
		if (m_arrBrick[iRowIdx][m_stCurPos.m_iColIdx])
		{
			break;
		}
	}

	//从底下往上找空白位置
	for (int iEmptyRowIdx = ROW_NUM - 1; iEmptyRowIdx >= iRowIdx; --iEmptyRowIdx)
	{
		if (!m_arrBrick[iEmptyRowIdx][m_stCurPos.m_iColIdx])
		{
			TRACE("%s: %d", __FUNCTION__, iEmptyRowIdx);
			return iEmptyRowIdx;
		}
	}

	TRACE("%s: %d", __FUNCTION__, iRowIdx);
	return iRowIdx;
}


void CTetrisGame::SaveGameData()
{
	//仅在非结束状态下保存数据
	if (m_enGameState == GAMESTATE_OVER)
	{
		return;
	}

	TRACE("%s", __FUNCTION__);

	//保存
	CGeneralManager::getInstance()->SaveTetrisData(m_arrBrick);
	//保存分数
	SET_INTVALUE("SCORE_RECORD", m_iScore);
}
