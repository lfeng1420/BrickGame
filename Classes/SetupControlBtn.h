#pragma once

class CBarrierLayer;

class CSetupControlBtn : public Layer
{
public:
    CSetupControlBtn();
	virtual ~CSetupControlBtn();

	// Init
	virtual bool init();

	// CREATE_FUNC macro
	CREATE_FUNC(CSetupControlBtn);

	// Init UI
	virtual void InitUI();

private:
	// Create UI
	void __CreateUI();

	// Slider value change
	void __OnSliderValueChanged(Ref* pSender, Control::EventType enEvent);

	// Click button
	void __OnButtonEvent(Ref* pSender);

protected:
	enum
	{
		DEFAULT_HEIGHT = 150,

		LABEL_FONT_SIZE = 24,
		VALUE_FONT_SIZE = 24,

		X_PADDING = 25,

        DIRBTN_SCALE_MIN = 80,
        DIRBTN_SCALE_MAX = 100,
        DIRBTN_YOFFSET_MIN = -50,
        DIRBTN_YOFFSET_MAX = 50,
	};

protected:
	// Slider
	ControlSlider* m_pScaleSlider;
	ControlSlider* m_pOffsetSlider;

	// Label
	Label* m_pScaleVolume;
	Label* m_pOffsetVolume;
	Label* m_pScaleLabel;
	Label* m_pOffsetLabel;

	// Background layer
    MenuItemSprite*	m_pBGSprite;

	// Close button
	MenuItemSprite* m_pCloseMenuItem;
	Menu*			m_pMenu;

	// Barrier layer
	CBarrierLayer*	m_pBarrierLayer;
};

