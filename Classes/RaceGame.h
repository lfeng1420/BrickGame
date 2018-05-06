#pragma once
#include "GameBase.h"

class CRaceGame : public CGameBase
{
public:
	// Start
	virtual void Start();

	// Update
	// param: dt, millisecond
	virtual void Update(float dt);

	// Get game id
	virtual EnGameID GetGameID();

	// Button event
	virtual void OnButtonEvent(const SEventContextButton* pButtonEvent);

private:
	// Init all cars
	void __InitCars();

	// Create car
	void __CreateNewCar(bool& bUpdateFlag);

	// Check condition
	bool __CanCreateCar();

	// Get invalid car index
	bool __GetInvalidCarIdx(int* arrCarIdx, int nCount);

	// Check car overlap
	bool __CheckCarOverlap(int nColIdx, bool bStrictFlag);

	// Update enemy cars
	void __UpdateCarsAndRoadSigns(float dt, bool& bUpdateFlag);

	// Init road sign
	void __InitRoadSigns();

	// Generate all bricks
	void __UpdateAllBricksState();

private:
	enum
	{
		CAR_ROW_COUNT = 4,
		CAR_COLUMN_COUNT = 3,
		CAR_COLUMN_START_INDEX = 1 + CAR_COLUMN_COUNT / 2,
		CAR_COLUMN_END_INDEX = (COLUMN_COUNT - 1) - CAR_COLUMN_COUNT / 2,
		CAR_COUNT_MAX = 10,
		CAR_ROAD_COUNT = 4,
		CAR_DISTANCE = ROW_COUNT / 2 + 1,
		CAR_SELF_ROW_COUNT = ROW_COUNT - CAR_ROW_COUNT,
		BEYOND_CAR_ADD_SCORE = 10,
		CAR_MOVE_INTERVAL = 160,
		ROAD_SIGN_BRICK_COUNT = 3,
		ROAD_SIGN_SEP_COUNT = 1,
		FINISH_RACE_SCORE_ADD_COUNT = 16,
	};

	struct _TCarData
	{
		POSITION stPos;
		bool bValidFlag;
	};

	typedef	list<int>		TList_RoadSignPos;

private:
	// Enemy car data
	_TCarData				m_arrCarData[CAR_COUNT_MAX];

	// Road sign data
	TList_RoadSignPos		m_lsRoadSignPos;

	// Car update interval
	int						m_nUpdateInterval;

	// Self car col index
	int						m_nSelfCarColIdx;

	// Speed up flag
	bool					m_bSpeedUpFlag;

	// Add score count in the normal stage
	int						m_nAddScoreCount;
};