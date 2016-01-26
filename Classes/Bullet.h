#pragma once
#include "TankCommon.h"

class CBullet
{
public:
	CBullet();
	~CBullet();

	//初始化
	void Init(int iRowIdx, int iColIdx, int iDirection);

	//更新位置
	void Update(float dt);

	//获取位置
	const TANK_POS& GetPos();

	//是否有效
	bool IsValid();

	//设置是否有效标记
	void SetValid(bool bValid);

	//是否击中了坦克
	bool IsHitOnTank(const TANK_POS& stTankPos);

private:
	enum 
	{
		BULLET_UPDATE_TIME = 500,	//每500毫秒刷新一次
	};

private:
	bool m_bValid;			//是否有效

	float m_fCurTime;		//当前等待更新时间

	TANK_POS m_stPos;		//当前位置

	int m_iDirection;		//方向
};

