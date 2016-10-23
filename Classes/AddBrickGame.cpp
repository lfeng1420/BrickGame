#include "AddBrickGame.h"

const int PASS_SCORE[SPEED_MAX + 1] = {2000, 4000, 6000, 8000, 10000, 13000, 16000, 20000, 24000, 28000, 35000};

CAddBrickGame::CAddBrickGame(CGameScene* pScene, bool bSpecialMode/* = false*/) : CSceneBase(pScene), 
									m_iSpeed(0), m_iLevel(0), m_iScore(0), m_bSpecialMode(bSpecialMode)
{
}


CAddBrickGame::~CAddBrickGame()
{
}


void CAddBrickGame::Init()
{
	//获取在选择游戏界面设置的速度和等级
	m_iSpeed = GET_INTVALUE("SPEED", 0);
	m_iLevel = GET_INTVALUE("LEVEL", 0);
	m_iLife = GET_INTVALUE("LIFE", 4);

	//初始化当前分数
	m_iScore = 0;

	//更新等级和速度
	m_pGameScene->UpdateLevel(m_iLevel);
	m_pGameScene->UpdateSpeed(m_iSpeed);
	m_pGameScene->UpdateSmallBricks();
	m_pGameScene->UpdateScore(m_iScore, false);

	//初始化数据
	initData();
}


void CAddBrickGame::Play(float dt)
{
	if (m_enGameState == GAMESTATE_OVER)
	{
		//时间更新
		m_fWaitTime += dt;
		if (m_fWaitTime < GAMEOVER_WAIT_INTERVAL)
		{
			return;
		}
		m_fWaitTime = 0;

		//设置剩余生命
		if (--m_iLife <= 0)
		{
			m_pGameScene->RunScene(SCENE_GAMEOVER);
			return;
		}

		//更新显示生命
		m_pGameScene->UpdateSmallBricks();

		//重置数据
		initData();
	}

	if (m_enGameState == GAMESTATE_PASS)
	{
		//时间更新
		m_fWaitTime += dt;
		if (m_fWaitTime < GAMEPASS_WAIT_INTERVAL)
		{
			return;
		}
		m_fWaitTime = 0;

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
		initData();
	}

	if (m_enGameState == GAMESTATE_RUNNING)
	{
		selfMove(dt);

		bool bRefreshFlag = false;
		if (bulletsMove(dt))
		{
			bRefreshFlag = true;
		}

		if (fireBricks(dt))
		{
			bRefreshFlag = true;
		}

		if (bricksMoveDown(dt))
		{
			bRefreshFlag = true;
		}

		if (!bRefreshFlag)
		{
			return;
		}
	}

	//更新游戏状态
	updateGameState();


	m_pGameScene->UpdateBricks();
}


bool CAddBrickGame::GetBrickState(int iRowIndex, int iColIndex)
{
	//我方所在区域
	int nSelfRowIdx = m_stSelfPos.m_iRowIdx;
	int nSelfColIdx = m_stSelfPos.m_iColIdx;
	if ((iRowIndex == nSelfRowIdx && iColIndex >= nSelfColIdx - 1 && iColIndex <= nSelfColIdx + 1)
		|| (iRowIndex == nSelfRowIdx - 1 && iColIndex == nSelfColIdx))
	{
		return true;
	}

	//子弹
	FOR_EACH_CONTAINER (TLIST_POS, m_lsBullets, itBullet)
	{
		if (itBullet->m_iRowIdx == iRowIndex && itBullet->m_iColIdx == iColIndex)
		{
			return true;
		}
	}

	return m_arrBrick[iRowIndex][iColIndex];
}


bool CAddBrickGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

	return false;
}


SCENE_INDEX CAddBrickGame::GetSceneType()
{
	return SCENE_ADDBRICK;
}


void CAddBrickGame::OnLeftBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnPressFlag[VBTN_LEFT] = true;
}


void CAddBrickGame::OnLeftBtnReleased()
{
	m_arrBtnPressFlag[VBTN_LEFT] = false;
}


void CAddBrickGame::OnRightBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnPressFlag[VBTN_RIGHT] = true;
}


void CAddBrickGame::OnRightBtnReleased()
{
	m_arrBtnPressFlag[VBTN_RIGHT] = false;
}


void CAddBrickGame::OnFireBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnPressFlag[VBTN_FIRE] = true;
	fireBricks(FIRE_CHECK_INTERVAL * 0.5f);
}


