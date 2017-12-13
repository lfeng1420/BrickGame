#pragma once
#include "BGGlobal.h"

class CBarrierLayer : public Layer
{
public:
	CBarrierLayer();
	~CBarrierLayer();

	virtual bool init();

	//设置触摸激活标记
	void SetListenerEnabled(bool bEnabledFlag);

	CREATE_FUNC(CBarrierLayer);

private:
	//Listener
	EventListenerTouchOneByOne* m_pListener;
};