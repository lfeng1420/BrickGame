#include "TankGame.h"


const int CORNER_POS[8] = 
{
	1, 1, 
	1, COLUMN_NUM - 2,
	ROW_NUM - 2, 1,
	ROW_NUM - 2, COLUMN_NUM - 2,
};

const int TANK_POS_LIST[DIR_MAX][12] =
{
	{ -1, -1, -1,  0,  0,  0,  0,  1,  1, -1,  1,  0 },
	{ -1, -1, -1,  1,  0, -1,  0,  0,  0,  1,  1,  0 },
	{ -1,  0, -1,  1,  0, -1,  0,  0,  1,  0,  1,  1 },
	{ -1,  0,  0, -1,  0,  0,  0,  1,  1, -1,  1,  1 },
};

// 坦克Boss
/*
	100000001
	111111111
	011111110
	010111010
	010111010
	011111110
	100010001
	000010000
*/

const static int BOSS_ROW_COUNT = 8;
const static int BOSS_COL_COUNT = 9;

const bool BOSS_SHAPE_STATE[BOSS_ROW_COUNT][BOSS_COL_COUNT] =
{
	true, false, false, false, false, false, false, false, true,
	true, true, true, true, true, true, true, true, true,
	false, true, true, true, true, true, true, true, false,
	false, true, false, true, true, true, false, true, false,
	false, true, false, true, true, true, false, true, false,
	false, true, true, true, true, true, true, true, false,
	true, false, false, false, true, false, false, false, true,
	false, false, false, false, true, false, false, false, false,
};

CTankGame::CTankGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CTankGame::~CTankGame()
{
}


