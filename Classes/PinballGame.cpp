#include "PinballGame.h"


CPinballGame::CPinballGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
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
	BallMove(dt);
}


//获取当前Brick状态
bool CPinballGame::GetBrickState(int iRowIndex, int iColIndex)
{
	return false;
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

}


//左释放
void CPinballGame::OnLeftBtnReleased()
{

}


//右按下
void CPinballGame::OnRightBtnPressed()
{

}


//右释放
void CPinballGame::OnRightBtnReleased()
{

}


//Fire按下
void CPinballGame::OnFireBtnPressed()
{
	if (m_enGameState != GAMESTATE_PAUSE)
	{
		return;
	}

	m_enGameState = GAMESTATE_RUNNING;
}


void CPinballGame::InitData()
{
	//重置界面
	m_pGameScene->UpdateBricks();

	//初始化球每次移动的距离
	m_stBallDis = {-1, 1};

	//挡板的位置
	m_stGuard = { ROW_NUM - 1, (COLUMN_NUM - GUARD_BRICK_COUNT) / 2 };
	for (int i = 0; i < GUARD_BRICK_COUNT; ++i)
	{
		m_pGameScene->UpdateBrick(m_stGuard.m_iRowIdx, m_stGuard.m_iColIdx + i, false, true);
	}

	//球位置
	m_stBall = { ROW_NUM - 2, m_stGuard.m_iColIdx + (GUARD_BRICK_COUNT - 1) / 2 };
	m_pGameScene->UpdateBrick(m_stBall.m_iRowIdx, m_stBall.m_iColIdx, false, true);

	//初始化各个方块位置
	int iBrickIdx = 0;
	for (int i = 5 - m_iSpeed / 2; i <= ROW_NUM / 2 - 1; ++i)
	{
		int iStartColIdx = (i % 2 == 0 ? 2 : 1);
		for (int j = iStartColIdx; j <= COLUMN_NUM - iStartColIdx - 1; ++j)
		{
			m_mapBrick[iBrickIdx++] = {i, j};
			m_pGameScene->UpdateBrick(i, j, false, true);
		}
	}

	//游戏状态
	m_enGameState = GAMESTATE_PAUSE;

	//时间相关
	m_fMoveTime = 0;
}

void CPinballGame::BallMove( float dt )
{
	if (m_enGameState != GAMESTATE_RUNNING)
	{
		return;
	}

	//时间更新
	m_fMoveTime += dt;
	if (m_fMoveTime < BALL_MOVE_INTERVAL - 13 * m_iSpeed)
	{
		return;
	}
	//重置
	m_fMoveTime = 0;

	//检查下一个位置
	POSITION stNextPos = m_stBall + m_stBallDis;
	
	//获取仅横向/纵向移动时的坐标
	POSITION stNextRowPos = { m_stBall.m_iRowIdx + m_stBallDis.m_iRowIdx, m_stBall.m_iColIdx };
	POSITION stNextColPos = { m_stBall.m_iRowIdx, m_stBall.m_iColIdx + m_stBallDis.m_iColIdx };

	int iRowHitIdx = GetHitBrickIndex(stNextRowPos);
	int iColHitIdx = GetHitBrickIndex(stNextColPos);
	int iActHitIdx = GetHitBrickIndex(stNextPos);

	if (stNextPos.m_iRowIdx < 0)
	{
		if (stNextPos.m_iColIdx < 0 || stNextPos.m_iColIdx >= COLUMN_NUM)
		{
			m_stBallDis.m_iColIdx *= -1;
		}
		m_stBallDis.m_iRowIdx *= -1;
	}

	if (stNextPos.m_iColIdx < 0 || stNextPos.m_iColIdx >= COLUMN_NUM)
	{
		if (iRowHitIdx >= 0)
		{
			//修改方向
			m_stBallDis.m_iColIdx *= -1;
			m_stBallDis.m_iRowIdx *= -1;

			//移除方块
			m_mapBrick.erase(iRowHitIdx);

			//加分
			m_iScore += 100;
			m_pGameScene->UpdateScore(m_iScore);
		}
	}

	
	if (stNextPos.m_iColIdx >= m_stGuard.m_iColIdx && stNextPos.m_iRowIdx == m_stGuard.m_iRowIdx
			&& stNextPos.m_iColIdx < m_stGuard.m_iColIdx + GUARD_BRICK_COUNT)
	{
		m_stBallDis.m_iRowIdx *= -1;
	}


	//取消之前的显示
	m_pGameScene->UpdateBrick(m_stBall.m_iRowIdx, m_stBall.m_iColIdx, false, false);

	//位置更新
	m_stBall += m_stBallDis;

	//显示新位置
	m_pGameScene->UpdateBrick(m_stBall.m_iRowIdx, m_stBall.m_iColIdx, false, true);
}


//获取击中的方块索引
int CPinballGame::GetHitBrickIndex(const POSITION& stPos)
{
	for (int i = 0; i < m_mapBrick.size(); ++i)
	{
		if (stPos == m_mapBrick[i])
		{
			return i;
		}
	}

	return -1;
}