void CAddBrickGame::OnFireBtnReleased()
{
	m_arrBtnPressFlag[VBTN_FIRE] = false;
}


void CAddBrickGame::initData()
{
	//初始化
	for (int nRowIdx = 0; nRowIdx < ROW_NUM; ++nRowIdx)
	{
		//确定一行的空位
		int nEmptyColIdx = Random(0, COLUMN_NUM);
		for (int nColIdx = 0; nColIdx < COLUMN_NUM; ++nColIdx)
		{
			if (nEmptyColIdx == nColIdx || nRowIdx >= m_iLevel)
			{
				m_arrBrick[nRowIdx][nColIdx] = false;
			}
			else
			{
				m_arrBrick[nRowIdx][nColIdx] = randState();
			}
		}
	}

	//初始化我方所在位置
	m_stSelfPos.m_iColIdx = COLUMN_NUM / 2;
	m_stSelfPos.m_iRowIdx = ROW_NUM - 1;

	//初始化按钮按下标记
	for (int nIdx = 0; nIdx < VBTN_MAX; ++nIdx)
	{
		m_arrBtnPressFlag[nIdx] = false;
	}

	//游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//时间相关
	m_fBulletMoveTime = 0;
	m_fFireBtnTime = 0;
	m_fSelfMoveTime = 0;
	m_fWaitTime = 0;

	//清空子弹
	m_lsBullets.clear();
}


bool CAddBrickGame::bricksMoveDown(float dt)
{
	m_fWaitTime += dt;
	if (m_fWaitTime < BRICKS_MOVE_INTERVAL - 150 * m_iSpeed)
	{
		return false;
	}

	m_fWaitTime = 0;

	//下移一行方块
	for (int nRowIdx = ROW_NUM - 1; nRowIdx > 0; --nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < COLUMN_NUM; ++nColIdx)
		{
			m_arrBrick[nRowIdx][nColIdx] = m_arrBrick[nRowIdx - 1][nColIdx];
			
			//如果下一个位置正好是某个子弹所在位置，则删除子弹，设置该位置状态
			int nNextRowIdx = nRowIdx + 1;
			if (nNextRowIdx < ROW_NUM && m_arrBrick[nRowIdx][nColIdx])
			{
				FOR_EACH_CONTAINER(TLIST_POS, m_lsBullets, itBullet)
				{
					if (nNextRowIdx == itBullet->m_iRowIdx && nColIdx == itBullet->m_iColIdx)
					{
						//删除这个子弹
						m_lsBullets.erase(itBullet);
						
						//设置状态并检查消行
						m_arrBrick[nNextRowIdx][nColIdx] = true;
						deleteLine(nNextRowIdx);
						break;
					}
				}
			}
		}
	}

	//生成第一行方块
	int nRandColIdx = Random(0, COLUMN_NUM);
	for (int nColIdx = 0; nColIdx < COLUMN_NUM; ++nColIdx)
	{
		m_arrBrick[0][nColIdx] = (nRandColIdx == nColIdx ? false : randState());
	}

	return true;
}


bool CAddBrickGame::randState()
{
	return Random(1, 20) >= 10;
}


void CAddBrickGame::updateGameState()
{
	//检查自己区域内是否有方块
	int nSelfRowIdx = m_stSelfPos.m_iRowIdx;
	int nSelfColIdx = m_stSelfPos.m_iColIdx;
	if (m_arrBrick[nSelfRowIdx - 1][nSelfColIdx]
		|| m_arrBrick[nSelfRowIdx][nSelfColIdx] 
		|| (nSelfColIdx > 0 && m_arrBrick[nSelfRowIdx][nSelfColIdx - 1])
		|| (nSelfColIdx + 1 < COLUMN_NUM && m_arrBrick[nSelfRowIdx][nSelfColIdx + 1]))
	{
		m_enGameState = GAMESTATE_OVER;
		return;
	}

	//检查是否有方块行到达倒数第二行
	for (int nColIdx = 0; nColIdx < COLUMN_NUM; ++nColIdx)
	{
		if (m_arrBrick[ROW_NUM - 1][nColIdx])
		{
			m_enGameState = GAMESTATE_OVER;
			return;
		}
	}

	//检查分数
	if (!m_bSpecialMode && m_iScore >= PASS_SCORE[m_iSpeed])
	{
		m_enGameState = GAMESTATE_PASS;
		return;
	}
}


