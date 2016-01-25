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

//获取位置
const TANK_POS& CBullet::GetPos()
{
	return m_stPos;
}


bool CBullet::IsValid()
{
	return m_bValid;
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
}
