#include "GameOver.h"

CGameOver::CGameOver()
{
}


CGameOver::~CGameOver()
{
}


//初始化
void CGameOver::Init()
{
	m_iRowIdx = 0;
	m_iColIdx = -1;
	m_iDirection = DIR_RIGHT;
	m_iAllCount = 0;
	m_iEndRowIdx = ROW_NUM - 1;
	m_iEndColIdx = COLUMN_NUM - 1;
	m_iBeginColIdx = -1;
	m_iBeginRowIdx = 0;
}


//播放游戏结束动画
bool CGameOver::Play()
{
	if (m_iAllCount == ROW_NUM * COLUMN_NUM)
	{
		//播放结束
		return false;
	}

	++m_iAllCount;

	switch (m_iDirection)
	{
	case DIR_RIGHT:
		++m_iColIdx;
		break;
	case DIR_DOWN:
		++m_iRowIdx;
		break;
	case DIR_LEFT:
		--m_iColIdx;
		break;
	case DIR_UP:
		--m_iRowIdx;
		break;
	}
	//到达右上角
	if (m_iColIdx == m_iEndColIdx && m_iRowIdx == m_iBeginRowIdx)
	{
		m_iDirection = DIR_DOWN;
		++m_iBeginColIdx;
	}
	else if (m_iColIdx == m_iEndColIdx && m_iRowIdx == m_iEndRowIdx)
	{
		m_iDirection = DIR_LEFT;
		++m_iBeginRowIdx;
	}
	else if (m_iColIdx == m_iBeginColIdx && m_iRowIdx == m_iEndRowIdx)
	{
		m_iDirection = DIR_UP;
		--m_iEndColIdx;
	}
	else if (m_iColIdx == m_iBeginColIdx && m_iRowIdx == m_iBeginRowIdx)
	{
		m_iDirection = DIR_RIGHT;
		--m_iEndRowIdx;
	}

	return true;
}


SCENE_INDEX CGameOver::GetSceneType()
{
	return GAME_OVER;
}


void CGameOver::GetCurPos(int& iRowIndex, int& iColIndex)
{
	iRowIndex = m_iRowIdx;
	iColIndex = m_iColIdx;
}


//获取每次执行完Play后等待的时间
float CGameOver::GetRefreshTime()
{
	return 0;
}