bool CAddBrickGame::fireBricks(float dt)
{
	if (!m_arrBtnPressFlag[VBTN_FIRE])
	{
		m_fFireBtnTime = 0;
		return false;
	}

	m_fFireBtnTime += dt;
	if (m_fFireBtnTime < FIRE_CHECK_INTERVAL - 5 * m_iSpeed)
	{
		return false;
	}

	m_fFireBtnTime = 0;

	//如果正好对应列上一行有个方块，则设置状态并返回
	int nBulletRowIdx = m_stSelfPos.m_iRowIdx - 2;
	int nBulletColIdx = m_stSelfPos.m_iColIdx;
	if (m_arrBrick[nBulletRowIdx - 1][nBulletColIdx])
	{
		m_arrBrick[nBulletRowIdx][nBulletColIdx] = true;
		m_pGameScene->UpdateBrick(nBulletRowIdx, nBulletRowIdx, false, true);

		//检查消行
		deleteLine(nBulletRowIdx);

		return false;
	}

	//添加子弹
	m_lsBullets.push_back(POSITION(nBulletRowIdx, nBulletColIdx));
	return true;
}


bool CAddBrickGame::selfMove(float dt)
{
	if (!m_arrBtnPressFlag[VBTN_LEFT] && !m_arrBtnPressFlag[VBTN_RIGHT])
	{
		m_fSelfMoveTime = 0;
		return false;
	}

	//时间更新
	m_fSelfMoveTime += dt;
	if (m_fSelfMoveTime < MOVE_CHECK_INTERVAL)
	{
		return false;
	}

	m_fSelfMoveTime = 0;

	//按钮检查
	if (m_arrBtnPressFlag[VBTN_LEFT] && m_stSelfPos.m_iColIdx > 0)
	{
		if (m_arrBrick[m_stSelfPos.m_iRowIdx - 1][m_stSelfPos.m_iColIdx - 1])
		{
			return false;
		}

		--m_stSelfPos.m_iColIdx;
	}
	else if (m_arrBtnPressFlag[VBTN_RIGHT] && m_stSelfPos.m_iColIdx < COLUMN_NUM - 1)
	{
		if (m_arrBrick[m_stSelfPos.m_iRowIdx][m_stSelfPos.m_iColIdx + 1])
		{
			return false;
		}

		++m_stSelfPos.m_iColIdx;
	}

	//刷新底下两行
	m_pGameScene->UpdateBricks(ROW_NUM - 2, 0);

	return true;
}


bool CAddBrickGame::bulletsMove(float dt)
{
	m_fBulletMoveTime += dt;
	if (m_fBulletMoveTime < BULLET_MOVE_INTERVAL - 3 * m_iSpeed)
	{
		return false;
	}

	m_fBulletMoveTime = 0;

	//遍历所有子弹
	for (TLIST_POS_ITER itBullet = m_lsBullets.begin(); itBullet != m_lsBullets.end(); )
	{
		int nRowIdx = itBullet->m_iRowIdx;
		int nColIdx = itBullet->m_iColIdx;
		int nNextRowIdx = nRowIdx - 1;

		if (nNextRowIdx == 0 || (nNextRowIdx - 1 >= 0 && m_arrBrick[nNextRowIdx - 1][nColIdx]))
		{
			//设置状态并检查消行
			m_arrBrick[nNextRowIdx][nColIdx] = true;
			deleteLine(nNextRowIdx);

			itBullet = m_lsBullets.erase(itBullet);
		}
		else
		{
			//移动
			--itBullet->m_iRowIdx;
			++itBullet;
		}
	}

	return true;
}


void CAddBrickGame::deleteLine(int nRowIdx)
{
	for (int nIdx = 0; nIdx < COLUMN_NUM; ++nIdx)
	{
		if (!m_arrBrick[nRowIdx][nIdx])
		{
			return;
		}
	}

	//隐藏之后所有方块
	int nBrickCount = 0;
	for (; nRowIdx < ROW_NUM; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < COLUMN_NUM; ++nColIdx)
		{
			if (m_arrBrick[nRowIdx][nColIdx])
			{
				m_arrBrick[nRowIdx][nColIdx] = false;
				++nBrickCount;
			}
		}
	}

	m_iScore += nBrickCount * 10;
	m_pGameScene->UpdateScore(m_iScore, true);
}

