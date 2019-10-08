#include "stdafx.h"
#include "SetupControlBtn.h"
#include "BarrierLayer.h"


CSetupControlBtn::CSetupControlBtn()
{
}


CSetupControlBtn::~CSetupControlBtn()
{
}


bool CSetupControlBtn::init()
{
	if (!Layer::init())
	{
		return false;
	}

	__CreateUI();
	InitUI();

	return true;
}


void CSetupControlBtn::__CreateUI()
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
		CC_CALLBACK_1(CSetupControlBtn::__OnButtonEvent, this));

	// Menu
	m_pMenu = Menu::create(m_pBGSprite, m_pCloseMenuItem, nullptr);
	m_pMenu->setPosition(Vec2::ZERO);
	this->addChild(m_pMenu);

	// Brick scale label
	m_pScaleLabel = Label::createWithSystemFont(CGlobalFunc::GetString(STRID_DIRBTNSCALE), FONT_NAME, LABEL_FONT_SIZE);
	this->addChild(m_pScaleLabel);
	// Value label
	int nBrickScale = GET_INTVALUE("DIRBTN_SCALE", DIRBTN_SCALE_MAX);
    string strValue = std::to_string(nBrickScale);
	m_pScaleVolume = Label::createWithSystemFont(strValue, FONT_NAME, VALUE_FONT_SIZE);
	this->addChild(m_pScaleVolume);
	//Slider
	m_pScaleSlider = ControlSlider::create(
		CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("sliderbg.png")),
		CREATE_SPRITEWITHNAME("sliderprogress.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png")
	);
	m_pScaleSlider->setMaximumValue(DIRBTN_SCALE_MAX);
	m_pScaleSlider->setMinimumValue(DIRBTN_SCALE_MIN);
	m_pScaleSlider->setValue(nBrickScale);
	m_pScaleSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(CSetupControlBtn::__OnSliderValueChanged), Control::EventType::VALUE_CHANGED);
	this->addChild(m_pScaleSlider);

	// Offset
	m_pOffsetLabel = Label::createWithSystemFont(CGlobalFunc::GetString(STRID_DIRBTNYOFFSET), FONT_NAME, LABEL_FONT_SIZE);
	this->addChild(m_pOffsetLabel);
	// Value
	int nOffsetVolume = GET_INTVALUE("DIRBTN_YOFFSET", 0);
    strValue = std::to_string(nOffsetVolume);
	m_pOffsetVolume = Label::createWithSystemFont(strValue, FONT_NAME, VALUE_FONT_SIZE);
	this->addChild(m_pOffsetVolume);
	//Slider
	m_pOffsetSlider = ControlSlider::create(
		CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("sliderbg.png")),
		CREATE_SPRITEWITHNAME("sliderprogress.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png")
	);
	m_pOffsetSlider->setMaximumValue(DIRBTN_YOFFSET_MAX);
	m_pOffsetSlider->setMinimumValue(DIRBTN_YOFFSET_MIN);
	m_pOffsetSlider->setValue(nOffsetVolume);
	m_pOffsetSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(CSetupControlBtn::__OnSliderValueChanged), Control::EventType::VALUE_CHANGED);
	this->addChild(m_pOffsetSlider);

	bool bNightMode = GET_BOOLVALUE("NIGHTMODE", false);
	//Label Color
	m_pScaleLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	m_pOffsetLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	m_pScaleVolume->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	m_pOffsetVolume->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
}


void CSetupControlBtn::InitUI()
{
	Size visibleSize = GET_VISIBLESIZE();
	Size bgSprSize = GET_CONTENTSIZE(m_pBGSprite);
	Size closeSize = GET_CONTENTSIZE(m_pCloseMenuItem);
	Size bgmLabelSize = GET_CONTENTSIZE(m_pScaleLabel);
	Size bgmVolumeSize = GET_CONTENTSIZE(m_pScaleVolume);
	Size bgmSliderSize = GET_CONTENTSIZE(m_pScaleSlider);
	Size effectLabelSize = GET_CONTENTSIZE(m_pOffsetLabel);
	Size effectVolumeSize = GET_CONTENTSIZE(m_pOffsetVolume);
	Size effectSliderSize = GET_CONTENTSIZE(m_pOffsetSlider);

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
	m_pScaleLabel->setPosition(fLabelX, fBGMY);
	m_pScaleSlider->setPosition(fSliderX, fBGMY);
	m_pScaleVolume->setPosition(fVolumeX, fBGMY);
	//Effect
	m_pOffsetLabel->setPosition(fLabelX, fEffectY);
	m_pOffsetSlider->setPosition(fSliderX, fEffectY);
	m_pOffsetVolume->setPosition(fVolumeX, fEffectY);
}


void CSetupControlBtn::__OnSliderValueChanged(Ref* pSender, Control::EventType enEvent)
{
	int nDirBtnScale = m_pScaleSlider->getValue();
	int nDirBtnYOffset = m_pOffsetSlider->getValue();
	SET_INTVALUE("DIRBTN_SCALE", nDirBtnScale);
	SET_INTVALUE("DIRBTN_YOFFSET", nDirBtnYOffset);

	// Update label
    string strValue = std::to_string(nDirBtnScale);
	m_pScaleVolume->setString(strValue);
    strValue = std::to_string(nDirBtnYOffset);
	m_pOffsetVolume->setString(strValue);
}


void CSetupControlBtn::__OnButtonEvent(Ref* pSender)
{
	removeFromParent();
}