//初始化
void CTankGame::Init()
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
void CTankGame::Play(float dt)
{
	//状态切换， 如果BOSS阶段没有开启，此时所有坦克已经死亡，则切换到暂停状态
	if (!m_bBossFlag && m_enGameState == GAMESTATE_RUNNING && CheckAllTankDead())
	{
		//切换到暂停状态
		m_enGameState = GAMESTATE_PAUSE;

		//重置时间
		m_fWaitRefreshTime = 0;

		//清除所有子弹
		for (int i = 0; i < BULLET_MAXNUM; ++i)
		{
			m_arrBullet[i].m_bValid = false;
		}
	}

	//刷新标记显示状态
	if (m_enGameState != GAMESTATE_OVER)
	{
		SelfFlagFlash(dt);
	}

	if (GAMESTATE_OVER == m_enGameState)
	{
		//时间更新
		m_fWaitRefreshTime += dt;
		if (m_fWaitRefreshTime < BOOM_REFRESH_INTERVAL)
		{
			return;
		}
		m_fWaitRefreshTime = 0;

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
	else if (GAMESTATE_PASS == m_enGameState)
	{
		//时间更新
		m_fWaitRefreshTime += dt;
		if (m_fWaitRefreshTime < GAMEPASS_REFRESH_INTERVAL)
		{
			return;
		}
		m_fWaitRefreshTime = 0;
		
		if (m_iAddScoreCount < GAMEPASS_ADDCOUNT)
		{
			++m_iAddScoreCount;
			m_iScore += TANK_KILL_ADD_SCORE;
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
	else if (GAMESTATE_RUNNING == m_enGameState)
	{
		//子弹移动
		bool bRefreshFlag = BulletMove(dt);

		//我方坦克移动
		bRefreshFlag = SelfTankMove(dt) || bRefreshFlag;

		//其他坦克移动
		bRefreshFlag = TankMove(dt) || bRefreshFlag;

		//坦克射击
		bRefreshFlag = TankFire(dt) || bRefreshFlag;

		//检查子弹是否击中坦克
		BulletShoot();

		//创建坦克
		bRefreshFlag = CreateTank(dt) || bRefreshFlag;

		if (!bRefreshFlag)
		{
			return;
		}
	}
	else if (GAMESTATE_PAUSE == m_enGameState)
	{
		bool bDoneFlag = false;
		if (!WaitToMoveBottomCenter(dt, bDoneFlag))
		{
			return;
		}

		if (bDoneFlag)
		{
			//移动完成，创建Boss，切换状态
			m_bBossFlag = true;
			m_stBoss.m_bDead = false;
			
			//随机发射子弹时间间隔
			m_stBoss.m_fFireMaxTime = Random(BOSS_FIRE_MIN_INTERVAL / 10 - m_iSpeed, BOSS_FIRE_MAX_INTERVAL / 10 - 3 * m_iSpeed) * 10;
			m_stBoss.m_fFireWaitTime = 0;
			m_stBoss.m_iMaxStep = 12 + m_iSpeed;	//击中最大次数
			m_stBoss.m_iCurStep = 0;
			
			//方向只有左右
			int iDirection = Random(DIR_MIN, DIR_MAX);
			if (iDirection <= DIR_DOWN)
			{
				m_stBoss.m_iDirection = DIR_RIGHT;
			}
			else
			{
				m_stBoss.m_iDirection = DIR_LEFT;
			}

			m_enGameState = GAMESTATE_RUNNING;
		}
	}

	m_pGameScene->UpdateBricks();
}


//获取当前Brick状态
bool CTankGame::GetBrickState(int iRowIndex, int iColIndex)
{
	bool bState;
	if (GAMESTATE_OVER == m_enGameState && ShowBoom(iRowIndex, iColIndex, bState))
	{
		return bState;
	}


	POSITION stTargetPos;
	stTargetPos.m_iRowIdx = iRowIndex;
	stTargetPos.m_iColIdx = iColIndex;

	//获取标记所在位置
	POSITION stFlagPos;
	GetFlagPos(stFlagPos);
	if (stTargetPos == stFlagPos)
	{
		//log("[Show] m_bSelfFlagVisible=%d", m_bSelfFlagVisible);
		return m_bSelfFlagVisible;
	}

	//自己
	if (DrawTank(m_stSelfTank.m_stPos, m_bBossFlag ? DIR_UP : m_stSelfTank.m_iDirection, stTargetPos))
	{
		return true;
	}

	if (!m_bBossFlag)
	{
		//其他坦克
		for (int i = 0; i < TANK_MAXNUM; ++i)
		{
			if (m_arrTank[i].m_bDead)
			{
				continue;
			}

			const TANK_DATA& stData = m_arrTank[i];
			if (DrawTank(stData.m_stPos, stData.m_iDirection, stTargetPos))
			{
				return true;
			}
		}
	}
	else
	{
		//BOSS阶段且BOSS未死亡
		if (!m_stBoss.m_bDead)
		{
			int iBossColIdx = m_stBoss.m_stPos.m_iColIdx;
			if (iRowIndex >= 0 && iRowIndex < BOSS_ROW_COUNT
				&& iColIndex >= iBossColIdx - BOSS_COL_COUNT / 2 && iColIndex <= iBossColIdx + BOSS_COL_COUNT / 2)
			{
				return BOSS_SHAPE_STATE[iRowIndex][iColIndex - (iBossColIdx - BOSS_COL_COUNT / 2)];
			}
		}
	}

	//子弹
	if (m_enGameState != GAMESTATE_PAUSE)
	{
		for (int i = 0; i < BULLET_MAXNUM; ++i)
		{
			BULLET_DATA& refData = m_arrBullet[i];
			if (refData.m_bValid && stTargetPos == refData.m_stPos)
			{
				return true;
			}
		}
	}

	return false;
}

//获取小方块序列中的方块状态
bool CTankGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

	return false;
}

//获取游戏类型
SCENE_INDEX CTankGame::GetSceneType()
{
	return SCENE_TANK;
}


//初始化数据
void CTankGame::InitData()
{
	//初始化时间相关
	m_fTankMoveTime = 0;
	m_fTankCreateTime = 0;
	m_fSelfMoveTime = 0;
	m_fWaitRefreshTime = 0;
	m_lfCurTime = -1;

	//爆炸显示计数
	m_iShowBoomCount = 0;
	//分数增加次数计数
	m_iAddScoreCount = 0;

	//爆炸显示状态
	m_bShowBoom = true;

	//标记显示状态
	m_bSelfFlagVisible = false;
	m_lfSelfFlashTime = 0;

	//坦克创建数量初始化
	m_iTankCreateCount = 0;

	//游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//发射状态
	m_bFireState = false;

	//初始化自己
	m_stSelfTank.m_bDead = false;
	m_stSelfTank.m_iCamp = CAMP_A;
	m_stSelfTank.m_fFireWaitTime = 0;
	m_stSelfTank.m_fFireMaxTime = TANK_SELF_FIRE_TIME - 20 * m_iSpeed;
	m_stSelfTank.m_stPos.m_iRowIdx = ROW_NUM / 2 - 1;
	m_stSelfTank.m_stPos.m_iColIdx = COLUMN_NUM / 2 - 1;
	m_stSelfTank.m_iDirection = DIR_UP;

	//初始化其他坦克
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		//坦克
		m_arrTank[i].m_bDead = true;
		m_arrTank[i].m_iCamp = CAMP_B;
		m_arrTank[i].m_bNeedReset = false;
	}

	//初始化按钮状态
	for (int i = DIR_MIN; i < DIR_MAX; ++i)
	{
		m_arrBtnState[i] = false;
	}

	//初始化所有子弹状态
	for (int i = 0; i < BULLET_MAXNUM; ++i)
	{
		m_arrBullet[i].m_bValid = false;
	}

	//初始化四个角落坦克创建状态
	for (int i = 0; i < 4; ++i)
	{
		m_arrCornerState[i] = false;
	}


	//boss初始化
	m_stBoss.m_bDead = true;
	m_stBoss.m_iCamp = CAMP_B;
	m_stBoss.m_stPos.m_iRowIdx = BOSS_ROW_COUNT - 1;
	m_stBoss.m_stPos.m_iColIdx = COLUMN_NUM / 2 - 1;
	m_stBoss.m_fFireWaitTime = 0;

	//初始化BOSS阶段标记
	m_bBossFlag = false;

	m_pGameScene->UpdateBricks();
}


//创建坦克
bool CTankGame::CreateTank(float dt)
{
	//仅创建指定数量坦克
	if (m_iTankCreateCount >= TANK_CREATE_MAXCOUNT)
	{
		return false;
	}

	m_fTankCreateTime += dt;
	if (m_fTankCreateTime < TANK_CREATE_TIME - 40 * m_iSpeed)
	{
		return false;
	}

	m_fTankCreateTime = 0;

	//将要使用的坦克索引
	int iTankIdx = 0;
	
	for (; iTankIdx < TANK_MAXNUM; iTankIdx++)
	{
		if (!m_arrTank[iTankIdx].m_bDead)
		{
			continue;
		}

		//位置有效标记
		bool bPosValidFlag = false;

		TANK_DATA& stData = m_arrTank[iTankIdx];
		if (m_iTankCreateCount < TANK_MAXNUM)
		{
			for (int i = 0; i < 4; ++i)
			{
				if (m_arrCornerState[i])
				{
					continue;
				}
				
				//取该角落位置
				stData.m_stPos.m_iRowIdx = CORNER_POS[2 * i]; 
				stData.m_stPos.m_iColIdx = CORNER_POS[2 * i + 1];
				if (CheckPos(iTankIdx, stData.m_stPos))
				{
					//设置状态
					bPosValidFlag = true;
					m_arrCornerState[i] = true;
					break;
				}
			}
		}
		
		//随机找一个角落创建
		if (!bPosValidFlag)
		{
			bPosValidFlag = GetCornerPos(iTankIdx);
		}

		if (bPosValidFlag)
		{
			break;
		}
	}

	if (iTankIdx >= TANK_MAXNUM)
	{
		return false;
	}

	TANK_DATA& stData = m_arrTank[iTankIdx];

	//随机方向
	stData.m_iDirection = Random(DIR_MIN, DIR_MAX);

	//设置死亡标记
	stData.m_bDead = false;
	stData.m_iCurStep = 0;
	stData.m_iMaxStep = Random(0, TANK_MOVE_MAXSTEP);
	stData.m_fFireWaitTime = 0;
	stData.m_fFireMaxTime = Random(1, (BULLET_CREATE_MAXTIME - 300 * m_iSpeed) / 100) * 100;

	//坦克创建数量更新
	++m_iTankCreateCount;

	return true;
}


bool CTankGame::DrawTank(const POSITION& stPos, int iDirection, const POSITION& stTargetPos)
{
	for (int j = 0; j < 12; j += 2)
	{
		int iNewRowIdx = stPos.m_iRowIdx + TANK_POS_LIST[iDirection][j];
		int iNewColIdx = stPos.m_iColIdx + TANK_POS_LIST[iDirection][j + 1];
		if (iNewRowIdx == stTargetPos.m_iRowIdx && iNewColIdx == stTargetPos.m_iColIdx)
		{
			return true;
		}
	}

	return false;
}


//检查位置是否有效
bool CTankGame::CheckPos(int iTankIdx, const POSITION& stPos)
{
	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		if (m_arrTank[i].m_bDead || (iTankIdx >= 0 && i == iTankIdx))
		{
			continue;
		}

		POSITION& stTankPos = m_arrTank[i].m_stPos;
		if (abs(stPos.m_iRowIdx - stTankPos.m_iRowIdx) < 3 && abs(stPos.m_iColIdx - stTankPos.m_iColIdx) < 3)
		{
			return false;
		}
	}

	if (iTankIdx >= 0)
	{
		POSITION& stSelfPos = m_stSelfTank.m_stPos;
		if (abs(stPos.m_iRowIdx - stSelfPos.m_iRowIdx) < 3 && abs(stPos.m_iColIdx - stSelfPos.m_iColIdx) < 3)
		{
			return false;
		}
	}

	return true;
}


