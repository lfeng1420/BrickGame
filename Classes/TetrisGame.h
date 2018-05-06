#pragma once
#include "GameBase.h"

class CTetrisGame : public CGameBase
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

	// Save data
	virtual bool SaveData(bool bForceFlag);

public:
	// Get shape count
	virtual int GetShapeCount();

	// On tetris move finish
	virtual void OnTetrisMoveFinish();

	// Check game over
	virtual bool CheckShapePos(const POSITION& stNextPos, int nShapeID);

	// Get shape brick state
	virtual bool GetShapeBrickState(int nShapeID, int nRowIdx, int nColIdx);

	// Update other
	virtual void UpdateOther(float dt, bool& bUpdateFlag);

	// Update valid brick state
	virtual void UpdateValidBricksState();

	// Load tetris record
	virtual bool LoadTetrisData();

	// Save tetris record
	virtual bool SaveTetrisData(bool bForceFlag = false);

public:
	// Remove rows
	bool RemoveRows();

	// Update tetris shape
	void UpdateShape(int nShapeID, const POSITION& stNewPos, bool bSyncFlag = false);

	// Random tetris shape
	virtual void RandomShape(bool bUpdateFlag = false, int nCurShapeID = TETRIS_SHAPE_COUNT, int nNextShapeID = TETRIS_SHAPE_COUNT);

	// Get shape row offset
	int GetShapeRowOffset(int nShapeID);

	// Get shape column offset
	int GetShapeColOffset(int nShapeID);

	// Get shape brick state (origin)
	bool GetShapeBrickOriginState(int nShapeID, int nRowIdx, int nColIdx);

private:
	// Update tetris
	void __TetrisControlMove(float dt, bool& bUpdateFlag);

	// Tetris move down
	void __TetrisMoveDown(float dt, bool& bUpdateFlag);

	// Check one row full
	bool __CheckOneRowFull(int nRowIdx);

	// Remove one row
	void __RemoveOneRow(int nRowIdx);

	// Get next shape id
	int __GetNextShapeID();

	//////////////////////////////////////////////////////////////////////////
	// Get move interval
	int __GetTetrisMoveInterval();

public:
	enum
	{
		REMOVE_ONE_ROW_ADD_SCORE = 10,
		TETRIS_MOVE_INTERVAL = 500,
		CONTROL_MOVE_INTERVAL = 95,
	};

	struct _TTetrisData
	{
		int nShapeID;
		POSITION stPos;
		int nMoveInterval;
		int nDir;
		int nControlInterval;
		bool bSpeedUpFlag;
	};

protected:
	// Current tetris data
	_TTetrisData		m_stTetrisData;

	// Next shape
	int					m_nNextShapeID;
};