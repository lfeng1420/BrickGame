#include "RacingGame.h"


CRacingGame::CRacingGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CRacingGame::~CRacingGame()
{
}


//初始化
void CRacingGame::Init()
{
	//获取在选择游戏界面设置的速度和等级
	m_iSpeed = GET_INTVALUE("SPEED", 0);
	m_iLevel = GET_INTVALUE("LEVEL", 0);

	//默认生命数为4
	m_iLife = GET_INTVALUE("LIFE", 4);

	//初始化当前分数
	m_iScore = 0;

	//更新界面，分数、等级和生命
	m_pGameScene->UpdateScore(m_iScore);
	m_pGameScene->UpdateLevel(m_iLevel);
	m_pGameScene->UpdateSmallBricks();

	InitData();
}


void CRacingGame::InitData()
{
	//初始化经过的赛车数量
	m_iPassCarCount = 0;

	//默认顶部三行已有路标
	m_iRoadSignCount = 3;

	//等待更新时间
	m_fWaitTime = 0;

	//初始化第一次显示标记
	m_bFirstShow = true;

	//初始化游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//设置记录的最近两个赛车行位置
	m_arrRowIdx[0] = -1;
	m_arrRowIdx[1] = -1;

	//初始化加速标记
	m_bImproveSpeed = false;

	//初始化爆炸显示/隐藏标记，默认显示
	m_bShowBoom = true;

	//初始化闪烁爆炸效果次数
	m_iShowBoomCount = 0;

	InitBrick();
}

//更新
void CRacingGame::Play(float dt)
{
	//检查是否游戏结束
	if (m_arrCurBrick[ROW_NUM - 4][m_iCarPos * 3 + 2] == 1 && m_arrCurBrick[ROW_NUM - 5][m_iCarPos * 3 + 2] == 1)
	{
		m_enGameState = GAMESTATE_OVER;
	}

	if (!m_bFirstShow)
	{
		m_fWaitTime += dt;
		if (m_fWaitTime < GetWaitInterval())
		{
			return;
		}

		//重置时间
		m_fWaitTime = 0;
	}
	else
	{
		RandCreateCars();
		m_bFirstShow = false;
	}

	if (m_enGameState == GAMESTATE_RUNNING)
	{
		UpdateBricks();
	}
	else if (m_enGameState == GAMESTATE_OVER)
	{
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
	else if (m_enGameState == GAMESTATE_PASS)
	{
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
			if (++m_iSpeed >= 10)
			{
				m_iSpeed = 0;
				if (++m_iLevel >= 10)
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


//获取当前Brick状态
bool CRacingGame::GetBrickState(int iRowIndex, int iColIndex)
{
	//赛车位置
	int iCarColIdx = m_iCarPos * 3 + 2;
	int iCarRowIdx = ROW_NUM - 2;

	if (m_enGameState == GAMESTATE_OVER)
	{
		//爆炸
		if (iRowIndex - iColIndex == iCarRowIdx - iCarColIdx && iRowIndex >= iCarRowIdx - 2 && iRowIndex <= iCarRowIdx + 1)
		{
			return m_bShowBoom;
		}

		if (iRowIndex + iColIndex == iCarColIdx + iCarRowIdx - 1 && iRowIndex >= iCarRowIdx - 2 && iRowIndex <= iCarRowIdx + 1)
		{
			return m_bShowBoom;
		}

		if ((iRowIndex == iCarRowIdx		&& iColIndex == iCarColIdx + 1)
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx + 1)
			|| (iRowIndex == iCarRowIdx		&& iColIndex == iCarColIdx - 2)
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx - 2)
			|| (iRowIndex == iCarRowIdx - 2 && iColIndex == iCarColIdx)
			|| (iRowIndex == iCarRowIdx - 2 && iColIndex == iCarColIdx - 1)
			|| (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx)
			|| (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx - 1)
			)
		{
			return false;
		}
	}
	else
	{
		//画赛车自己
		if ((iRowIndex == iCarRowIdx		&& iColIndex == iCarColIdx)
			|| (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx)
			|| (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx - 1)
			|| (iRowIndex == iCarRowIdx + 1 && iColIndex == iCarColIdx + 1)
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx - 1)
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx)
			|| (iRowIndex == iCarRowIdx - 1 && iColIndex == iCarColIdx + 1)
			|| (iRowIndex == iCarRowIdx - 2 && iColIndex == iCarColIdx)
			)
		{
			return true;
		}

	}
	return m_arrCurBrick[iRowIndex][iColIndex];
}


//获取游戏类型
SCENE_INDEX CRacingGame::GetSceneType()
{
	return SCENE_RACING;
}


//左
void CRacingGame::OnLeftBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING || m_iCarPos == 0)
	{
		return;
	}

	--m_iCarPos;

	//检查目标车道是否被占用
	if (m_arrCurBrick[ROW_NUM - 2][m_iCarPos * 3 + 2] == 1)
	{
		m_enGameState = GAMESTATE_OVER;
	}

	m_pGameScene->UpdateBricks(ROW_NUM - 4, m_iCarPos * 3 + 1, ROW_NUM, (m_iCarPos + 1) * 3 + 4);
}