//坦克移动
bool CTankGame::TankMove(float dt)
{
	//时间更新
	m_fTankMoveTime += dt;
	float fMaxTime = TANK_MOVE_INTERVAL - 50 * m_iSpeed + (m_bBossFlag ? 200 : 0);
	if (m_fTankMoveTime < fMaxTime)
	{
		return false;
	}

	m_fTankMoveTime = 0;

	if (m_bBossFlag)
	{
		//获取下一个位置，检查是否有效
		POSITION stNextPos;
		if (!GetNextPos(TANK_BOSS, stNextPos))
		{
			//更换方向
			m_stBoss.m_iDirection = (m_stBoss.m_iDirection == DIR_LEFT ? DIR_RIGHT : DIR_LEFT);
			
			//重新获取一次下一个位置，理论上不会失败
			if (!GetNextPos(TANK_BOSS, stNextPos))
			{
				log("%s GetNextPos return false!");
				return false;
			}
		}

		//更新位置
		m_stBoss.m_stPos = stNextPos;
		return true;
	}
	else
	{
		for (int i = 0; i < TANK_MAXNUM; ++i)
		{
			if (m_arrTank[i].m_bDead)
			{
				continue;
			}

			UpdateTankPos(i);
		}
	}

	return true;
}


//更新坦克位置
void CTankGame::UpdateTankPos(int iTankIdx)
{
	TANK_DATA& refData = iTankIdx < 0 ? m_stSelfTank : m_arrTank[iTankIdx];

	//是否需要重置步数和方向
	if (refData.m_bNeedReset || refData.m_iCurStep == refData.m_iMaxStep)	//可能随机到maxstep为0的情况
	{
		refData.m_iDirection = Random(DIR_MIN, DIR_MAX);
		refData.m_iCurStep = 0;
		refData.m_iMaxStep = Random(0, TANK_MOVE_MAXSTEP);
		refData.m_bNeedReset = false;
		return;
	}

	POSITION stNextPos;
	if (GetNextPos(iTankIdx, stNextPos) && CheckPos(iTankIdx, stNextPos))
	{
		//更新位置
		refData.m_stPos = stNextPos;

		//Step计数
		if (++refData.m_iCurStep >= refData.m_iMaxStep)
		{
			refData.m_bNeedReset = true;
		}
	}
	else
	{
		refData.m_bNeedReset = true;
	}

}

