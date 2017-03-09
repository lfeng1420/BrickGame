#include "HitBrickGame.h"

const int PASS_SCORE[SPEED_MAX + 1] = {5000, 13000, 23000, 35000, 50000, 65000, 80000, 100000, 120000, 140000, 160000};

CHitBrickGame::CHitBrickGame(CGameScene* pScene, bool bSpecialMode/* = false*/) : CSceneBase(pScene), 
									m_iSpeed(0), m_iLevel(0), m_iScore(0), m_bSpecialMode(bSpecialMode)
{
}


CHitBrickGame::~CHitBrickGame()
{
}


void CHitBrickGame::Init()
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


void CHitBrickGame::Play(float dt)
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


bool CHitBrickGame::GetBrickState(int iRowIndex, int iColIndex)
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


bool CHitBrickGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

	return false;
}


SCENE_INDEX CHitBrickGame::GetSceneType()
{
	return SCENE_HITBRICK;
}


void CHitBrickGame::OnLeftBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnPressFlag[VBTN_LEFT] = true;
	selfMove(MOVE_CHECK_INTERVAL * 0.4f);
}


void CHitBrickGame::OnLeftBtnReleased()
{
	m_arrBtnPressFlag[VBTN_LEFT] = false;
}


void CHitBrickGame::OnRightBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnPressFlag[VBTN_RIGHT] = true;
	selfMove(MOVE_CHECK_INTERVAL * 0.4f);
}


void CHitBrickGame::OnRightBtnReleased()
{
	m_arrBtnPressFlag[VBTN_RIGHT] = false;
}


void CHitBrickGame::OnFireBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnPressFlag[VBTN_FIRE] = true;
	fireBricks(FIRE_CHECK_INTERVAL * 0.5f);
}


void CHitBrickGame::OnFireBtnReleased()
{
	m_arrBtnPressFlag[VBTN_FIRE] = false;
}


void CHitBrickGame::initData()
{
	//初始化
	for (int nRowIdx = 0; nRowIdx < ROW_NUM; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < COLUMN_NUM; ++nColIdx)
		{
			m_arrBrick[nRowIdx][nColIdx] = ((nRowIdx < m_iLevel) ? randState() : false);
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


bool CHitBrickGame::bricksMoveDown(float dt)
{
	m_fWaitTime += dt;
	if (m_fWaitTime < BRICKS_MOVE_INTERVAL - 120 * m_iSpeed)
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
			
			//如果正好和子弹重叠，则设置隐藏，删除子弹
			if (m_arrBrick[nRowIdx][nColIdx])
			{
				FOR_EACH_CONTAINER(TLIST_POS, m_lsBullets, itBullet)
				{
					if (nRowIdx == itBullet->m_iRowIdx && nColIdx == itBullet->m_iColIdx)
					{
						//加分
						addScore();

						//删除这个子弹
						m_lsBullets.erase(itBullet);
						m_arrBrick[nRowIdx][nColIdx] = false;

						break;
					}
				}
			}
		}
	}

	//生成第一行方块
	for (int nColIdx = 0; nColIdx < COLUMN_NUM; ++nColIdx)
	{
		m_arrBrick[0][nColIdx] = randState();
	}

	return true;
}


bool CHitBrickGame::randState()
{
	return Random(1, 20) >= 10;
}


void CHitBrickGame::updateGameState()
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

		//振动
		m_pGameScene->OnLongVibrate();

		return;
	}

	//检查是否有方块行到达倒数第二行
	for (int nColIdx = 0; nColIdx < COLUMN_NUM; ++nColIdx)
	{
		if (m_arrBrick[ROW_NUM - 1][nColIdx])
		{
			m_enGameState = GAMESTATE_OVER;

			//振动
			m_pGameScene->OnLongVibrate();

			return;
		}
	}

	//检查分数
	if (!m_bSpecialMode && m_iScore >= PASS_SCORE[m_iSpeed])
	{
		m_enGameState = GAMESTATE_PASS;

		//振动
		m_pGameScene->OnLongVibrate();

		return;
	}
}


bool CHitBrickGame::fireBricks(float dt)
{
	if (!m_arrBtnPressFlag[VBTN_FIRE])
	{
		m_fFireBtnTime = 0;
		return false;
	}

	m_fFireBtnTime += dt;
	if (m_fFireBtnTime < FIRE_CHECK_INTERVAL - 7 * m_iSpeed)
	{
		return false;
	}

	m_fFireBtnTime = 0;

	//如果正好该位置有个方块，则隐藏并返回
	int nBulletRowIdx = m_stSelfPos.m_iRowIdx - 2;
	int nBulletColIdx = m_stSelfPos.m_iColIdx;
	if (m_arrBrick[nBulletRowIdx][nBulletColIdx])
	{
		addScore();
		m_arrBrick[nBulletRowIdx][nBulletColIdx] = false;
		m_pGameScene->UpdateBrick(nBulletRowIdx, nBulletRowIdx, false, false);
		return false;
	}

	//添加子弹
	m_lsBullets.push_back(POSITION(nBulletRowIdx, nBulletColIdx));
	return true;
}


bool CHitBrickGame::selfMove(float dt)
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


bool CHitBrickGame::bulletsMove(float dt)
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
		//检查位置
		if (checkBullet(*itBullet))
		{
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


bool CHitBrickGame::checkBullet(const POSITION& stPos)
{
	if (stPos.m_iRowIdx - 1 < 0)
	{
		return true;
	}

	if (m_arrBrick[stPos.m_iRowIdx - 1][stPos.m_iColIdx])
	{
		addScore();
		m_arrBrick[stPos.m_iRowIdx - 1][stPos.m_iColIdx] = false;
		return true;
	}

	return false;
}


void CHitBrickGame::addScore()
{
	m_iScore += 10;
	m_pGameScene->UpdateScore(m_iScore, true);
}

