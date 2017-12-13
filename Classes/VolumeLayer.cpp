#include "VolumeLayer.h"
#include "GeneralManager.h"
#include "SetupLayer.h"
#include "BarrierLayer.h"


CVolumeLayer::CVolumeLayer()
{
}


CVolumeLayer::~CVolumeLayer()
{
}


bool CVolumeLayer::init()
{
	CC_RETURN_FALSE_IF(!Layer::init());

	Size visibleSize = GET_VISIBLESIZE();
	bool bNightMode = GET_BOOLVALUE("NIGHTMODE", false);
	CGeneralManager* pManager = CGeneralManager::getInstance();
	int nLangID = (CCApplication::getInstance()->getCurrentLanguage() != LanguageType::CHINESE);

	//BarrierLayer
	m_pBarrierLayer = CBarrierLayer::create();
	m_pBarrierLayer->setPosition(Vec2::ZERO);
	this->addChild(m_pBarrierLayer);

	//背景
	m_pBGSprite = CREATE_SPRITEWITHNAME(pManager->GetSpriteName("volumebg.png", bNightMode));
	m_pBGSprite->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	Size bgSprSize = GET_CONTENTSIZE(m_pBGSprite);
	this->addChild(m_pBGSprite);

	//按钮
	string strCloseSprName = pManager->GetSpriteName("close.png", bNightMode);
	m_pCloseMenuItem = MenuItemSprite::create(CREATE_SPRITEWITHNAME(strCloseSprName), CREATE_SPRITEWITHNAME(strCloseSprName), CC_CALLBACK_1(CVolumeLayer::onButtonEvent, this));
	Size closeSize = GET_CONTENTSIZE(m_pCloseMenuItem);
	m_pCloseMenuItem->setPosition(visibleSize.width * 0.5f + bgSprSize.width * 0.5f - closeSize.width * 0.5f, visibleSize.height * 0.5f + bgSprSize.height * 0.5f - closeSize.height * 0.5f);
	Menu* pMenu = Menu::create(m_pCloseMenuItem, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);

	//背景音乐Label
	m_pBGMLabel = Label::createWithSystemFont(pManager->GetStringByID(nLangID, STRNAME_BGM), FONT_NAME, LABEL_FONT_SIZE);
	Size bgmLabelSize = GET_CONTENTSIZE(m_pBGMLabel);
	this->addChild(m_pBGMLabel);
	//值Label
	int nBGMVolume = GET_INTVALUE("BGM_VOLUME", 100);
	char szValue[10] = {0};
	sprintf(szValue, "%d", nBGMVolume);
	m_pBGMVolume = Label::createWithSystemFont(szValue, FONT_NAME, VALUE_FONT_SIZE);
	Size bgmVolumeSize = GET_CONTENTSIZE(m_pBGMVolume);
	this->addChild(m_pBGMVolume);
	//Slider
	m_pBGMSlider = ControlSlider::create(
		CREATE_SPRITEWITHNAME(pManager->GetSpriteName("sliderbg.png", bNightMode)),
		CREATE_SPRITEWITHNAME("sliderprogress.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png")
		);
	Size bgmSliderSize = GET_CONTENTSIZE(m_pBGMSlider);
	m_pBGMSlider->setMaximumValue(100);
	m_pBGMSlider->setMinimumValue(0);
	m_pBGMSlider->setValue(nBGMVolume);
	m_pBGMSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(CVolumeLayer::onSliderValueChanged), Control::EventType::VALUE_CHANGED);
	this->addChild(m_pBGMSlider);

	//音效
	m_pEffectLabel = Label::createWithSystemFont(pManager->GetStringByID(nLangID, STRNAME_EFFECT), FONT_NAME, LABEL_FONT_SIZE);
	Size effectLabelSize = GET_CONTENTSIZE(m_pEffectLabel);
	this->addChild(m_pEffectLabel);
	//值Label
	int nEffectVolume = GET_INTVALUE("EFFECT_VOLUME", 100);
	sprintf(szValue, "%d", nEffectVolume);
	m_pEffetVolume = Label::createWithSystemFont(szValue, FONT_NAME, VALUE_FONT_SIZE);
	Size effectVolumeSize = GET_CONTENTSIZE(m_pEffetVolume);
	this->addChild(m_pEffetVolume);
	//Slider
	m_pEffectSlider = ControlSlider::create(
		CREATE_SPRITEWITHNAME(pManager->GetSpriteName("sliderbg.png", bNightMode)),
		CREATE_SPRITEWITHNAME("sliderprogress.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png"),
		CREATE_SPRITEWITHNAME("sliderthumb.png")
		);
	Size effectSliderSize = GET_CONTENTSIZE(m_pEffectSlider);
	m_pEffectSlider->setMaximumValue(100);
	m_pEffectSlider->setMinimumValue(0);
	m_pEffectSlider->setValue(nEffectVolume);
	m_pEffectSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(CVolumeLayer::onSliderValueChanged), Control::EventType::VALUE_CHANGED);
	this->addChild(m_pEffectSlider);

	//设置位置
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
	//BGM
	m_pBGMLabel->setPosition(fLabelX, fBGMY);
	m_pBGMSlider->setPosition(fSliderX, fBGMY);
	m_pBGMVolume->setPosition(fVolumeX, fBGMY);
	//Effect
	m_pEffectLabel->setPosition(fLabelX, fEffectY);
	m_pEffectSlider->setPosition(fSliderX, fEffectY);
	m_pEffetVolume->setPosition(fVolumeX, fEffectY);
	//Label Color
	m_pBGMLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	m_pEffectLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	m_pBGMVolume->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	m_pEffetVolume->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);

	//更新位置
	updatePos();

	return true;
}


