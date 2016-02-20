#include "SnakeGame.h"

//相反方向
int OPPSITE_DIRECTION[DIR_MAX] = 
{
	DIR_LEFT,
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
};


CSnakeGame::CSnakeGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CSnakeGame::~CSnakeGame()
{
}


//初始化
void CSnakeGame::Init()
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
void CSnakeGame::Play(float dt)
{
	if (m_enGameState == GAMESTATE_RUNNING)
	{
		//刷新标记
		bool bNeedRefreshFlag = false;

		m_fWaitRefreshTime += dt;
		float fMaxTime = m_bImproveSpeed ? 50 : (SNAKE_MOVE_INTERVAL - 30 * m_iSpeed);
		if (m_fWaitRefreshTime >= fMaxTime)
		{
			m_fWaitRefreshTime = 0;
			SnakeMove();
			bNeedRefreshFlag = true;
		}

		m_fAppleShowTime += dt;
		if (m_fAppleShowTime >= APPLE_REFRESH_INTERVAL)
		{
			m_fAppleShowTime = 0;
			m_bAppleState = !m_bAppleState;
			bNeedRefreshFlag = true;
		}

		//是否需要刷新界面
		if (!bNeedRefreshFlag)
		{
			return;
		}
	}
	else if (m_enGameState == GAMESTATE_PASS)
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

	m_pGameScene->UpdateBricks();
}


//获取当前Brick状态
bool CSnakeGame::GetBrickState(int iRowIndex, int iColIndex)
{
	POSITION stCurPos;
	stCurPos.m_iRowIdx = iRowIndex;
	stCurPos.m_iColIdx = iColIndex;
	
	//苹果
	if (stCurPos == m_stApplePos)
	{
		return m_bAppleState;
	}

	if (m_enGameState == GAMESTATE_OVER)
	{
		const POSITION& stHeaderPos = m_mapSnakeNodes[0];
		int iRowStartIdx = (stHeaderPos.m_iRowIdx > ROW_NUM - 4 ? ROW_NUM - 4 : stHeaderPos.m_iRowIdx);
		int iColStartIdx = (stHeaderPos.m_iColIdx > COLUMN_NUM - 4 ? COLUMN_NUM - 4 : stHeaderPos.m_iColIdx);
		for (int i = iRowStartIdx; i < iRowStartIdx + 4; ++i)
		{
			for (int j = iColStartIdx; j < iColStartIdx + 4; ++j)
			{
				if (iRowIndex == i && iColIndex == j )
				{
					return m_bShowBoom && BOOM_STATE[i - iRowStartIdx][j - iColStartIdx];
				}
			}
		}
	}

	//蛇
	for (int i = 0; i < m_mapSnakeNodes.size(); ++i)
	{
		if (stCurPos == m_mapSnakeNodes[i])
		{
			return true;
		}
	}

	return false;
}


//获取小方块序列中的方块状态
bool CSnakeGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

	return false;
}


//获取游戏类型
SCENE_INDEX CSnakeGame::GetSceneType()
{
	return SCENE_SNAKE;
}


void CSnakeGame::InitData()
{
	//初始化蛇方向
	m_iSnakeDirection = DIR_RIGHT;

	//初始化蛇身，默认有3节，0默认为头部，从头部开始创建
	m_mapSnakeNodes.clear();
	int iStartColIdx = COLUMN_NUM / 2 + SNAKE_DEFAULT_LEN / 2;
	for (int i = 0; i < SNAKE_DEFAULT_LEN; ++i, --iStartColIdx)
	{
		POSITION& refPos = m_mapSnakeNodes[i];
		refPos.m_iRowIdx = ROW_NUM / 2 - 1;
		refPos.m_iColIdx = iStartColIdx;
	}

	//初始化时间
	m_fWaitRefreshTime = 0;
	m_fAppleShowTime = 0;

	//苹果显示状态
	m_bAppleState = false;

	//游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//爆炸显示计数
	m_iShowBoomCount = 0;
	//爆炸显示状态
	m_bShowBoom = true;

	//分数增加次数计数
	m_iAddScoreCount = 0;

	//初始化加速标记
	m_bImproveSpeed = false;

	//随机苹果位置
	RandApplePos();
}


//随机苹果位置
void CSnakeGame::RandApplePos()
{
	//先随机一列，因为不可能出现某一列被占满的情况
	int iColIndex = Random(0, COLUMN_NUM - 1);

	//统计当前行占位状态
	bool arrRowStateList[ROW_NUM] = { false };
	for (int i = 0; i < m_mapSnakeNodes.size(); ++i)
	{
		POSITION& stNodePos = m_mapSnakeNodes[i];
		if (stNodePos.m_iColIdx == iColIndex)
		{
			arrRowStateList[stNodePos.m_iRowIdx] = true;
		}
	}

	int arrRowIdxList[ROW_NUM] = {0};
	int iCount = 0;
	for (int i = 0; i < ROW_NUM; ++i)
	{
		if (!arrRowStateList[i])
		{
			arrRowIdxList[iCount++] = i;
		}
	}

	//随机获取一个索引
	int iRandIdx = Random(0, iCount);

	//设置位置
	m_stApplePos.m_iRowIdx = arrRowIdxList[iRandIdx];
	m_stApplePos.m_iColIdx = iColIndex;
}


