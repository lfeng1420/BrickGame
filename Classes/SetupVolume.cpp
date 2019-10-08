#include "stdafx.h"
#include "SetupVolume.h"
#include "BarrierLayer.h"


CSetupVolume::CSetupVolume()
{
}


CSetupVolume::~CSetupVolume()
{
}


bool CSetupVolume::init()
{
	if (!Layer::init())
	{
		return false;
	}

	__CreateUI();
	InitUI();

	return true;
}


void CSetupVolume::__CreateUI()
{
	Size visibleSize = GET_VISIBLESIZE();

	//BarrierLayer
	m_pBarrierLayer = CBarrierLayer::create();
	this->addChild(m_pBarrierLayer);

	// Bakcground
	m_pBGSprite = MenuItemSprite::create(
        CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("volumebg.png")),
        nullptr,
        nullptr
    );

	// Menu item
	string strCloseSprName = CGlobalFunc::GetSpriteNameWithMode("close.png");
	m_pCloseMenuItem = MenuItemSprite::create(
		CREATE_SPRITEWITHNAME(strCloseSprName),
		CREATE_SPRITEWITHNAME(strCloseSprName),
		CC_CALLBACK_1(CSetupVolume::__OnButtonEvent, this));

	// Menu
	m_pMenu = Menu::create(m_pBGSprite, m_pCloseMenuItem, nullptr);
	m_pMenu->setPosition(Vec2::ZERO);
	this->addChild(m_pMenu);

	// BGM Label
	m_pBGMLabel = Label::createWithSystemFont(CGlobalFunc::GetString(STRID_BGM), FONT_NAME, LABEL_FONT_SIZE);
	this->addChild(m_pBGMLabel);
	// Value label
	int nBGMVolume = GET_INTVALUE("BGM_VOLUME", 100);
	char szValue[10] = { 0 };
	sprintf(szValue, "%d", nBGMVolume);
	m_pBGMVolume = Label::createWithSystemFont(szValue, FONT_NAME, VALUE_FONT_SIZE);
	this->addChild(m_pBGMVolume);
	//Slider
	m_pBGMSlider = ControlSlider::create(
		CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("sliderbg.png")),
		CREATE_SPRITEWITHNAME("sliderprogress.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png")
	);
	m_pBGMSlider->setMaximumValue(100);
	m_pBGMSlider->setMinimumValue(0);
	m_pBGMSlider->setValue(nBGMVolume);
	m_pBGMSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(CSetupVolume::__OnSliderValueChanged), Control::EventType::VALUE_CHANGED);
	this->addChild(m_pBGMSlider);

	// Effect
	m_pEffectLabel = Label::createWithSystemFont(CGlobalFunc::GetString(STRID_EFFECT), FONT_NAME, LABEL_FONT_SIZE);
	this->addChild(m_pEffectLabel);
	// Value
	int nEffectVolume = GET_INTVALUE("EFFECT_VOLUME", 100);
	sprintf(szValue, "%d", nEffectVolume);
	m_pEffetVolume = Label::createWithSystemFont(szValue, FONT_NAME, VALUE_FONT_SIZE);
	this->addChild(m_pEffetVolume);
	//Slider
	m_pEffectSlider = ControlSlider::create(
		CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("sliderbg.png")),
		CREATE_SPRITEWITHNAME("sliderprogress.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png")
	);
	m_pEffectSlider->setMaximumValue(100);
	m_pEffectSlider->setMinimumValue(0);
	m_pEffectSlider->setValue(nEffectVolume);
	m_pEffectSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(CSetupVolume::__OnSliderValueChanged), Control::EventType::VALUE_CHANGED);
	this->addChild(m_pEffectSlider);

	bool bNightMode = GET_BOOLVALUE("NIGHTMODE", false);
	//Label Color
	m_pBGMLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	m_pEffectLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	m_pBGMVolume->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	m_pEffetVolume->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
}


void CSetupVolume::InitUI()
{
	Size visibleSize = GET_VISIBLESIZE();
	Size bgSprSize = GET_CONTENTSIZE(m_pBGSprite);
	Size closeSize = GET_CONTENTSIZE(m_pCloseMenuItem);
	Size bgmLabelSize = GET_CONTENTSIZE(m_pBGMLabel);
	Size bgmVolumeSize = GET_CONTENTSIZE(m_pBGMVolume);
	Size bgmSliderSize = GET_CONTENTSIZE(m_pBGMSlider);
	Size effectLabelSize = GET_CONTENTSIZE(m_pEffectLabel);
	Size effectVolumeSize = GET_CONTENTSIZE(m_pEffetVolume);
	Size effectSliderSize = GET_CONTENTSIZE(m_pEffectSlider);

	float fMaxHeight = max(effectLabelSize.height, effectSliderSize.height);
	float fLabelMaxWidth = max(bgmLabelSize.width, effectLabelSize.width);
	float fTotalWidth = fLabelMaxWidth + X_PADDING + bgmSliderSize.width + X_PADDING + bgmVolumeSize.width;
	float fYPadding = (bgSprSize.height - fMaxHeight * 2) / 8;
	float fXPadding = (visibleSize.width - fTotalWidth) * 0.5f;
	float fBGMY = visibleSize.height * 0.5f + fYPadding + fMaxHeight * 0.5f;
	float fEffectY = visibleSize.height * 0.5f - fYPadding - fMaxHeight * 0.5f;
	float fLabelX = fXPadding + fLabelMaxWidth * 0.5f;
	float fSliderX = fXPadding + fLabelMaxWidth + X_PADDING + bgmSliderSize.width * 0.5f;
	float fVolumeX = fXPadding + fLabelMaxWidth + X_PADDING * 2 + bgmSliderSize.width + bgmVolumeSize.width * 0.5f;

	// Barrier layer
	m_pBarrierLayer->setPosition(Vec2::ZERO);
	// Background
	m_pBGSprite->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	// Close button
	m_pCloseMenuItem->setPosition(visibleSize.width * 0.5f + bgSprSize.width * 0.5f - closeSize.width * 0.5f,
		visibleSize.height * 0.5f + bgSprSize.height * 0.5f - closeSize.height * 0.5f);
	//BGM
	m_pBGMLabel->setPosition(fLabelX, fBGMY);
	m_pBGMSlider->setPosition(fSliderX, fBGMY);
	m_pBGMVolume->setPosition(fVolumeX, fBGMY);
	//Effect
	m_pEffectLabel->setPosition(fLabelX, fEffectY);
	m_pEffectSlider->setPosition(fSliderX, fEffectY);
	m_pEffetVolume->setPosition(fVolumeX, fEffectY);
}


void CSetupVolume::__OnSliderValueChanged(Ref* pSender, Control::EventType enEvent)
{
	int nBGMVolume = m_pBGMSlider->getValue();
	int nEffectVolume = m_pEffectSlider->getValue();
	SET_INTVALUE("BGM_VOLUME", nBGMVolume);
	SET_INTVALUE("EFFECT_VOLUME", nEffectVolume);
	AUDIO_INSTANCE()->setBackgroundMusicVolume(nBGMVolume * 0.01f);
	AUDIO_INSTANCE()->setEffectsVolume(nEffectVolume * 0.01f);

	// Update label
	char szVolume[10] = { 0 };
	sprintf(szVolume, "%d", nBGMVolume);
	m_pBGMVolume->setString(szVolume);
	sprintf(szVolume, "%d", nEffectVolume);
	m_pEffetVolume->setString(szVolume);
}


void CSetupVolume::__OnButtonEvent(Ref* pSender)
{
	removeFromParent();
}
