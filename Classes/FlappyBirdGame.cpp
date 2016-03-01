#include "FlappyBirdGame.h"


CFlappyBirdGame::CFlappyBirdGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CFlappyBirdGame::~CFlappyBirdGame()
{
}


//初始化
void CFlappyBirdGame::Init()
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
void CFlappyBirdGame::Play(float dt)
{
	if (m_enGameState == GAMESTATE_RUNNING)
	{
		bool bRefreshFlag = BirdMove(dt);
		bRefreshFlag = PillarMove(dt) || bRefreshFlag;
		if (!bRefreshFlag)
		{
			return;
		}
	}
	else if (m_enGameState == GAMESTATE_OVER)
	{
		m_fRefreshTime += dt;
		if (m_fRefreshTime < GAMEOVER_WAITTIME)
		{
			return;
		}

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
	else if (m_enGameState == GAMESTATE_PASS)
	{
		m_fRefreshTime += dt;
		if (m_fRefreshTime < GAMEPASS_INTERVAL)
		{
			return;
		}

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

	m_pGameScene->UpdateBricks();
}


//柱子移动
bool CFlappyBirdGame::PillarMove(float dt)
{
	m_fPillarMoveTime += dt;
	if (m_fPillarMoveTime < PILLAR_MOVE_INTERVAL - m_iSpeed * 13)
	{
		return false;
	}

	//重置
	m_fPillarMoveTime = 0;

	for (int i = 0; i < PILLAR_MAXCOUNT; ++i)
	{
		PILLAR& stData = m_arrPillar[i];
		if (stData.m_iLen > 0 && --stData.m_iColIdx < 0)
		{
			stData.m_iLen = 0;
		}
	}

	//更新移动列数计数
	if (++m_iMoveCount >= PILLAR_COLUMN_DISTANCE)
	{
		m_iMoveCount = 0;

		//创建新柱子
		CreatePillar();
	}

	return true;
}


//创建新柱子
void CFlappyBirdGame::CreatePillar()
{
	for (int i = 0; i < PILLAR_MAXCOUNT; ++i)
	{
		PILLAR& stData = m_arrPillar[i];
		if (stData.m_iLen == 0)
		{
			stData.m_iColIdx = COLUMN_NUM - 1;
			stData.m_iLen = Random(1, PILLAR_MAXLEN);
			return;
		}
	}
}


//鸟移动
bool CFlappyBirdGame::BirdMove(float dt)
{
	float fInterval = BIRD_DOWN_INTERVAL - 3 * m_iSpeed;
	//fInterval *= m_bImproveSpeed ? 0.4f : 1;

	//时间更新
	m_fBirdMoveTime += dt;
	if (m_fBirdMoveTime < fInterval)
	{
		return false;
	}

	//重置
	m_fBirdMoveTime = 0;

	//下降行数
	int iDownColCount = 1;

	//检查下降的距离是否超过范围
	if (ROW_NUM - m_iBirdRowIdx <= iDownColCount)
	{
		m_enGameState = GAMESTATE_OVER;
		iDownColCount = ROW_NUM - m_iBirdRowIdx - 1;
	}

	//更新所在行
	m_iBirdRowIdx += iDownColCount;

	return true;
}


void CFlappyBirdGame::UpdateGameState()
{
	//检查鸟是否碰到柱子
	for (int i = 0; i < PILLAR_MAXCOUNT; ++i)
	{
		const PILLAR& stData = m_arrPillar[i];
		if (stData.m_iColIdx == COLUMN_NUM / 2 || stData.m_iColIdx == COLUMN_NUM / 2 - 1)
		{
			int iDownLen = PILLAR_MAXLEN - stData.m_iLen;
			if ((m_iBirdRowIdx >= 0 && m_iBirdRowIdx < stData.m_iLen) ||
				(m_iBirdRowIdx >= ROW_NUM - iDownLen && m_iBirdRowIdx < ROW_NUM))
			{
				m_enGameState = GAMESTATE_OVER;
			}
		}
	}
}


//获取当前Brick状态
bool CFlappyBirdGame::GetBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == m_iBirdRowIdx && iColIndex == COLUMN_NUM / 2 - 1)
	{
		return true;
	}

	//柱子
	for(int i = 0; i < PILLAR_MAXCOUNT; ++i)
	{
		const PILLAR& stData = m_arrPillar[i];
		if (stData.m_iLen == 0)
		{
			continue;
		}

		//下方柱子长度
		int iDownLen = PILLAR_MAXLEN - stData.m_iLen;

		if (iColIndex == stData.m_iColIdx 
			&& ((iRowIndex >= 0 && iRowIndex < stData.m_iLen) || 
				(iRowIndex >= ROW_NUM - iDownLen && iRowIndex < ROW_NUM))
			)
		{
			return true;
		}
	}

	return false;
}


//生命数
bool CFlappyBirdGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

	return false;
}


//获取游戏类型
SCENE_INDEX CFlappyBirdGame::GetSceneType()
{
	return SCENE_FLAPPYBIRD;
}


//上按下
void CFlappyBirdGame::OnUpBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	if (--m_iBirdRowIdx < 0)
	{
		m_iBirdRowIdx = 0;
		m_enGameState = GAMESTATE_OVER;
		return;
	}

	UpdateGameState();
}


//Fire按下
void CFlappyBirdGame::OnFireBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//设置标记
	m_bImproveSpeed = true;
}


//Fire释放
void CFlappyBirdGame::OnFireBtnReleased()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//设置标记
	m_bImproveSpeed = false;
}


//初始化数据、变量等
void CFlappyBirdGame::InitData()
{
	//初始化自己所在行位置
	m_iBirdRowIdx = ROW_NUM / 2 - 1;

	//显示状态
	m_bBirdVisible = true;
	//上升状态
	m_bBirdUpState = false;

	//初始化柱子位置
	for (int i = 0; i < PILLAR_MAXCOUNT; ++i)
	{
		m_arrPillar[i].m_iColIdx = 0;
		m_arrPillar[i].m_iLen = 0;
	}

	//初始化游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//时间相关
	m_fPillarMoveTime = 0;
	m_fRefreshTime = 0;
	m_fBirdMoveTime = 0;

	//初始化移动列数计数
	m_iMoveCount = 0;

	m_bImproveSpeed = false;
}

