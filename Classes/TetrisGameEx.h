#pragma once
#include "TetrisGame.h"

class CTetrisGameEx : public CTetrisGame
{
public:
	// Start
	void Start();

	// Get shape count
	int GetShapeCount();

	// On tetris move finish
	void OnTetrisMoveFinish();

	// Get shape brick state
	bool GetShapeBrickState(int nShapeID, int nRowIdx, int nColIdx);

	// Check shape pos
	bool CheckShapePos(const POSITION& stNextPos, int nShapeID);

	// Button event
	void OnButtonEvent(const SEventContextButton* pButtonEvent);

	// Update other
	void UpdateOther(float dt, bool& bUpdateFlag);

	// Update valid brick state
	void UpdateValidBricksState();

	// Get game id
	EnGameID GetGameID();

	// Load tetris record
	bool LoadTetrisData();

	// Save tetris record
	bool SaveTetrisData(bool bForceFlag = false);

private:
	enum
	{
		SHAPE_FLASH_INTERVAL = 90,
		SHAPEID_ADD_BRICK = 19,
		SHAPEID_REMOVE_BRICK = 20,
		SHAPEID_BOOM = 21,
		SHAPEID_DOT = 22,
		BOOM_REMOVE_ROW_COUNT = 2,
	};

private:
	// Get not full row index
	int __GetCurFillBrickRowIdx();

	// Get operate row index ( for add/remove shape)
	int __GetOperateRowIdx();

	// On boom shape move finish
	void __OnBoomShapeMoveFinish(bool bClearBricksFlag = true);

private:
	// Shape visible flag
	bool m_bVisibleFlag;

	// Interval
	int m_nFlashInterval;
};