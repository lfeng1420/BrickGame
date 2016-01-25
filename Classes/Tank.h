class CTank
{
private:
	int m_iColIdx;			//列位置

	int m_iRowIdx;			//行位置

	float m_iWaitMoveTime;	//当前等待移动的时间

	float m_iWaitFireTime;	//当前等待发射子弹时间

	int m_iFireTime;		//可发射子弹时间

	int m_iCurStep;			//当前走过的步数

	int m_iMaxStep;			//本次计划走的步数

	int m_iRetryNum;		//重试次数

	int m_iDirection;		//走的方向

	int m_iCamp;			//阵营

	bool m_bDead;			//是否死亡

private:
	enum TANK_CAMP
	{
		CAMP_NONE,	
		CAMP_A,
		CAMP_B,
	}

public:
	CTank(int iCamp);
	~CTank();

	void Init(int iRowIdx, int iColIdx);

	void UpdateTime(float dt);

	bool CanMove();

	void Move();

	bool CanFire();

	void Fire();

	bool IsDead();
}