//获取下一个位置
bool CTankGame::GetNextPos(int iTankIdx, POSITION& stOutPos)
{
	const TANK_DATA& refData = (iTankIdx >= 0 ? m_arrTank[iTankIdx] : (iTankIdx == TANK_SELF ? m_stSelfTank : m_stBoss));
	OBJECT_TYPE enType = TYPE_TANK;
	if (iTankIdx == TANK_BOSS)
	{
		enType = TYPE_BOSS;
	}
	return GetNextPos(refData.m_stPos, refData.m_iDirection, stOutPos, enType);
}


//获取下一个位置
bool CTankGame::GetNextPos(const POSITION& stCurPos, int iDirection, POSITION& stOutPos, OBJECT_TYPE enType)
{
	stOutPos = stCurPos;
	switch (iDirection)
	{
	case DIR_UP:
		--stOutPos.m_iRowIdx;
		break;
	case DIR_DOWN:
		++stOutPos.m_iRowIdx;
		break;
	case DIR_LEFT:
		--stOutPos.m_iColIdx;
		break;
	case DIR_RIGHT:
		++stOutPos.m_iColIdx;
		break;
	default:
		log("%s  Wrong Dirction Type. refData.m_iDirection=%d", __FUNCTION__, iDirection);
		break;
	}

	//这里是因为普通的坦克位置是在1位置，因此移动到边界时需要有一列的间距
	// 0
	//010
	//0 0
	int iPadding = 0;
	if (enType == TYPE_BOSS)
	{
		iPadding = 4;
	}
	else if (enType == TYPE_TANK)
	{
		iPadding = 1;
	}

	if (stOutPos.m_iColIdx < iPadding || stOutPos.m_iColIdx > COLUMN_NUM - 1 - iPadding
		|| stOutPos.m_iRowIdx < iPadding || stOutPos.m_iRowIdx > ROW_NUM - 1 - iPadding)
	{
		return false;
	}

	return true;
}


//左按下
void CTankGame::OnLeftBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnState[DIR_LEFT] = true;
}

//左释放
void CTankGame::OnLeftBtnReleased()
{
	m_arrBtnState[DIR_LEFT] = false;
}

