#include "PinballGame.h"


CPinballGame::CPinballGame(CGameScene* pGameScene, bool bExtraMode) : CSceneBase(pGameScene), m_bExtraMode(bExtraMode)
{
}


CPinballGame::~CPinballGame()
{
}


//初始化
void CPinballGame::Init()
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
void CPinballGame::Play(float dt)
{
	if (m_enGameState == GAMESTATE_RUNNING || m_enGameState == GAMESTATE_PAUSE)
	{
		bool bUpdateFlag = GuardMove(dt);
		bUpdateFlag = BallMove(dt) || bUpdateFlag;
		bUpdateFlag = BricksRoll(dt) || bUpdateFlag;
		if (!bUpdateFlag)
		{
			return;
		}
	}

	if (m_enGameState == GAMESTATE_PASS)
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
			m_iScore += GAMEPASS_ADDSCORE;
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

			//重置数据
			InitData();
		}
	}
	else if (m_enGameState == GAMESTATE_OVER)
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
bool CPinballGame::GetBrickState(int iRowIndex, int iColIndex)
{
	if (m_enGameState == GAMESTATE_OVER && iRowIndex >= ROW_NUM - 4)
	{
		int iEndColIdx = (m_stBallPos.m_iColIdx < 3 ? 3 : m_stBallPos.m_iColIdx);
		if (iColIndex <= iEndColIdx && iColIndex >= iEndColIdx - 3)
		{
			return m_bShowBoom && BOOM_STATE[iRowIndex - (ROW_NUM - 4)][iColIndex - (iEndColIdx - 3)];
		}
	}

	if (iRowIndex == ROW_NUM - 1 && iColIndex >= m_iGuardColIdx && iColIndex < m_iGuardColIdx + GUARD_BRICK_COUNT)
	{
		return true;
	}

	return m_arrBricks[iRowIndex][iColIndex];
}


//获取小方块序列中的方块状态
bool CPinballGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

	return false;
}


//获取游戏类型
SCENE_INDEX CPinballGame::GetSceneType()
{
	return SCENE_PINBALL;
}


//左按下
void CPinballGame::OnLeftBtnPressed()
{
	PLAY_EFFECT(EFFECT_CHANGE2);

	bLeftMoveFlag = true;
}


//左释放
void CPinballGame::OnLeftBtnReleased()
{
	bLeftMoveFlag = false;
}


//右按下
void CPinballGame::OnRightBtnPressed()
{
	PLAY_EFFECT(EFFECT_CHANGE2);

	bRightMoveFlag = true;
}


//右释放
void CPinballGame::OnRightBtnReleased()
{
	bRightMoveFlag = false;
}


//Fire按下
void CPinballGame::OnFireBtnPressed()
{
	PLAY_EFFECT(EFFECT_CHANGE2);

	if (m_enGameState == GAMESTATE_PAUSE)
	{
		m_enGameState = GAMESTATE_RUNNING;
	}

	if (m_enGameState == GAMESTATE_RUNNING)
	{
		m_bImproveSpeedFlag = true;
	}
}


void CPinballGame::OnFireBtnReleased()
{
	if (m_enGameState == GAMESTATE_RUNNING)
	{
		m_bImproveSpeedFlag = false;
	}
}