//蛇更新位置
void CSnakeGame::SnakeMove()
{
	PLAY_EFFECT(EFFECT_WALL);

	//记录最后一个节点位置，因为可能需要增加节点
	int iNodeCount = m_mapSnakeNodes.size();
	POSITION stLastPos = m_mapSnakeNodes[iNodeCount - 1];

	//获取头部下个位置
	POSITION stHeaderPos = m_mapSnakeNodes[0];
	switch (m_iSnakeDirection)
	{
	case DIR_RIGHT:
		++stHeaderPos.m_iColIdx;
		break;
	case DIR_DOWN:
		++stHeaderPos.m_iRowIdx;
		break;
	case DIR_LEFT:
		--stHeaderPos.m_iColIdx;
		break;
	case DIR_UP:
		--stHeaderPos.m_iRowIdx;
		break;
	}

	if (CheckGameOver(stHeaderPos))
	{
		//设置苹果显示状态
		m_bAppleState = true;

		//音效
		PLAY_EFFECT(EFFECT_BOOM);
		return;
	}

	//更新其他节点
	for (int i = iNodeCount - 2; i >= 0; --i)
	{
		POSITION& stSrcPos = m_mapSnakeNodes[i + 1];
		const POSITION& stDestPos = m_mapSnakeNodes[i];
		stSrcPos.m_iRowIdx = stDestPos.m_iRowIdx;
		stSrcPos.m_iColIdx = stDestPos.m_iColIdx;
	}

	//头部位置更新
	m_mapSnakeNodes[0] = stHeaderPos;

	//检查头部位置是否是苹果的位置
	if (stHeaderPos == m_stApplePos)
	{
		//加分
		m_iScore += SNAKE_EAT_ADD_SCORE;
		m_pGameScene->UpdateScore(m_iScore);

		//增加节点
		m_mapSnakeNodes[iNodeCount] = stLastPos;

		//检查是否通过
		/*if (iNodeCount + 1 == SNAKE_MAX_LEN)
		{
			m_enGameState = GAMESTATE_PASS;
			return;
		}*/

		//苹果不显示
		m_pGameScene->UpdateBrick(m_stApplePos.m_iRowIdx, m_stApplePos.m_iColIdx, false, false);

		//重新随机苹果位置
		RandApplePos();
	}
}



bool CSnakeGame::CheckGameOver(const POSITION& stHeaderPos)
{
	//检查位置是否有效
	if (stHeaderPos.m_iColIdx < 0 || stHeaderPos.m_iColIdx > COLUMN_NUM - 1
		|| stHeaderPos.m_iRowIdx < 0 || stHeaderPos.m_iRowIdx > ROW_NUM - 1)
	{
		m_enGameState = GAMESTATE_OVER;
		return true;
	}

	for (int i = 1; i < m_mapSnakeNodes.size(); ++i)
	{
		if (stHeaderPos == m_mapSnakeNodes[i])
		{
			m_enGameState = GAMESTATE_OVER;
			return true;
		}
	}

	return false;
}


//改变方向
void CSnakeGame::ChangeDirection(int iDirection)
{
	//如果是相反方向或相同方向，或者游戏不是运行中状态，则返回
	if (m_iSnakeDirection == OPPSITE_DIRECTION[iDirection]
		|| m_iSnakeDirection == iDirection 
		|| m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	//设置方向
	m_iSnakeDirection = iDirection;
	
	//移动
	SnakeMove();

	//重置时间
	m_fWaitRefreshTime = 0;
}


//左按下
void CSnakeGame::OnLeftBtnPressed()
{
	ChangeDirection(DIR_LEFT);
}


//右按下
void CSnakeGame::OnRightBtnPressed()
{
	ChangeDirection(DIR_RIGHT);
}


//上按下
void CSnakeGame::OnUpBtnPressed()
{
	ChangeDirection(DIR_UP);
}


//下按下
void CSnakeGame::OnDownPressed()
{
	ChangeDirection(DIR_DOWN);
}


//Fire按下
void CSnakeGame::OnFireBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//按钮音效
	PLAY_EFFECT(EFFECT_CHANGE2);

	m_bImproveSpeed = true;
}


//Fire释放
void CSnakeGame::OnFireBtnReleased()
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	m_bImproveSpeed = false;
}

