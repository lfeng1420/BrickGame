#pragma once

class CBarrierLayer;

class CSetupVolume : public Layer
{
public:
	CSetupVolume();
	virtual ~CSetupVolume();

	// Init
	virtual bool init();

	// CREATE_FUNC macro
	CREATE_FUNC(CSetupVolume);

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
	};

protected:
	// Slider
	ControlSlider* m_pBGMSlider;
	ControlSlider* m_pEffectSlider;

	// Label
	Label* m_pBGMVolume;
	Label* m_pEffetVolume;
	Label* m_pBGMLabel;
	Label* m_pEffectLabel;

	// Background layer
    MenuItemSprite*	m_pBGSprite;

	// Close button
	MenuItemSprite* m_pCloseMenuItem;
	Menu*			m_pMenu;

	// Barrier layer
	CBarrierLayer*	m_pBarrierLayer;
};