void CPinballGame::InitData()
{
	int iStartRowIdx = 2 + m_iLevel / 2;
	for (int i = 0; i < ROW_NUM; ++i)
	{
		if (i >= iStartRowIdx && i < iStartRowIdx + BRICKS_ROWCOUNT)
		{
			
			for (int j = 0; j < COLUMN_NUM / 2; ++j)
			{
				m_arrBricks[i][j] = (Random(0, 10) >= 5);
			}

			for (int j = COLUMN_NUM / 2; j < COLUMN_NUM; ++j)
			{
				m_arrBricks[i][j] = m_arrBricks[i][COLUMN_NUM - j - 1];
			}
		}
		else
		{
			for (int j = 0; j < COLUMN_NUM; ++j)
			{
				m_arrBricks[i][j] = false;
			}
		}
	}

	//初始化球每次移动的距离
	m_stBallDis.m_iRowIdx = -1;
	m_stBallDis.m_iColIdx = (Random(0, 10) >= 5 ? -1 : 1);

	//初始化左右移状态
	bLeftMoveFlag = false;
	bRightMoveFlag = false;

	//加速状态
	m_bImproveSpeedFlag = false;

	//爆炸相关
	m_bShowBoom = true;
	m_iAddScoreCount = 0;
	m_iShowBoomCount = 0;

	//附加模式
	m_fRollTime = 0;
	m_bLeftRollFlag = (Random(0, 10) >= 5);

	//挡板的位置
	m_iGuardColIdx = (COLUMN_NUM - GUARD_BRICK_COUNT) / 2;

	//球位置
	m_stBallPos.m_iRowIdx = ROW_NUM - 2;
	m_stBallPos.m_iColIdx = m_iGuardColIdx + (GUARD_BRICK_COUNT - 1) / 2;
	m_arrBricks[ROW_NUM - 2][m_iGuardColIdx + (GUARD_BRICK_COUNT - 1) / 2] = true;

	//游戏状态
	m_enGameState = GAMESTATE_PAUSE;

	//时间相关
	m_fWaitTime = 0;
	m_fBtnCheckTime = 0;
}

