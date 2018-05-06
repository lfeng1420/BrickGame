#pragma once

class ITimerHandler;

class CTimerManager : public Ref
{
public:
	CTimerManager();
	~CTimerManager();

	// Set timer
	bool SetTimer(ITimerHandler* pHandler, int nTimerID, int nInterval, int nCallTimes, const char* szDesc);

	// Kill timer
	void KillTimer(ITimerHandler* pHandler, int nTimerID);

	// Update per frame
	void update(float dt);

public:
	// Create
	bool Create();

	// Close
	void Close();

private:
	struct _TTimerData
	{
		int nTimerID;
		int nInterval;
		ITimerHandler* pHandler;
		std::string strDesc;
		int nCurInterval;
		int nCallTimes;
	};

	typedef		list<_TTimerData>		TList_Timer;
	typedef		TList_Timer::iterator	TList_TimerIter;

private:
	// Find timer
	_TTimerData* __GetTimerData(int nTimerID, ITimerHandler* pHandler);

	// Remove timer
	void __RemoveTimerData(int nTimerID, ITimerHandler* pHandler);

	// Update all timer
	void __UpdateAllTimerData(int nInterval);

private:
	// Timer list
	TList_Timer		m_lsTimer;
};

