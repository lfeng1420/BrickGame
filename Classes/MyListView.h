#pragma once

class CMyListView : public ListView
{
public:
	// Create
	static ListView* create();

	// Handle move logic
	virtual void handleMoveLogic(Touch *touch);
};