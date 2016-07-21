#include "MatchGame.h"

const bool BRICK_STATE[][2][2] =
{
	{
		{ false, false },
		{ true, false, },
	},
	{
		{ true, false },
		{ true, false, },
	},
	{
		{ true, false },
		{ true, true, },
	},
	{
		{ true, true },
		{ true, true, },
	},
};

CMatchGame::CMatchGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CMatchGame::~CMatchGame()
{
}


//初始化
void CMatchGame::Init()
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

	//初始化匹配成功计数
	m_iMatchSuccCount = 0;

	InitData();
}


//更新
void CMatchGame::Play(float dt)
{
	if (m_enGameState ==  GAMESTATE_PASS)
	{
		//时间更新
		m_fWaitTime += dt;
		if (m_fWaitTime < GAMEPASS_REFRESH_INTERVAL)
		{
			return;
		}
		m_fWaitTime = 0;

		if (m_iAddScoreCount < GAMEPASS_ADDCOUNT)
		{
			++m_iAddScoreCount;
			m_iScore += MACTHSUCC_ADDSCORE;
			m_pGameScene->UpdateScore(m_iScore);
			return;
		}
		else
		{
			//更新速度和等级
			if (++m_iSpeed > 10)
			{
				m_iSpeed = 0;
				if (++m_iLevel > 10)
				{
					m_iLevel = 0;
				}
			}

			//更新显示
			m_pGameScene->UpdateLevel(m_iLevel);
			m_pGameScene->UpdateSpeed(m_iSpeed);

			//重置匹配次数
			m_iMatchSuccCount = 0;

			//重置数据
			InitData();
		}
	}

	if (m_enGameState == GAMESTATE_RUNNING)
	{
		//方块移动
		bool bUpdateFlag = DestBricksMove(dt);
		bUpdateFlag = MyBricksMove(dt) || bUpdateFlag;
		if (!bUpdateFlag)
		{
			return;
		}
	}

	if (m_enGameState == GAMESTATE_OVER)
	{
		//时间更新
		m_fWaitTime += dt;
		if (m_fWaitTime < BOOM_REFRESH_INTERVAL)
		{
			return;
		}
		m_fWaitTime = 0;

		if (m_iShowBoomCount < BOOM_SHOWCOUNT)
		{
			m_bShowBoom = !m_bShowBoom;
			++m_iShowBoomCount;
		}
		else
		{
			//设置剩余生命
			--m_iLife;
			m_pGameScene->UpdateSmallBricks();

			//检查是否有剩余生命，没有则返回游戏结束界面
			if (m_iLife <= 0)
			{
				m_pGameScene->RunScene(SCENE_GAMEOVER);
				return;
			}

			//重置数据
			InitData();
		}
	}

	m_pGameScene->UpdateBricks();
}


//获取当前Brick状态
bool CMatchGame::GetBrickState(int iRowIndex, int iColIndex)
{
	if (m_enGameState == GAMESTATE_OVER)
	{
		int iBoomStartRowIdx = (m_iMyRowIdx > ROW_NUM - 4 ? ROW_NUM - 4 : m_iMyRowIdx);
		if (iRowIndex >= iBoomStartRowIdx && iRowIndex < iBoomStartRowIdx + 4)
		{
			int iIndex = iColIndex / 5;
			for (int i = 0; i < BRICK_MATCH_NUM; ++i)
			{
				if ((m_arrBoomIndex[i] == iColIndex / 5) && (iColIndex % 5 < 4))
				{
					return m_bShowBoom && BOOM_STATE[iRowIndex - iBoomStartRowIdx][iColIndex % 5];
				}
			}
		}
	}

	do 
	{
		bool bMyFlag = false;
		int iAcutalRowIdx = 0;

		if (iRowIndex < m_iDestRowIdx + 2 && iRowIndex >= m_iDestRowIdx)
		{
			iAcutalRowIdx = iRowIndex - m_iDestRowIdx;
		}
		else if (iRowIndex < m_iMyRowIdx + 2 && iRowIndex >= m_iMyRowIdx)
		{
			iAcutalRowIdx = iRowIndex - m_iMyRowIdx;
			bMyFlag = true;
		}
		else
		{
			break;
		}

		int iIndex = iColIndex / 5;
		if (iColIndex % 5 < 2)
		{
			int iType = bMyFlag ? m_arrMyBrick[iIndex] : m_arrDestBrick[iIndex];
			int iActualColIdx = iColIndex - iIndex * 5;
			return BRICK_STATE[iType][iAcutalRowIdx][iActualColIdx];
		}
	} while (0);

	return false;
}


//获取小方块序列中的方块状态
bool CMatchGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

	return false;
}


//获取游戏类型
SCENE_INDEX CMatchGame::GetSceneType()
{
	return SCENE_MATCH;
}


//左按下
void CMatchGame::OnLeftBtnPressed()
{
	ChangeType(DIR_LEFT, true);
}