bool CPinballGame::BallMove( float dt )
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return false;
	}

	//时间更新
	m_fWaitTime += dt;
	if (m_fWaitTime < (BALL_MOVE_INTERVAL - 8 * m_iSpeed) * (m_bImproveSpeedFlag ? 0.5f : 1))
	{
		return false;
	}
	//重置
	m_fWaitTime = 0;

	//检查游戏结束
	if (m_stBallPos.m_iRowIdx >= ROW_NUM - 1)
	{
		m_enGameState = GAMESTATE_OVER;
		PLAY_EFFECT(EFFECT_BOOM);
		return true;
	}

	bool bNextPosValidFlag = true;
	POSITION stNextPos = m_stBallPos + m_stBallDis;

	//位置检查
	do 
	{
		if (stNextPos.m_iRowIdx < 0)
		{
			m_stBallDis.m_iRowIdx *= -1;

			//是否需要反转列方向
			if (stNextPos.m_iColIdx < 0 || stNextPos.m_iColIdx >= COLUMN_NUM)
			{
				m_stBallDis.m_iColIdx *= -1;
			}

			PLAY_EFFECT(EFFECT_WALL);

			//下一个位置无效
			bNextPosValidFlag = false;
			break;
		}

		if (stNextPos.m_iColIdx < 0 || stNextPos.m_iColIdx >= COLUMN_NUM)
		{
			bNextPosValidFlag = false;

			if (m_arrBricks[stNextPos.m_iRowIdx][m_stBallPos.m_iColIdx])
			{
				m_arrBricks[stNextPos.m_iRowIdx][m_stBallPos.m_iColIdx] = false;

				//方向变更
				m_stBallDis.m_iRowIdx *= -1;

				//加分
				AddScore(10);
			}
			else if (m_arrBricks[stNextPos.m_iRowIdx][m_stBallPos.m_iColIdx - m_stBallDis.m_iColIdx])
			{
				m_arrBricks[stNextPos.m_iRowIdx][m_stBallPos.m_iColIdx - m_stBallDis.m_iColIdx] = false;

				//方向变更
				m_stBallDis.m_iRowIdx *= -1;

				//加分
				AddScore(10);
			}
			else
			{
				//音效
				PLAY_EFFECT(EFFECT_WALL);
			}

			//列方向反转
			m_stBallDis.m_iColIdx *= -1;

			break;
		}

		if (m_arrBricks[stNextPos.m_iRowIdx][m_stBallPos.m_iColIdx]
			&& m_arrBricks[m_stBallPos.m_iRowIdx][stNextPos.m_iColIdx])
		{
			m_arrBricks[stNextPos.m_iRowIdx][m_stBallPos.m_iColIdx] = false;
			m_arrBricks[m_stBallPos.m_iRowIdx][stNextPos.m_iColIdx] = false;

			//加分
			AddScore(20);

			//方向变更
			m_stBallDis.m_iColIdx *= -1;
			m_stBallDis.m_iRowIdx *= -1;

			//下一个位置无效
			bNextPosValidFlag = false;
			break;
		}
		else if (m_arrBricks[stNextPos.m_iRowIdx][stNextPos.m_iColIdx])
		{
			m_arrBricks[stNextPos.m_iRowIdx][stNextPos.m_iColIdx] = false;

			//加分
			AddScore(10);

			//方向变更
			m_stBallDis.m_iColIdx *= -1;
			m_stBallDis.m_iRowIdx *= -1;

			//下一个位置无效
			bNextPosValidFlag = false;
			break;
		}
		else if (m_arrBricks[stNextPos.m_iRowIdx][m_stBallPos.m_iColIdx]
			&& !m_arrBricks[m_stBallPos.m_iRowIdx][stNextPos.m_iColIdx])
		{
			m_arrBricks[stNextPos.m_iRowIdx][m_stBallPos.m_iColIdx] = false;

			//加分
			AddScore(10);

			//方向变更
			m_stBallDis.m_iRowIdx *= -1;

			//下一个位置无效
			bNextPosValidFlag = false;
			break;
		}
		else if (!m_arrBricks[stNextPos.m_iRowIdx][m_stBallPos.m_iColIdx]
			&& m_arrBricks[m_stBallPos.m_iRowIdx][stNextPos.m_iColIdx])
		{
			m_arrBricks[m_stBallPos.m_iRowIdx][stNextPos.m_iColIdx] = false;

			//加分
			AddScore(10);

			//方向变更
			m_stBallDis.m_iColIdx *= -1;

			//下一个位置无效
			bNextPosValidFlag = false;
			break;
		}

	} while (0);

	//更新位置
	m_arrBricks[m_stBallPos.m_iRowIdx][m_stBallPos.m_iColIdx] = false;
	m_stBallPos = bNextPosValidFlag ? stNextPos : (m_stBallPos + m_stBallDis);
	m_arrBricks[m_stBallPos.m_iRowIdx][m_stBallPos.m_iColIdx] = true;

	//挡板是否挡住球
	if (m_stBallPos.m_iRowIdx == ROW_NUM - 2 
		&& ((m_stBallPos.m_iColIdx > m_iGuardColIdx - 1 && m_stBallPos.m_iColIdx < m_iGuardColIdx + GUARD_BRICK_COUNT)
		|| (m_stBallPos.m_iColIdx == m_iGuardColIdx - 1 && m_stBallDis.m_iColIdx > 0 && m_stBallDis.m_iRowIdx > 0)	//往右下方向移动
		|| (m_stBallPos.m_iColIdx == m_iGuardColIdx + GUARD_BRICK_COUNT && m_stBallDis.m_iColIdx < 0 && m_stBallDis.m_iRowIdx > 0))	//往左下方向移动
		)
	{
		//音效
		PLAY_EFFECT(EFFECT_WALL);

		//如果是往右下/左下方向移动，则直接反向
		if ((m_stBallPos.m_iColIdx == m_iGuardColIdx - 1 && m_stBallDis.m_iColIdx > 0 && m_stBallDis.m_iRowIdx > 0)
			|| (m_stBallPos.m_iColIdx == m_iGuardColIdx + GUARD_BRICK_COUNT && m_stBallDis.m_iColIdx < 0 && m_stBallDis.m_iRowIdx > 0))
		{
			m_stBallDis.m_iColIdx *= -1;
			m_stBallDis.m_iRowIdx *= -1;
		}
		else
		{
			m_stBallDis.m_iRowIdx *= -1;
		}

		return true;
	}

	//检查是否通过当前等级
	if (CheckGamePass())
	{
		m_enGameState = GAMESTATE_PASS;
	}

	return true;
}


