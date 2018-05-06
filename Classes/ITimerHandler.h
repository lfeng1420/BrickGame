#pragma once

// 定时器回调
class ITimerHandler
{
public:
	virtual void OnTimer(int nTimerID) = 0;
};