//右按下
void CTankGame::OnRightBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnState[DIR_RIGHT] = true;
}

//右释放
void CTankGame::OnRightBtnReleased()
{
	m_arrBtnState[DIR_RIGHT] = false;
}
//上按下
void CTankGame::OnUpBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnState[DIR_UP] = true;
}

//上释放
void CTankGame::OnUpBtnReleased()
{
	m_arrBtnState[DIR_UP] = false;
}

//下按下
void CTankGame::OnDownPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	m_arrBtnState[DIR_DOWN] = true;
}

//下释放
void CTankGame::OnDownReleased()
{
	m_arrBtnState[DIR_DOWN] = false;
}

//Fire按下
void CTankGame::OnFireBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//记录此时毫秒数
	double lfCurTime = GetMillSecond();
	if (lfCurTime - m_lfCurTime >= m_stSelfTank.m_fFireMaxTime)
	{
		//创建子弹
		CreateBullet(-1);
		m_lfCurTime = lfCurTime;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//开启连发状态
	m_bFireState = true;
}

//Fire释放
void CTankGame::OnFireBtnReleased()
{
	m_bFireState = false;
}



bool CTankGame::WaitToMoveBottomCenter(float dt, bool& bDoneFlag)
{
	bDoneFlag = false;

	m_fWaitRefreshTime += dt;
	if (m_fWaitRefreshTime < TANK_SELF_MOVE_INTERVAL - 3 * m_iSpeed)
	{
		return false;
	}

	m_fWaitRefreshTime = 0;

	//距离
	int iRowDis = ROW_NUM - 2 - m_stSelfTank.m_stPos.m_iRowIdx;
	int iColDis = COLUMN_NUM / 2 - 1 - m_stSelfTank.m_stPos.m_iColIdx;

	//重置方向
	m_arrBtnState[DIR_DOWN] = false;
	m_arrBtnState[DIR_LEFT] = false;
	m_arrBtnState[DIR_RIGHT] = false;
	m_arrBtnState[DIR_UP] = false;

	//设置我方坦克移动等待时间（可移动）
	m_fSelfMoveTime = TANK_SELF_MOVE_INTERVAL;

	if (iRowDis == 0 && iColDis == 0 && m_stSelfTank.m_iDirection == DIR_UP)
	{
		bDoneFlag = true;
	}
	else if (iRowDis > 0)
	{
		m_arrBtnState[DIR_DOWN] = true;
		SelfTankMove(0);
	}
	else if (iColDis > 0)
	{
		m_arrBtnState[DIR_RIGHT] = true;
		SelfTankMove(0);
	}
	else if (iColDis < 0)
	{
		m_arrBtnState[DIR_LEFT] = true;
		SelfTankMove(0);
	}
	else if (m_stSelfTank.m_iDirection != DIR_UP)
	{
		m_arrBtnState[DIR_UP] = true;
		SelfTankMove(0);
	}

	return true;
}


void CTankGame::SelfFlagFlash(float dt)
{
	m_lfSelfFlashTime += dt;
	if (m_lfSelfFlashTime < TANK_SELF_FLAG_FLASH_INTERVAL)
	{
		return;
	}

	m_lfSelfFlashTime = 0;
	m_bSelfFlagVisible = !m_bSelfFlagVisible;
	//log("[Change] m_bSelfFlagVisible=%d", m_bSelfFlagVisible);

	POSITION stFlagPos;
	GetFlagPos(stFlagPos);
	m_pGameScene->UpdateBrick(stFlagPos.m_iRowIdx, stFlagPos.m_iColIdx, false, m_bSelfFlagVisible);
}


bool CTankGame::GetFlagPos(POSITION& stPos)
{
	stPos = m_stSelfTank.m_stPos;
	if (m_bBossFlag)
	{
		//如果开启了BOSS阶段，此时方向已经固定，标记位置也固定了
		stPos.m_iRowIdx += 1;
		return true;
	}

	//获取标记位置
	switch (m_stSelfTank.m_iDirection)
	{
	case DIR_UP:
		++stPos.m_iRowIdx;
		break;
	case DIR_DOWN:
		--stPos.m_iRowIdx;
		break;
	case DIR_LEFT:
		++stPos.m_iColIdx;
		break;
	case DIR_RIGHT:
		--stPos.m_iColIdx;
		break;
	default:
		log("%s  Wrong Dirction Type. m_stSelfTank.m_iDirection=%d", __FUNCTION__, m_stSelfTank.m_iDirection);
		return false;
		break;
	}

	return true;
}


const int POS_CHANGE_LIST[16] =
{
	-1, -2, 1, -2,
	-2, -1, -2, 1,
	-1, 2, 1, 2,
	2, -1, 2, 1
};