bool CPinballGame::GuardMove(float dt)
{
	//时间更新
	m_fBtnCheckTime += dt;
	if (m_fBtnCheckTime < BTN_CHECK_INTERVAL)
	{
		return false;
	}

	m_fBtnCheckTime = 0;

	//按钮检查
	if (bLeftMoveFlag && m_iGuardColIdx > 0)
	{
		//需要先检查小球所在位置，避免小球正好在更新后的挡板位置列范围内
		if (m_enGameState == GAMESTATE_PAUSE ||
			(m_stBallPos.m_iRowIdx == ROW_NUM - 2 && m_stBallPos.m_iColIdx >= m_iGuardColIdx &&
			m_stBallPos.m_iColIdx < m_iGuardColIdx + GUARD_BRICK_COUNT))
		{
			m_arrBricks[m_stBallPos.m_iRowIdx][m_stBallPos.m_iColIdx] = false;
			--m_stBallPos.m_iColIdx;
			m_arrBricks[m_stBallPos.m_iRowIdx][m_stBallPos.m_iColIdx] = true;
		}

		--m_iGuardColIdx;
	}
	else if (bRightMoveFlag && m_iGuardColIdx < COLUMN_NUM - GUARD_BRICK_COUNT)
	{
		if (m_enGameState == GAMESTATE_PAUSE ||
			(m_stBallPos.m_iRowIdx == ROW_NUM - 2 && m_stBallPos.m_iColIdx >= m_iGuardColIdx &&
			m_stBallPos.m_iColIdx < m_iGuardColIdx + GUARD_BRICK_COUNT))
		{
			m_arrBricks[m_stBallPos.m_iRowIdx][m_stBallPos.m_iColIdx] = false;
			++m_stBallPos.m_iColIdx;
			m_arrBricks[m_stBallPos.m_iRowIdx][m_stBallPos.m_iColIdx] = true;
		}

		++m_iGuardColIdx;
	}

	return true;
}


void CPinballGame::AddScore(int iScore)
{
	//加分
	m_iScore += iScore;
	m_pGameScene->UpdateScore(m_iScore);

	//音效
	PLAY_EFFECT(EFFECT_ADD);
}


bool CPinballGame::CheckGamePass()
{
	int iStartRowIdx = 2 + m_iLevel / 2;
	for (int i = iStartRowIdx; i < iStartRowIdx + BRICKS_ROWCOUNT; ++i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			if (m_arrBricks[i][j])
			{
				return false;
			}
		}
	}

	return true;
}


void CPinballGame::SaveGameData()
{
	
}


bool CPinballGame::BricksRoll(float dt)
{
	if (!m_bExtraMode || (m_bExtraMode && m_enGameState != GAMESTATE_RUNNING))
	{
		return false;
	}

	m_fRollTime += dt;
	if (m_fRollTime < BRICK_BASE_MOVE_INTERVAL - 40 * m_iSpeed)
	{
		return false;
	}

	m_fRollTime = 0;

	int iStartRowIdx = 2 + m_iLevel / 2;
	if (m_bLeftRollFlag)
	{
		for (int iColIdx = COLUMN_NUM - 1; iColIdx > 0; --iColIdx)
		{
			for (int iRowIdx = iStartRowIdx; iRowIdx < iStartRowIdx + BRICKS_ROWCOUNT; ++iRowIdx)
			{
				m_arrBricks[iRowIdx][iColIdx] = m_arrBricks[iRowIdx][iColIdx - 1];
			}
		}
	}
	else
	{
		for (int iColIdx = 0; iColIdx < COLUMN_NUM - 1; ++iColIdx)
		{
			for (int iRowIdx = iStartRowIdx; iRowIdx < iStartRowIdx + BRICKS_ROWCOUNT; ++iRowIdx)
			{
				m_arrBricks[iRowIdx][iColIdx] = m_arrBricks[iRowIdx][iColIdx + 1];
			}
		}
	}
	

	//生成新的一列
	int iGenColIdx = (m_bLeftRollFlag ? 0 : (COLUMN_NUM - 1));
	for (int iRowIdx = iStartRowIdx; iRowIdx < iStartRowIdx + BRICKS_ROWCOUNT; ++iRowIdx)
	{
		m_arrBricks[iRowIdx][iGenColIdx] = (Random(0, 10) >= 5);
	}

	return true;
}
