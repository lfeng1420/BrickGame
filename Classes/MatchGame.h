#pragma once
#include "GameBase.h"

class CMatchGame : public CGameBase
{
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
	// Init src / dest shapes
	void __InitSrcDestShapes();

	// Random shape
	void __RandomShapes(int* arrShapes);

	// Update all bricks state
	void __UpdateAllBricksState();

	// Update shapes position
	bool __UpdateShapes(float dt, bool bSelfOnlyFlag = false);

	// Get shape move interval
	int __GetShapeMoveInterval(bool bDestShapeFlag);

	// Get dismatch shape index
	int __GetDismatchShapeIdx();

	// Handle match result
	void __HandleMatchResult();

private:
	enum
	{
		SHAPE_COUNT_MAX = 3,
		DEST_SHAPE_MOVE_INTERVAL = 1000,
		SRC_SHAPE_MOVE_INTERVAL = 30,
		SHAPE_UPDATE_INTERVAL = 60,
		SHAPE_MATCH_ADD_SCORE = 10,
		REQUIRE_MATCH_SUCC_COUNT = 10,
	};

	struct _TShapeData
	{
		int arrShapeID[SHAPE_COUNT_MAX];
		int nRowIdx;
		int nInterval;
	};

private:
	// Dest shapes
	_TShapeData		m_stDestShape;

	// Self shapes
	_TShapeData		m_stSrcShape;

	// Match succ count
	int				m_nMatchSuccCount;
};