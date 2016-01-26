#pragma once
#include "TankCommon.h"

class CTank
{
private:
	bool m_bDead;					//是否死亡

	TANK_POS m_stTankPos;			//坦克位置

	int m_iCurStep;					//当前走的步数

	int m_iMaxStep;					//最大步数

	float m_fWaitMoveTime;			//射击已等待时间

	float m_fWaitFireTime;			//下一次射击的时间

	float m_fFireTime;				//发射子弹时间

	int m_iDirection;				//当前方向

	int m_iCamp;					//阵营

private:
	enum 
	{
		TANK_MOVE_WAITTIME = 1000,				//每秒刷新一次

		TANK_FIRE_MAXTIME = 5000,				//射击等待最长时间

		TANK_MOVE_MAXSTEP = 11,					//最大移动步数
	};

public:
	CTank();
	~CTank();

	void Init(int iRowIdx, int iColIdx, int iCamp);			//初始化

	void SetDirection(int iDir);							//设置方向

	void UpdateTime(float dt);								//更新时间记录

	bool CanMove();											//是否可以移动

	void Move();											//移动

	void RandStepAndDirection();							//随机方向和步数

	bool CanFire();											//是否可以攻击

	void Fire();											//攻击

	bool IsDead();											//是否死亡

	void SetDead(bool bDead);								//设置死亡标记

	const TANK_POS& GetPos();								//获取当前位置

	TANK_POS GetNextPos();									//获取下一个位置

	int GetDirection();										//获取当前方向
};