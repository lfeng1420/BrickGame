#include "Tank.h"
#include "GlobalDef.h"

CTank::CTank() : m_iCamp(CAMP_NONE)
{

}


CTank::~CTank()
{

}


void CTank::Init(int iRowIdx, int iColIdx, int iCamp)
{
	//设置位置和阵营
	m_iCamp = iCamp;
	m_stTankPos.m_iRowIdx = iRowIdx;
	m_stTankPos.m_iColIdx = iColIdx;

	//死亡标记
	m_bDead = false;

	//方向
	m_iDirection = Random(DIR_MIN, DIR_MAX);

	//步数
	m_iCurStep = 0;
	m_iMaxStep = Random(1, TANK_MOVE_MAXSTEP);

	//等待移动时间
	m_fWaitMoveTime = 0;
	
	//等待发射子弹时间
	m_fWaitFireTime = 0;
}


void CTank::SetDirection(int iDir)
{
	m_iDirection = iDir;
}


void CTank::UpdateTime(float dt)
{
	if (m_bDead)
	{
		return;
	}

	m_fWaitFireTime += dt;
	m_fWaitMoveTime += dt;
}


bool CTank::CanMove()
{
	return !m_bDead && m_fWaitMoveTime >= TANK_MOVE_WAITTIME;
}


bool CTank::Move()
{
	//重置时间
	m_fWaitMoveTime = 0;

	do 
	{
		if (m_iCurStep >= m_iMaxStep)
		{
			//已达到目标步数，需要重置（步数为0的情况，此时无需移动）
			break;
		}

		//更新位置
		TANK_POS stPos;
		if (!GetNextPos(stPos))
		{
			break;
		}

		//位置有效，更新位置，当前移动步数增加
		m_stTankPos = stPos;
		if (++m_iCurStep >= m_iMaxStep)
		{
			//步数已达到目标步数，需要重置
			break;
		}
		else
		{
			//还没有达到目标步数，无需重置
			return false;
		}

	} while (0);

	RandStep();
	return true;
}


void CTank::RandStep()
{
	//重置时间
	m_fWaitMoveTime = 0;

	//重置步数
	m_iCurStep = 0;
	m_iMaxStep = Random(0, TANK_MOVE_MAXSTEP);
}


bool CTank::GetNextPos(TANK_POS& stPos)
{
	//更新位置
	stPos.m_iRowIdx = m_stTankPos.m_iRowIdx;
	stPos.m_iColIdx = m_stTankPos.m_iColIdx;

	switch (m_iDirection)
	{
	case DIR_RIGHT:
		++stPos.m_iColIdx;
		break;
	case DIR_LEFT:
		--stPos.m_iColIdx;
		break;
	case DIR_UP:
		--stPos.m_iRowIdx;
		break;
	case DIR_DOWN:
		++stPos.m_iRowIdx;
		break;
	}


	//检查位置是否有效
	if (stPos.m_iRowIdx < 1 || stPos.m_iRowIdx > ROW_NUM - 2
		|| stPos.m_iColIdx < 1 || stPos.m_iColIdx > COLUMN_NUM - 2)
	{
		return false;
	}

	return true;
}


bool CTank::CanFire()
{
	return !m_bDead && m_fWaitFireTime >= TANK_FIRE_MAXTIME;
}


void CTank::Fire()
{
	//重置时间
	m_fWaitFireTime = 0;

	//随机下一次发射时间
	m_fFireTime = Random(1000, TANK_FIRE_MAXTIME, 1000);
}


const TANK_POS& CTank::GetPos()
{
	return m_stTankPos;
}


bool CTank::IsDead()
{
	return m_bDead;
}

void CTank::SetDead(bool bDead)
{
	m_bDead = bDead;
}


int CTank::GetDirection()
{
	return m_iDirection;
}
