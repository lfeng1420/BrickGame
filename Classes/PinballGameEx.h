#pragma once
#include "PinballGame.h"

class CPinballGameEx : public CPinballGame
{
public:
	// Start game
	void Start();

	// Other udpate logic
	void UpdateOther(float dt, bool& bUpdateFlag);

	// Get game id
	EnGameID GetGameID();

private:
	// Get generate bricks interval
	int __GetGenBricksInterval();

private:
	enum
	{
		GEN_BRICKS_INTERVAL = 500,
	};

private:
	// Generate bricks interval
	int				m_nGenInterval;

	// Move dir
	bool			m_bMoveLeftFlag;
};