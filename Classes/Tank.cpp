#include "Tank.h"

CTank::CTank(int iCamp) : m_iCamp(iCamp)
{

}


CTank::~CTank()
{

}


void CTank::Init(int iRowIdx, int iColIdx)
{
	//设置位置
	m_iRowIdx = iRowIdx;
	m_iColIdx = iColIdx;

	//死亡标记
	m_bDead = false;

	//方向
	m_iDirection = Random(DIR_MIN, DIR_MAX);

	//步数
	m_iCurStep = 0;
	m_iMaxStep = Random(1, TANK_MOVE_MAXSTEP);

	//等待移动时间
	m_iWaitMoveTime = 0;

	//重试次数
	m_iRetryNum = 0;
	
	//等待发射子弹时间
	m_iWaitFireTime = 0;
}


void CTank::UpdateTime(float dt)
{
	m_iWaitFireTime += dt;
	m_iWaitMoveTime += dt;
}


bool CTank::CanMove()
{
	
}


void CTank::Move()
{

}


bool CTank::CanFire()
{

}


void CTank::Fire()
{

}