bool CTankGame::SelfTankMove( float dt )
{
	m_fSelfMoveTime += dt;
	if (m_fSelfMoveTime < TANK_SELF_MOVE_INTERVAL - 3 * m_iSpeed)
	{
		return false;
	}
	
	//重置
	m_fSelfMoveTime = 0;

	if (m_bBossFlag)
	{
		if (m_arrBtnState[DIR_LEFT])
		{
			m_stSelfTank.m_iDirection = DIR_LEFT;
		}
		else if (m_arrBtnState[DIR_RIGHT])
		{
			m_stSelfTank.m_iDirection = DIR_RIGHT;
		}
		else
		{
			return false;
		}

		//检查下一个位置是否有效
		POSITION stNextPos;
		if (!GetNextPos(TANK_SELF, stNextPos))
		{
			return false;
		}

		//更新
		m_stSelfTank.m_stPos = stNextPos;
		//画我方坦克
		m_pGameScene->UpdateBricks(stNextPos.m_iRowIdx - 1, stNextPos.m_iColIdx - 1, stNextPos.m_iRowIdx + 2, stNextPos.m_iColIdx + 2);

		return true;
	}
	else
	{
		for (int i = DIR_MIN; i < DIR_MAX; ++i)
		{
			if (!m_arrBtnState[i])
			{
				continue;
			}

			POSITION stNextPos;

			if (m_stSelfTank.m_iDirection != i)
			{
				m_stSelfTank.m_iDirection = i;

				stNextPos = m_stSelfTank.m_stPos;
			}
			else
			{
				//方向与当前方向一致，检查下一个位置是否有效
				if (!GetNextPos(TANK_SELF, stNextPos) || !CheckPos(TANK_SELF, stNextPos))
				{
					return false;
				}

				//更新位置
				m_stSelfTank.m_stPos = stNextPos;

				//最后一行/列不显示
				m_pGameScene->UpdateBrick(stNextPos.m_iRowIdx + POS_CHANGE_LIST[i * 4], stNextPos.m_iColIdx + POS_CHANGE_LIST[i * 4 + 1], false, false);
				m_pGameScene->UpdateBrick(stNextPos.m_iRowIdx + POS_CHANGE_LIST[i * 4 + 2], stNextPos.m_iColIdx + POS_CHANGE_LIST[i * 4 + 3], false, false);
			}

			//画我方坦克
			m_pGameScene->UpdateBricks(stNextPos.m_iRowIdx - 1, stNextPos.m_iColIdx - 1, stNextPos.m_iRowIdx + 2, stNextPos.m_iColIdx + 2);
			return true;
		}
	}

	return false;
}


//坦克发射子弹
bool CTankGame::TankFire(float dt)
{
	//刷新标记
	bool bRefreshFlag = false;

	//我方坦克
	if (m_bFireState)
	{
		m_stSelfTank.m_fFireWaitTime += dt;
		if (m_stSelfTank.m_fFireWaitTime >= m_stSelfTank.m_fFireMaxTime)
		{
			m_stSelfTank.m_fFireWaitTime = 0;

			//创建子弹
			CreateBullet(-1);

			//设置刷新标记
			bRefreshFlag = true;
		}
	}
	
	if (m_bBossFlag)
	{
		m_stBoss.m_fFireWaitTime += dt;
		if (m_stBoss.m_fFireMaxTime > m_stBoss.m_fFireWaitTime)
		{
			return false;
		}

		m_stBoss.m_fFireWaitTime = 0;

		//随机时间
		m_stBoss.m_fFireMaxTime = Random(BOSS_FIRE_MIN_INTERVAL / 10 - m_iSpeed, BOSS_FIRE_MAX_INTERVAL / 10 - 3 * m_iSpeed) * 10;
		m_stBoss.m_fFireWaitTime = 0;

		//创建子弹
		CreateBullet(-2);

		return true;
	}
	else
	{
		for (int i = 0; i < TANK_MAXNUM; ++i)
		{
			TANK_DATA& refData = m_arrTank[i];

			if (refData.m_bDead)
			{
				continue;
			}

			refData.m_fFireWaitTime += dt;
			if (refData.m_fFireWaitTime < refData.m_fFireMaxTime)
			{
				continue;
			}

			//随机时间
			refData.m_fFireMaxTime = Random(1, (BULLET_CREATE_MAXTIME - 300 * m_iSpeed) / 100) * 100;
			refData.m_fFireWaitTime = 0;

			//创建子弹
			CreateBullet(i);

			//刷新标记
			bRefreshFlag = true;
		}
	}
	
	return bRefreshFlag;
}