void CVolumeLayer::updatePos()
{
	Size visibleSize = GET_VISIBLESIZE();
	Size bgSprSize = GET_CONTENTSIZE(m_pBGSprite);
	Size closeSize = GET_CONTENTSIZE(m_pCloseMenuItem);
	Size bgmLabelSize = GET_CONTENTSIZE(m_pBGMLabel);
	Size bgmSliderSize = GET_CONTENTSIZE(m_pBGMSlider);
	Size bgmVolumeSize = GET_CONTENTSIZE(m_pBGMVolume);
	Size effectLabelSize = GET_CONTENTSIZE(m_pEffectLabel);
	Size effectSliderSize = GET_CONTENTSIZE(m_pEffectSlider);
	Size effectVolumeSize = GET_CONTENTSIZE(m_pEffetVolume);

	//BarrierLayer
	if (GET_BOOLVALUE("PORTRAIT", true))
	{
		//bg
		m_pBGSprite->setRotation(0);
		//Close
		m_pCloseMenuItem->setRotation(0);
		m_pCloseMenuItem->setPosition(visibleSize.width * 0.5f + bgSprSize.width * 0.5f - closeSize.width * 0.5f, visibleSize.height * 0.5f + bgSprSize.height * 0.5f - closeSize.height * 0.5f);

		//设置位置
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
		//BGM
		m_pBGMLabel->setRotation(0);
		m_pBGMSlider->setRotation(0);
		m_pBGMVolume->setRotation(0);
		m_pBGMLabel->setPosition(fLabelX, fBGMY);
		m_pBGMSlider->setPosition(fSliderX, fBGMY);
		m_pBGMVolume->setPosition(fVolumeX, fBGMY);
		//Effect
		m_pEffectLabel->setRotation(0);
		m_pEffectSlider->setRotation(0);
		m_pEffetVolume->setRotation(0);
		m_pEffectLabel->setPosition(fLabelX, fEffectY);
		m_pEffectSlider->setPosition(fSliderX, fEffectY);
		m_pEffetVolume->setPosition(fVolumeX, fEffectY);
	}
	else
	{
		//bg
		m_pBGSprite->setRotation(90);
		//close
		m_pCloseMenuItem->setRotation(90);
		m_pCloseMenuItem->setPosition(visibleSize.width * 0.5f + bgSprSize.height * 0.5f - closeSize.height * 0.5f, visibleSize.height * 0.5f - bgSprSize.width * 0.5f + closeSize.width * 0.5f);
	
		float fMaxWidth = max(effectLabelSize.height, effectSliderSize.height);
		float fLabelMaxHeight = max(bgmLabelSize.width, effectLabelSize.width);
		float fTotalHeight = fLabelMaxHeight + X_PADDING + bgmSliderSize.width + X_PADDING + bgmVolumeSize.width;
		float fXPadding = (bgSprSize.width - fMaxWidth * 2) / 8;
		float fYPadding = (visibleSize.height - fTotalHeight) * 0.5f;
		float fBGMX = visibleSize.width * 0.5f + fXPadding + fMaxWidth * 0.5f;
		float fEffectX = visibleSize.width * 0.5f - fXPadding - fMaxWidth * 0.5f;
		float fLabelY = fYPadding + fLabelMaxHeight * 0.5f + X_PADDING * 2 + bgmSliderSize.width + bgmVolumeSize.width;
		float fSliderY = fYPadding + bgmVolumeSize.width + X_PADDING + bgmSliderSize.width * 0.5f;
		float fVolumeY = fYPadding + bgmVolumeSize.width * 0.5f;
		//BGM
		m_pBGMLabel->setRotation(90);
		m_pBGMSlider->setRotation(90);
		m_pBGMVolume->setRotation(90);
		m_pBGMLabel->setPosition(fBGMX, fLabelY);
		m_pBGMSlider->setPosition(fBGMX, fSliderY);
		m_pBGMVolume->setPosition(fBGMX, fVolumeY);
		//Effect
		m_pEffectLabel->setRotation(90);
		m_pEffectSlider->setRotation(90);
		m_pEffetVolume->setRotation(90);
		m_pEffectLabel->setPosition(fEffectX, fLabelY);
		m_pEffectSlider->setPosition(fEffectX, fSliderY);
		m_pEffetVolume->setPosition(fEffectX, fVolumeY);
	}
}


void CVolumeLayer::onSliderValueChanged(Ref* pSender, Control::EventType enEvent)
{
	int nBGMVolume = m_pBGMSlider->getValue();
	int nEffectVolume = m_pEffectSlider->getValue();
	SET_INTVALUE("BGM_VOLUME", nBGMVolume);
	SET_INTVALUE("EFFECT_VOLUME", nEffectVolume);
	AUDIO_INSTANCE()->setBackgroundMusicVolume(nBGMVolume * 0.01f);
	AUDIO_INSTANCE()->setEffectsVolume(nEffectVolume * 0.01f);

	//更新Label
	char szVolume[10] = { 0 };
	sprintf(szVolume, "%d", nBGMVolume);
	m_pBGMVolume->setString(szVolume);
	sprintf(szVolume, "%d", nEffectVolume);
	m_pEffetVolume->setString(szVolume);
}


void CVolumeLayer::onButtonEvent(Ref* pSender)
{
	removeFromParent();
}
