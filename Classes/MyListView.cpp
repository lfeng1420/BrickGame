#include "stdafx.h"
#include "MyListView.h"


cocos2d::ui::ListView* CMyListView::create()
{
	CMyListView* widget = new CMyListView();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}


void CMyListView::handleMoveLogic(Touch *touch)
{
	Vec2 delta = touch->getLocation() - touch->getPreviousLocation();
	switch (_direction)
	{
	case Direction::HORIZONTAL: // vertical
	{
		scrollChildren(delta.y, 0.0f);
		break;
	}
	case Direction::VERTICAL: // horizontal
	{
		scrollChildren(0.0f, delta.x);
		break;
	}
	case Direction::BOTH: // both
	{
		scrollChildren(delta.x, delta.y);
		break;
	}
	default:
		break;
	}
}
