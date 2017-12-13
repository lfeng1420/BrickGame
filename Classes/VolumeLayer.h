#pragma once
#include "BGGlobal.h"

class CBarrierLayer;

class CVolumeLayer : public Layer
{
public:
	CVolumeLayer();
	~CVolumeLayer();

	//初始化
	virtual bool init();

	//更新位置
	void updatePos();

	CREATE_FUNC(CVolumeLayer);

private:
	//滑动条变化
	void onSliderValueChanged(Ref* pSender, Control::EventType enEvent);

	//按钮触发
	void onButtonEvent(Ref* pSender);

private:
	enum
	{
		DEFAULT_HEIGHT = 150,

		LABEL_FONT_SIZE = 24,
		VALUE_FONT_SIZE = 24,

		X_PADDING = 25,
	};

private:
	//滑动条
	ControlSlider* m_pBGMSlider;
	ControlSlider* m_pEffectSlider;

	//当前值Label
	Label* m_pBGMVolume;
	Label* m_pEffetVolume;

	//Label
	Label* m_pBGMLabel;
	Label* m_pEffectLabel;

	//背景层
	Sprite*	m_pBGSprite;

	//关闭按钮
	MenuItemSprite* m_pCloseMenuItem;

	//阻挡层
	CBarrierLayer*	m_pBarrierLayer;
};