//左释放
void CMatchGame::OnLeftBtnReleased()
{
	ChangeType(DIR_LEFT, false);
}


//右按下
void CMatchGame::OnRightBtnPressed()
{
	ChangeType(DIR_RIGHT, true);
}


//右释放
void CMatchGame::OnRightBtnReleased()
{
	ChangeType(DIR_RIGHT, false);
}


//上按下
void CMatchGame::OnUpBtnPressed()
{
	ChangeType(DIR_UP, true);
}


//上释放
void CMatchGame::OnUpBtnReleased()
{
	ChangeType(DIR_UP, false);
}


//下按下
void CMatchGame::OnDownPressed()
{
	ChangeType(DIR_DOWN, true);
}


//下释放
void CMatchGame::OnDownReleased()
{
	ChangeType(DIR_DOWN, false);
}


//Fire按下
void CMatchGame::OnFireBtnPressed()
{
	m_bConfirmMatch = true;
}


//初始化数据
void CMatchGame::InitData()
{
	//初始化4个方块
	int iTypeCount = sizeof(BRICK_STATE) / sizeof(bool) / 4;
	for (int i = 0; i < BRICK_MATCH_NUM; ++i)
	{
		m_arrDestBrick[i] = Random(0, iTypeCount);
		m_arrMyBrick[i] = Random(0, iTypeCount);

		//爆炸位置索引
		m_arrBoomIndex[i] = -1;
	}

	//目标方块所在行
	m_iDestRowIdx = m_iLevel;

	//我方方块所在行
	m_iMyRowIdx = ROW_NUM - 2;

	//时间相关
	m_fDestBrickMoveTime = 0;
	m_fWaitTime = 0;

	//游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//爆炸显示计数
	m_iShowBoomCount = 0;

	//分数增加次数计数
	m_iAddScoreCount = 0;

	//爆炸显示状态
	m_bShowBoom = true;

	//是否加速
	m_bConfirmMatch = false;

	//更新界面
	m_pGameScene->UpdateBricks();
}


//目标方块移动
bool CMatchGame::DestBricksMove( float dt )
{
	m_fDestBrickMoveTime += dt;
	if (m_fDestBrickMoveTime < BRICK_MOVE_INTERVAL - 70 * m_iSpeed)
	{
		return false;
	}

	//重置
	m_fDestBrickMoveTime = 0;

	//目标方块下降一格
	if (++m_iDestRowIdx <= m_iMyRowIdx - 2)
	{
		return true;
	}

	//更新游戏状态
	UpdateGameState();

	return true;
}


//更改方块类型
void CMatchGame::ChangeType(DIRECTION enDirection, bool bPressed)
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	if (bPressed)
	{
		PLAY_EFFECT(EFFECT_CHANGE2);
		UpdateMyBricks(enDirection);
	}
}



void CMatchGame::UpdateMyBricks(DIRECTION enDirection)
{
	const int arrRelation[DIR_MAX] = { 2, -1, 0, 1 };
	int iTypeCount = sizeof(BRICK_STATE) / sizeof(bool) / 4;

	int iBrickIndex = arrRelation[enDirection];
	if (iBrickIndex == -1)
	{
		return;
	}

	if (++m_arrMyBrick[iBrickIndex] > iTypeCount - 1)
	{
		m_arrMyBrick[iBrickIndex] = 0;
	}

	m_pGameScene->UpdateBricks(m_iMyRowIdx, 0, m_iMyRowIdx + 2, COLUMN_NUM);
}


//我方方块移动
bool CMatchGame::MyBricksMove(float dt)
{
	if (!m_bConfirmMatch)
	{
		return false;
	}

	m_fWaitTime += dt;
	if (m_fWaitTime < 30)
	{
		return false;
	}

	//重置
	m_fWaitTime = 0;

	if (--m_iMyRowIdx >= m_iDestRowIdx + 2)
	{
		return true;
	}

	//已经和目标方块重合，更新游戏状态
	UpdateGameState();
	
	return true;
}


void CMatchGame::UpdateGameState()
{
	//检查是否匹配
	int iBoomIndex = 0;
	for (int i = 0; i < BRICK_MATCH_NUM; ++i)
	{
		if (m_arrMyBrick[i] != m_arrDestBrick[i])
		{
			m_enGameState = GAMESTATE_OVER;
			PLAY_EFFECT(EFFECT_BOOM);
			m_arrBoomIndex[iBoomIndex++] = i;
		}
	}

	//如果都匹配，则加分，检查是否通过当前等级
	if (iBoomIndex == 0)
	{
		m_iScore += MACTHSUCC_ADDSCORE;
		m_pGameScene->UpdateScore(m_iScore);

		if (++m_iMatchSuccCount >= GAMEPASS_MATCHCOUNT)
		{
			m_enGameState = GAMESTATE_PASS;
		}
		else
		{
			//重新产生目标方块和我方方块
			InitData();
		}
	}
}


void CMatchGame::SaveGameData()
{
	
}