//子弹创建
void CTankGame::CreateBullet(int iTankIdx)
{
	const TANK_DATA& refTankData = (iTankIdx >= 0 ? m_arrTank[iTankIdx] : (iTankIdx == -1 ? m_stSelfTank : m_stBoss));

	for (int i = 0; i < BULLET_MAXNUM; ++i)
	{
		BULLET_DATA& refData = m_arrBullet[i];
		if (!refData.m_bValid)
		{
			//初始化子弹
			refData.m_bValid = true;
			refData.m_fMoveTime = 0;
			refData.m_iCamp = refTankData.m_iCamp;
			refData.m_stPos = refTankData.m_stPos;
			if (m_bBossFlag)
			{
				refData.m_iDirection = (iTankIdx == -2 ? DIR_DOWN : DIR_UP);
			}
			else
			{
				refData.m_iDirection = refTankData.m_iDirection;
			}
			return;
		}
	}
}


//子弹移动
bool CTankGame::BulletMove(float dt)
{
	//刷新标记
	bool bRefreshFlag = false;

	for (int i = 0; i < BULLET_MAXNUM; ++i)
	{
		BULLET_DATA& refData = m_arrBullet[i];
		if (!refData.m_bValid)
		{
			continue;
		}

		//时间更新
		refData.m_fMoveTime += dt;
		if ( refData.m_fMoveTime < BULLET_MOVE_INTERVAL - 2 * m_iSpeed )
		{
			continue;
		}
		refData.m_fMoveTime = 0;

		//下一个位置
		POSITION stNextPos;
		if (GetNextPos(refData.m_stPos, refData.m_iDirection, stNextPos, TYPE_BULLET))
		{
			refData.m_stPos = stNextPos;
		}
		else
		{
			//子弹状态
			refData.m_bValid = false;

			//不显示
			m_pGameScene->UpdateBrick(refData.m_stPos.m_iRowIdx, refData.m_stPos.m_iColIdx, false, false);
		}

		bRefreshFlag = true;
	}

	return bRefreshFlag;
}


//子弹是否打中某坦克
int CTankGame::GetBulletFireTankIndex(const POSITION& stBulletPos, int iCamp)
{
	if (iCamp == CAMP_A)
	{
		if (m_bBossFlag && stBulletPos == m_stBoss.m_stPos)
		{
			return 1;
		}

		for (int i = 0; i < TANK_MAXNUM; ++i)
		{
			if (m_arrTank[i].m_bDead)
			{
				continue;
			}

			TANK_DATA& refData = m_arrTank[i];
			int iTankDir = refData.m_iDirection;
			
			//检查是否击中
			for (int j = 0; j < 12; j += 2)
			{
				POSITION stPos = refData.m_stPos;
				stPos.m_iRowIdx += TANK_POS_LIST[iTankDir][j];
				stPos.m_iColIdx += TANK_POS_LIST[iTankDir][j + 1];
				if (stPos == stBulletPos)
				{
					return i;
				}
			}
		}
	}
	else if (iCamp == CAMP_B)
	{
		//检查是否击中
		for (int j = 0; j < 12; j += 2)
		{
			POSITION stPos = m_stSelfTank.m_stPos;
			stPos.m_iRowIdx += TANK_POS_LIST[m_stSelfTank.m_iDirection][j];
			stPos.m_iColIdx += TANK_POS_LIST[m_stSelfTank.m_iDirection][j + 1];
			if (stPos == stBulletPos)
			{
				return 1;
			}
		}
	}

	return -1;
}


//显示爆炸效果
bool CTankGame::ShowBoom(int iRowIndex, int iColIndex, bool& bState)
{
	POSITION stPos = m_stSelfTank.m_stPos;

	//边界调整
	if (stPos.m_iRowIdx == ROW_NUM - 2)
	{
		stPos.m_iRowIdx = ROW_NUM - 3;
	}
	if (stPos.m_iColIdx == COLUMN_NUM - 2)
	{
		stPos.m_iColIdx = COLUMN_NUM - 3;
	}
	
	int iActualRowIdx = iRowIndex - (stPos.m_iRowIdx - 1);
	int iActualColIdx = iColIndex - (stPos.m_iColIdx - 1);

	if (iActualRowIdx >= 0 && iActualRowIdx < 4
		&& iActualColIdx >= 0 && iActualColIdx < 4)
	{
		bState = BOOM_STATE[iActualRowIdx][iActualColIdx] && m_bShowBoom;
		return true;
	}

	return false;
}