//右
void CRacingGame::OnRightBtnPressed()
{
	if (m_enGameState != GAMESTATE_RUNNING || m_iCarPos == ROAD_COUNT - 1)
	{
		return;
	}

	++m_iCarPos;

	//检查目标车道是否被占用
	if (m_arrCurBrick[ROW_NUM - 2][m_iCarPos * 3 + 2] == 1)
	{
		m_enGameState = GAMESTATE_OVER;
	}

	m_pGameScene->UpdateBricks(ROW_NUM - 4, (m_iCarPos - 1) * 3 + 1, ROW_NUM, m_iCarPos * 3 + 4);
}


//上
void CRacingGame::OnUpBtnPressed()
{

}


//下
void CRacingGame::OnDownPressed()
{

}


//Fire
void CRacingGame::OnFireBtnPressed()
{
	//加速
	m_bImproveSpeed = true;
}


void CRacingGame::OnFireBtnReleased()
{
	m_bImproveSpeed = false;
}


//更新Brick，添加新行
void CRacingGame::UpdateBricks()
{
	//将上一行挪到下一行
	for (int i = ROW_NUM - 1; i >= 1; --i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrCurBrick[i][j] = m_arrCurBrick[i - 1][j];
		}
	}

	if (m_iRoadSignCount < 3)
	{
		//新行，如果当前路标数量小于3，继续设置路标
		m_arrCurBrick[0][0] = true;
		m_arrCurBrick[0][COLUMN_NUM - 1] = true;
		for (int i = 1; i < COLUMN_NUM - 1; ++i)
		{
			m_arrCurBrick[0][i] = false;
		}
	}
	else
	{
		m_iRoadSignCount = -1;
		for (int i = 0; i < COLUMN_NUM; ++i)
		{
			m_arrCurBrick[0][i] = false;
		}
	}

	++m_iRoadSignCount;

	m_arrRowIdx[0] += m_arrRowIdx[0] > 0 ? 1 : 0;
	m_arrRowIdx[1] += m_arrRowIdx[1] > 0 ? 1 : 0;
	if (m_arrRowIdx[1] == ROW_DISTANCE * 2)
	{
		++m_iPassCarCount;
		m_iScore += 100;
		m_pGameScene->UpdateScore(m_iScore);

		if (m_iPassCarCount >= GAMEPASS_CARCOUNT)
		{
			m_enGameState = GAMESTATE_PASS;
			m_iAddScoreCount = 0;
		}
	}

	if (m_arrRowIdx[0] == 2 + ROW_DISTANCE)
	{
		RandCreateCars();
	}
}


//画赛车
void CRacingGame::RandCreateCars()
{
	//　口
	//口口口
	//　口		位置以该行为准
	//口　口

	int iCount = Random(1, CAR_MAXNUM);
	for (int i = 0; i < iCount; ++i)
	{
		int iRoadIdx = Random(0, ROAD_COUNT);
		int iColIdx = iRoadIdx * 3 + 2;

		//第四行
		m_arrCurBrick[CAR_DEFAULTROW + 1][iColIdx - 1] = 1;
		m_arrCurBrick[CAR_DEFAULTROW + 1][iColIdx + 1] = 1;
		//第三行
		m_arrCurBrick[CAR_DEFAULTROW][iColIdx] = 1;
		//第二行
		m_arrCurBrick[CAR_DEFAULTROW - 1][iColIdx] = 1;
		m_arrCurBrick[CAR_DEFAULTROW - 1][iColIdx - 1] = 1;
		m_arrCurBrick[CAR_DEFAULTROW - 1][iColIdx + 1] = 1;
		//第一行
		m_arrCurBrick[CAR_DEFAULTROW - 2][iColIdx] = 1;
	}

	m_arrRowIdx[1] = m_arrRowIdx[0];
	m_arrRowIdx[0] = CAR_DEFAULTROW;
}


void CRacingGame::InitBrick()
{
	//初始化Brick
	int iCount = 0;
	for (int i = 0; i < ROW_NUM; ++i)
	{
		int iBeginIndex = 0;
		int iEndIndex = COLUMN_NUM - 1;
		if (iCount < 3)
		{
			m_arrCurBrick[i][0] = true;
			m_arrCurBrick[i][iEndIndex] = true;

			iBeginIndex = 1;
			iEndIndex -= 1;
		}
		else
		{
			iCount = -1;
		}

		for (int j = iBeginIndex; j <= iEndIndex; ++j)
		{
			m_arrCurBrick[i][j] = false;
		}

		++iCount;
	}

	//随机自己赛车的位置
	m_iCarPos = Random(0, ROAD_COUNT);
}


//获取等待时长
int CRacingGame::GetWaitInterval()
{
	if (m_enGameState == GAMESTATE_PASS)
	{
		return DEFAULT_INTERVAL;
	}

	//游戏结束时播放结束动画
	if (m_enGameState == GAMESTATE_OVER)
	{
		return 40;
	}

	//加速
	if (m_bImproveSpeed)
	{
		return 30;
	}
	else
	{
		//正常速度
		return DEFAULT_INTERVAL - 13 * m_iSpeed;
	}
}


//生命数
bool CRacingGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	if (iRowIndex == 0 && iColIndex < m_iLife)
	{
		return true;
	}

	return false;
}
