#include "Bullet.h"
#include "GlobalDef.h"

CBullet::CBullet()
{
}


CBullet::~CBullet()
{
}


//更新位置
void CBullet::Update(float dt)
{
	m_fCurTime += dt;
	if (m_fCurTime < BULLET_UPDATE_TIME)
	{
		return;
	}

	//更新位置
	switch (m_iDirection)
	{
	case DIR_RIGHT:
		++m_stPos.m_iColIdx;
		break;
	case DIR_LEFT:
		--m_stPos.m_iColIdx;
		break;
	case DIR_UP:
		--m_stPos.m_iRowIdx;
		break;
	case DIR_DOWN:
		++m_stPos.m_iRowIdx;
		break;
	}

	//如果超出边界，设置标记为无效
	if (m_stPos.m_iRowIdx < 0 || m_stPos.m_iRowIdx > ROW_NUM - 1
		|| m_stPos.m_iColIdx < 0 || m_stPos.m_iColIdx > COLUMN_NUM - 1)
	{
		m_bValid = false;
	}
}


//是否击中了坦克
bool CBullet::IsHitOnTank(const TANK_POS& stTankPos)
{
	if (abs(stTankPos.m_iRowIdx - stTankPos.m_iRowIdx) <= 1
		&& abs(m_stPos.m_iColIdx - stTankPos.m_iColIdx) <= 1)
	{
		return true;
	}

	return false;
}


//获取位置
const TANK_POS& CBullet::GetPos()
{
	return m_stPos;
}


//获取是否有效标记
bool CBullet::IsValid()
{
	return m_bValid;
}


//设置是否有效标记
void CBullet::SetValid(bool bValid)
{
	m_bValid = bValid;
}


void CBullet::Init(int iRowIdx, int iColIdx, int iDirection)
{
	//位置
	m_stPos.m_iRowIdx = iRowIdx;
	m_stPos.m_iColIdx = iColIdx;

	//方向
	m_iDirection = iDirection;

	//时间
	m_fCurTime = 0;

	//设置有效
	m_bValid = true;
}