void CTankGame::BulletShoot()
{
	for (int i = 0; i < BULLET_MAXNUM; ++i)
	{
		do 
		{
			BULLET_DATA& refData = m_arrBullet[i];
			if (!refData.m_bValid)
			{
				continue;
			}

			int iTankIdx = GetBulletFireTankIndex(refData.m_stPos, refData.m_iCamp);
			if (iTankIdx >= 0)
			{
				if (refData.m_iCamp == CAMP_A)
				{
					if (m_bBossFlag)
					{
						if (++m_stBoss.m_iCurStep >= m_stBoss.m_iMaxStep)
						{
							m_enGameState = GAMESTATE_PASS;
						}

						log("m_iCurStep=%d  m_iMaxStep=%d", m_stBoss.m_iCurStep, m_stBoss.m_iMaxStep);
					}
					else
					{
						TANK_DATA& refTankData = m_arrTank[iTankIdx];

						//敌方坦克死亡
						refTankData.m_bDead = true;

						//重画对应区域状态
						POSITION& stDeadTankPos = refTankData.m_stPos;
						m_pGameScene->UpdateBricks(stDeadTankPos.m_iRowIdx - 1, stDeadTankPos.m_iColIdx - 1,
							stDeadTankPos.m_iRowIdx + 2, stDeadTankPos.m_iColIdx + 2);

						//分数增加
						m_iScore += TANK_KILL_ADD_SCORE;
						m_pGameScene->UpdateScore(m_iScore);
					}
				}
				else if (refData.m_iCamp == CAMP_B)
				{
					//我方坦克死亡
					m_enGameState = GAMESTATE_OVER;
					PLAY_EFFECT(EFFECT_BOOM);
				}

				//子弹状态
				refData.m_bValid = false;

				//不显示
				m_pGameScene->UpdateBrick(refData.m_stPos.m_iRowIdx, refData.m_stPos.m_iColIdx, false, false);
				break;
			}

			//如果没击中坦克，检查是否和敌方子弹相抵
			int iBulletIdx = GetBulletFireBulletIndex(refData.m_stPos, refData.m_iCamp);
			if (iBulletIdx >= 0)
			{
				log("iBulletIdx=%d", iBulletIdx);
				//隐藏这两个子弹
				refData.m_bValid = false;
				m_pGameScene->UpdateBrick(refData.m_stPos.m_iRowIdx, refData.m_stPos.m_iColIdx, false, false);

				BULLET_DATA& refSecBulletData = m_arrBullet[iBulletIdx];
				refSecBulletData.m_bValid = false;
				m_pGameScene->UpdateBrick(refSecBulletData.m_stPos.m_iRowIdx, refSecBulletData.m_stPos.m_iColIdx, false, false);
			}
		} while (0);
	}
}


int CTankGame::GetBulletFireBulletIndex(const POSITION& stBulletPos, int iCamp)
{
	for (int i = 0; i < BULLET_MAXNUM; ++i)
	{
		BULLET_DATA& refData = m_arrBullet[i];
		if (!refData.m_bValid)
		{
			continue;
		}

		//不同阵营且位置相同时算击中
		if (refData.m_stPos == stBulletPos && iCamp != refData.m_iCamp)
		{
			return i;
		}
	}

	return -1;
}


//是否通过当前等级
bool CTankGame::CheckAllTankDead()
{
	if (m_enGameState == GAMESTATE_OVER || m_iTankCreateCount < TANK_CREATE_MAXCOUNT)
	{
		return false;
	}

	for (int i = 0; i < TANK_MAXNUM; ++i)
	{
		if (!m_arrTank[i].m_bDead)
		{
			return false;
		}
	}

	return true;
}


//获取一个有效的角落位置
bool CTankGame::GetCornerPos( int iTankIdx )
{
	int arrValidPosIdx[4] = { 0 };
	int iValidCount = 0;
	POSITION stTempPos;

	for (int i = 0; i < 8; i += 2)
	{
		stTempPos.m_iRowIdx = CORNER_POS[i];
		stTempPos.m_iColIdx = CORNER_POS[i + 1];
		if (CheckPos(iTankIdx, stTempPos))
		{
			arrValidPosIdx[iValidCount++] = i;
		}
	}

	if (iValidCount <= 0)
	{
		return false;
	}

	//设置位置
	int iRandIdx = Random(0, iValidCount);
	int iValidIdx = arrValidPosIdx[iRandIdx];
	POSITION& stPos = m_arrTank[iTankIdx].m_stPos;
	stPos.m_iRowIdx = CORNER_POS[iValidIdx];
	stPos.m_iColIdx = CORNER_POS[iValidIdx + 1];
	return true;
}


void CTankGame::SaveGameData()
{
	
}