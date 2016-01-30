#include "SnakeGame.h"


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
	m_pGameScene->UpdateScore(m_iScore);
	m_pGameScene->UpdateLevel(m_iLevel);
	m_pGameScene->UpdateSmallBricks();

	InitData();
}


//更新
void CSnakeGame::Play(float dt)
{
	m_fSnakeMoveTime += dt;
	if (m_fSnakeMoveTime >= SNAKE_MOVE_INTERVAL)
	{
		m_fSnakeMoveTime = 0;
		SnakeMove();
	}
	
	m_fAppleShowTime += dt;
	if (m_fAppleShowTime >= APPLE_REFRESH_INTERVAL)
	{
		m_fAppleShowTime = 0;
		m_bAppleState = !m_bAppleState;
	}

	m_pGameScene->UpdateBricks();
}


//获取当前Brick状态
bool CSnakeGame::GetBrickState(int iRowIndex, int iColIndex)
{
	POSITION stCurPos = { iRowIndex, iColIndex };
	
	//苹果
	if (stCurPos == m_stApplePos)
	{
		return m_bAppleState;
	}
	
	//蛇
	for (int i = 0; i < m_mapSnake.size(); ++i)
	{
		const POSITION& stPos = m_mapSnake[i].m_stPos;
		if (stCurPos == stPos)
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
	//初始化蛇身，默认有3节，0默认为头部，从头部开始创建
	int iStartColIdx = COLUMN_NUM / 2 + SNAKE_DEFAULT_LEN / 2;
	for (int i = 0; i < SNAKE_DEFAULT_LEN; ++i, --iStartColIdx)
	{
		SNAKE_NODE& stNode = m_mapSnake[i];
		stNode.m_stPos.m_iRowIdx = ROW_NUM / 2 - 1;
		stNode.m_stPos.m_iColIdx = iStartColIdx;
		stNode.m_iDirection = DIR_RIGHT;
	}

	//初始化时间
	m_fSnakeMoveTime = 0;
	m_fAppleShowTime = 0;

	//苹果显示状态
	m_bAppleState = false;

	//游戏状态
	m_enGameState = GAMESTATE_RUNNING;

	//随机苹果位置
	RandApplePos();
}


//随机苹果位置
void CSnakeGame::RandApplePos()
{
	//先随机一列，因为不可能出现某一列被占满的情况
	int iColIndex = Random(0, ROW_NUM - 1);

	//统计当前行占位个数
	int arrRowIdxList[ROW_NUM] = { 0 };
	int iValidCount = 0;
	for (int i = 0; i < m_mapSnake.size(); ++i)
	{
		POSITION& stNodePos = m_mapSnake[i].m_stPos;
		if (stNodePos.m_iColIdx == iColIndex)
		{
			continue;
		}

		arrRowIdxList[iValidCount++] = stNodePos.m_iRowIdx;
	}

	//随机获取一个索引
	int iRandIdx = Random(0, iValidCount);

	//设置位置
	m_stApplePos = {arrRowIdxList[iRandIdx], iColIndex};
}


//蛇更新位置
void CSnakeGame::SnakeMove()
{
	int iNodeCount = m_mapSnake.size();

	//先保存最后一个节点位置，因为可能需要添加节点
	SNAKE_NODE stLastSnakeNode = m_mapSnake[iNodeCount - 1];

	for (int i = 0; i < iNodeCount; ++i)
	{
		//如果更新节点失败，那么是蛇节点超出边界了，游戏结束
		//虽然游戏结束，但其他节点还是需要继续更新的
		if (!UpdateSnakeNode(i))
		{
			m_enGameState = GAMESTATE_OVER;
		}
	}

	//检查头部节点是否碰到苹果
	if (m_enGameState != GAMESTATE_OVER && m_mapSnake[0].m_stPos == m_stApplePos)
	{
		//往尾部添加节点，直接设置为更新前的最后一个节点即可
		m_mapSnake[iNodeCount] = stLastSnakeNode;
	}
}


//更新蛇节点位置
bool CSnakeGame::UpdateSnakeNode(int iIndex)
{
	SNAKE_NODE& refNode = m_mapSnake[iIndex];

	POSITION stNextPos = refNode.m_stPos;
	switch (refNode.m_iDirection)
	{
	case DIR_UP:
		--stNextPos.m_iRowIdx;
		break;
	case DIR_DOWN:
		++stNextPos.m_iRowIdx;
		break;
	case DIR_LEFT:
		--stNextPos.m_iColIdx;
		break;
	case DIR_RIGHT:
		++stNextPos.m_iColIdx;
		break;
	default:
		break;
	}

	//检查位置是否有效
	if (stNextPos.m_iColIdx < 0 || stNextPos.m_iColIdx > COLUMN_NUM - 1
		|| stNextPos.m_iRowIdx < 0 || stNextPos.m_iRowIdx > ROW_NUM - 1)
	{
		return false;
	}

	//位置更新
	refNode.m_stPos = stNextPos;

	//方向更新：是否经过需要变更方向的节点
	for (int i = 0; i < m_mapChangeNode.size(); ++i)
	{
		const SNAKE_NODE& refChangeNode = m_mapChangeNode[i];

		if (stNextPos == refChangeNode.m_stPos)
		{
			//更新方向
			refNode.m_iDirection = refChangeNode.m_iDirection;
			break;
		}
	}

	return true;
}


//左按下
void CSnakeGame::OnLeftBtnPressed()
{

}


//左释放
void CSnakeGame::OnLeftBtnReleased()
{

}


//右按下
void CSnakeGame::OnRightBtnPressed()
{

}


//右释放
void CSnakeGame::OnRightBtnReleased()
{

}


//上按下
void CSnakeGame::OnUpBtnPressed()
{

}


//上释放
void CSnakeGame::OnUpBtnReleased()
{

}


//下按下
void CSnakeGame::OnDownPressed()
{

}


//下释放
void CSnakeGame::OnDownReleased()
{

}


//Fire按下
void CSnakeGame::OnFireBtnPressed()
{

}


//Fire释放
void CSnakeGame::OnFireBtnReleased()
{

}

