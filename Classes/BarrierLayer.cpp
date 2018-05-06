#include "stdafx.h"
#include "BarrierLayer.h"


bool CBarrierLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(25, 25, 25, 150)))
	{
		return false;
	}

	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = [](Touch* pTouch, Event* pEvent) {return true; };
	pListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
	return true;
}