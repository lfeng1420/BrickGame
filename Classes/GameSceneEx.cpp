#include "stdafx.h"
#include "GameSceneEx.h"


cocos2d::Scene* CGameSceneEx::CreateScene(const TGameSceneContext* pContext /*= nullptr*/)
{
	auto scene = Scene::create();
	auto layer = CGameSceneEx::create(pContext);
	scene->addChild(layer);
	scene->setTag(GAME_SCENE_TAG);
	return scene;
}


cocos2d::LayerColor* CGameSceneEx::create(const TGameSceneContext* pContext /*= nullptr*/)
{
	CGameSceneEx* pLayer = new CGameSceneEx();
	if (pLayer == nullptr)
	{
		return nullptr;
	}

	// First step
	pLayer->ImportContextFirst(pContext);

	// init
	if (!pLayer->init())
	{
		delete pLayer;
		return nullptr;
	}

	// Last step
	pLayer->ImportContextLast(pContext);

	pLayer->autorelease();
	return pLayer;
}


void CGameSceneEx::InitUI()
{
	float fBottomY = 0;
	float fTopY = 0;
	float fTempBottomY = 0;
	__InitAllBricksEx(fBottomY, fTopY);
	__InitBottomMenuEx(fTempBottomY);
	float fBottomCenterY = (fBottomY - fTempBottomY) * 0.5f + fTempBottomY;
	__InitControllerEx(fBottomCenterY, fTopY);
	__InitRightUIEx(fBottomCenterY, fTopY);
	__ApplyRightHandModeEx();

	// Tips
	__InitTips();
}


void CGameSceneEx::UpdateLevelOrSpeed(Vector<Sprite*> vecSpr, int& nOldVal, int nNewVal)
{
	if (nOldVal == nNewVal)
	{
		return;
	}

	char szSpriteName[10] = { 0 };
	Sprite* pFirstSpr = vecSpr.at(0);
	Sprite* pSecSpr = vecSpr.at(1);
	if (pFirstSpr == nullptr || pSecSpr == nullptr)
	{
		return;
	}

	if (nNewVal < 10)
	{
		sprintf(szSpriteName, "%d.png", nNewVal);
		pSecSpr->setSpriteFrame(CGlobalFunc::GetSpriteNameWithMode(szSpriteName));
		pFirstSpr->setVisible(false);
		pSecSpr->setVisible(true);
		float fPosY = pFirstSpr->getPositionY();
		Size sprSize = GET_CONTENTSIZE(pSecSpr);
		pSecSpr->setPositionY(fPosY - sprSize.width * 0.5f - LEVEL_SPEED_NUM_PADDING * 0.5f);
	}
	else
	{
		pFirstSpr->setSpriteFrame(CGlobalFunc::GetSpriteNameWithMode("1.png"));
		pFirstSpr->setVisible(true);
		pSecSpr->setSpriteFrame(CGlobalFunc::GetSpriteNameWithMode("0.png"));
		pSecSpr->setVisible(true);
		float fPosY = pFirstSpr->getPositionY();
		Size sprSize = GET_CONTENTSIZE(pSecSpr);
		pSecSpr->setPositionY(fPosY - sprSize.width - LEVEL_SPEED_NUM_PADDING);
	}

	// Update value
	nOldVal = nNewVal;
}


bool CGameSceneEx::AdjustClickBtnID(Vec2 pos, int& nBtnID)
{
	//只处理方向键
	if (nBtnID >= BTNID_DIRMAX)
	{
		return true;
	}

	Vec2 dis = pos - m_oControllerCenterPos;
	float fFactor = dis.x / dis.y;
	if (fFactor >= 1 || fFactor <= -1)
	{
		nBtnID = dis.x > 1e-6 ? BTNID_UP : BTNID_DOWN;
		return true;
	}

	if (fFactor < 1 && fFactor >= -1)
	{
		nBtnID = dis.y > 1e-6 ? BTNID_LEFT : BTNID_RIGHT;
		return true;
	}

	nBtnID = -1;
	return false;
}


void CGameSceneEx::__InitAllBricksEx(float& fBottomY, float& fTopY)
{
	Size visibleSize = GET_VISIBLESIZE();
	Size brickSize = Size::ZERO;
	fBottomY = 0;
	fTopY = 0;
	float fCurX = visibleSize.width;

	for (int nRowIdx = 0; nRowIdx < ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < COLUMN_COUNT; ++nColIdx)
		{
			int nBrickID = GET_BRICKID(nRowIdx, nColIdx);

			// Create sprite
			Sprite* pSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode(m_arrBrickState[nBrickID] ? "black.png" : "empty.png"));
			if (brickSize.equals(Size::ZERO))
			{
				brickSize = GET_CONTENTSIZE(pSpr);
				fTopY = visibleSize.height * 0.5f + brickSize.width * COLUMN_COUNT * 0.5f;
				fBottomY = visibleSize.height * 0.5f - brickSize.width * COLUMN_COUNT * 0.5f;
			}
			pSpr->setRotation(90);
			pSpr->setPosition(fCurX - brickSize.height * 0.5f, fTopY - nColIdx * brickSize.width - brickSize.width * 0.5f);
			pSpr->setTag(nBrickID);
			this->addChild(pSpr);
			m_vecBrickSpr.pushBack(pSpr);
		}

		fCurX -= brickSize.width;
	}
}


void CGameSceneEx::__InitControllerEx(float fBottomCenterY, float fTopY)
{
	Size visibleSize = GET_VISIBLESIZE();
	float fBtnInnerPadding = CONTROLLER_INNER_PADDING;
	float fControllerPadding = 10;
	const float CONTROLLER_SCALE_MAX = GET_INTVALUE("DIRBTN_SCALE", DIRBTN_DEFAULT_SCALE) / 100.0f;

	// Up button
	string strBtn0Name = CGlobalFunc::GetSpriteNameWithMode("btn_0.png");
	string strBtn1Name = CGlobalFunc::GetSpriteNameWithMode("btn_1.png");
	Button* pUpBtn = Button::create(strBtn0Name, strBtn1Name, "", Widget::TextureResType::PLIST);
	Size btnSize = GET_CONTENTSIZE(pUpBtn);
	float fControlBtnScale = (visibleSize.height - fTopY - fControllerPadding * 2) / (btnSize.height * 2);
	if (fControlBtnScale > CONTROLLER_SCALE_MAX)
	{
		fControlBtnScale = CONTROLLER_SCALE_MAX;
		fControllerPadding = (visibleSize.height - fTopY - CONTROLLER_SCALE_MAX * btnSize.height * 2 + CONTROLLER_INNER_PADDING) * 0.5f;
	}
	pUpBtn->setRotation(90);
	pUpBtn->setScale(fControlBtnScale);
	pUpBtn->addTouchEventListener(CC_CALLBACK_2(CGameSceneEx::__OnButtonEvent, this, BTNID_UP));
	pUpBtn->setCustomHighlightFlag(true);
	Size upBtnSize = btnSize * fControlBtnScale;

	// Right button
	Button* pRightBtn = Button::create(strBtn0Name, strBtn1Name, "", Widget::TextureResType::PLIST);
	pRightBtn->setScale(fControlBtnScale);
	pRightBtn->setRotation(180);
	pRightBtn->addTouchEventListener(CC_CALLBACK_2(CGameSceneEx::__OnButtonEvent, this, BTNID_RIGHT));
	pRightBtn->setCustomHighlightFlag(true);
	Size rightBtnSize = GET_CONTENTSIZE(pRightBtn) * fControlBtnScale;

	// Down button
	Button* pDownBtn = Button::create(strBtn0Name, strBtn1Name, "", Widget::TextureResType::PLIST);
	pDownBtn->setScale(fControlBtnScale);
	pDownBtn->setRotation(270);
	pDownBtn->addTouchEventListener(CC_CALLBACK_2(CGameSceneEx::__OnButtonEvent, this, BTNID_DOWN));
	pDownBtn->setCustomHighlightFlag(true);
	Size downBtnSize = GET_CONTENTSIZE(pDownBtn) * fControlBtnScale;

	// Left button
	Button* pLeftBtn = Button::create(strBtn0Name, strBtn1Name, "", Widget::TextureResType::PLIST);
	pLeftBtn->setScale(fControlBtnScale);
	pLeftBtn->addTouchEventListener(CC_CALLBACK_2(CGameSceneEx::__OnButtonEvent, this, BTNID_LEFT));
	pLeftBtn->setCustomHighlightFlag(true);
	Size leftBtnSize = GET_CONTENTSIZE(pLeftBtn) * fControlBtnScale;

	//Fire button
	Button* pFireBtn = Button::create(CGlobalFunc::GetSpriteNameWithMode("fire_0.png"), CGlobalFunc::GetSpriteNameWithMode("fire_1.png"), "", Widget::TextureResType::PLIST);
	pFireBtn->setRotation(90);
	pFireBtn->addTouchEventListener(CC_CALLBACK_2(CGameSceneEx::__OnButtonEvent, this, BTNID_FIRE));
	Size fireBtnSize = pFireBtn->getContentSize();

	// Set position
	float fPosX = upBtnSize.height * 1.2f;
	float fTopCenterY = fTopY + fControllerPadding + upBtnSize.height;
	pLeftBtn->setPosition(Vec2(fPosX, fTopCenterY + leftBtnSize.height * 0.5f - fBtnInnerPadding));
	pRightBtn->setPosition(Vec2(fPosX, fTopCenterY - rightBtnSize.height * 0.5f + fBtnInnerPadding));
	pDownBtn->setPosition(Vec2(fPosX - upBtnSize.height * 0.5f + fBtnInnerPadding, fTopCenterY));
	pUpBtn->setPosition(Vec2(fPosX + upBtnSize.height * 0.5f - fBtnInnerPadding, fTopCenterY));
	pFireBtn->setPosition(Vec2(fPosX, fBottomCenterY));

	// Center pos and area size
	m_oControllerCenterPos = Vec2(fPosX, fTopCenterY);
	m_oControllerCenterSize = Size(upBtnSize.width, upBtnSize.width);

	this->addChild(pLeftBtn);
	this->addChild(pRightBtn);
	this->addChild(pDownBtn);
	this->addChild(pUpBtn);
	this->addChild(pFireBtn);

	// Add all buttons to vector
	// Order: BTNID_RIGHT -> BTNID_DOWN -> BTNID_LEFT -> BTNID_UP -> BTNID_FIRE
	m_vecDirBtn.pushBack(pRightBtn);
	m_vecDirBtn.pushBack(pDownBtn);
	m_vecDirBtn.pushBack(pLeftBtn);
	m_vecDirBtn.pushBack(pUpBtn);
	m_vecDirBtn.pushBack(pFireBtn);
}


void CGameSceneEx::__InitBottomMenuEx(float& fBottomY)
{
	float fSpriteScale = 0.4f;
	Size visibleSize = GET_VISIBLESIZE();

	// Start button
	Sprite* pPlaySpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("play.png"));
	Sprite* pPauseSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("pause.png"));
	m_pStartBtn = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(CGameSceneEx::__OnClickButton, this, BTNID_START),
		MenuItemSprite::create(pPlaySpr, pPlaySpr, nullptr),
		MenuItemSprite::create(pPauseSpr, pPauseSpr, nullptr),
		nullptr
	);
	m_pStartBtn->setRotation(90);
	m_pStartBtn->setScale(fSpriteScale);
	Size startBtnSize = m_pStartBtn->getContentSize() * fSpriteScale;

	// Sound button
	Sprite* pSndOnSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("sound_on.png"));
	Sprite* pSndOffSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("sound_off.png"));
	auto pSoundOnMenu = MenuItemSprite::create(
		pSndOnSpr,
		pSndOnSpr,
		nullptr
	);
	auto pSoundOffMenu = MenuItemSprite::create(
		pSndOffSpr,
		pSndOffSpr,
		nullptr
	);
	m_pSoundBtn = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(CGameSceneEx::__OnClickButton, this, BTNID_SOUND), 
		pSoundOnMenu, pSoundOffMenu,
		nullptr
	);
	m_pSoundBtn->setRotation(90);
	m_pSoundBtn->setScale(fSpriteScale);
	m_pSoundBtn->setSelectedIndex(GET_BOOLVALUE("SOUND", true) ? 0 : 1);
	Size soundBtnSize = m_pSoundBtn->getContentSize() * fSpriteScale;

	// Reset button
	Sprite* pResetSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("reset.png"));
	m_pResetBtn = MenuItemSprite::create(
		pResetSpr,
		pResetSpr,
		CC_CALLBACK_1(CGameSceneEx::__OnClickButton, this, BTNID_RESET)
	);
	m_pStartBtn->setRotation(90);
	m_pResetBtn->setScale(fSpriteScale);
	Size resetBtnSize = m_pResetBtn->getContentSize() * fSpriteScale;

	// Setup button
	Sprite* pStarSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("setup.png"));
	m_pSetupBtn = MenuItemSprite::create(
		pStarSpr,
		pStarSpr,
		CC_CALLBACK_1(CGameSceneEx::__OnClickButton, this, BTNID_SETUP)
	);
	m_pStartBtn->setRotation(90);
	m_pSetupBtn->setScale(fSpriteScale);
	Size setupBtnSize = m_pSetupBtn->getContentSize() * fSpriteScale;

	// Bottom button horizontal padding
	fBottomY = soundBtnSize.height * 0.7f;
	float fBtnBottomInnerPadding = (visibleSize.width - resetBtnSize.height - setupBtnSize.height - startBtnSize.height - soundBtnSize.height) / 5;
	m_pStartBtn->setPosition(fBtnBottomInnerPadding + startBtnSize.height * 0.5f, fBottomY);
	m_pSoundBtn->setPosition(fBtnBottomInnerPadding * 2 + startBtnSize.height + soundBtnSize.height * 0.5f, fBottomY);
	m_pResetBtn->setPosition(fBtnBottomInnerPadding * 3 + startBtnSize.height + soundBtnSize.height + resetBtnSize.height * 0.5f, fBottomY);
	m_pSetupBtn->setPosition(visibleSize.width - fBtnBottomInnerPadding - setupBtnSize.height * 0.5f, fBottomY);
	// Menu
	auto menu = Menu::create(m_pStartBtn, m_pSoundBtn, m_pResetBtn, m_pSetupBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	fBottomY += soundBtnSize.height * 0.5f;
}


void CGameSceneEx::__InitRightUIEx(float fBottomCenterY, float fTopY)
{
	float fSpriteScale = 0.38f;
	float fSmallBrickScale = 0.7f;
	int nSmallBrickPadding = 2;
	int nNumSprPadding = 1;
	Size visibleSize = GET_VISIBLESIZE();
	float fRightX = visibleSize.width;
	float fTopCenterY = (visibleSize.height - fTopY) * 0.5f + fTopY;
	string strZeroSprName = CGlobalFunc::GetSpriteNameWithMode("0.png");

	////////////////////////// Top part //////////////////////////

	// Score label
	Sprite* pScoreSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("score.png"));
	Size scoreSize = GET_CONTENTSIZE(pScoreSpr) * fSpriteScale;
	pScoreSpr->setRotation(90);
	pScoreSpr->setScale(fSpriteScale);
	pScoreSpr->setPosition(fRightX - scoreSize.height, fTopCenterY);
	fRightX -= scoreSize.height * 1.5f;
	this->addChild(pScoreSpr);
	m_vecMiscSpr.pushBack(pScoreSpr);

	// Score num sprite
	Size numSize = Size::ZERO;
	float fTempY = 0;
	for (int nIndex = 0; nIndex < SCORE_NUM_COUNT; ++nIndex)
	{
		Sprite* pSpr = CREATE_SPRITEWITHNAME(strZeroSprName);
		if (numSize.equals(Size::ZERO))
		{
			numSize = GET_CONTENTSIZE(pSpr);
			fRightX -= numSize.height * 0.3f;
			fTempY = fTopCenterY + nNumSprPadding * 2.5f + numSize.width * 3;
		}
		pSpr->setPosition(fRightX - numSize.height * 0.5f, fTempY - numSize.width * 0.5f);
		pSpr->setRotation(90);
		this->addChild(pSpr);
		m_vecScoreSpr.pushBack(pSpr);

		fTempY -= numSize.width + nNumSprPadding;
	}
	fRightX -= numSize.height;

	// Max score label
	Sprite* pMaxScoreSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("hiscore.png"));
	Size maxScoreSize = GET_CONTENTSIZE(pMaxScoreSpr) * fSpriteScale;
	pMaxScoreSpr->setPosition(fRightX - maxScoreSize.height, fTopCenterY);
	pMaxScoreSpr->setRotation(90);
	pMaxScoreSpr->setScale(fSpriteScale);
	fRightX -= maxScoreSize.height * 1.5f;
	this->addChild(pMaxScoreSpr);
	m_vecMiscSpr.pushBack(pMaxScoreSpr);

	// Max score num sprite
	fRightX -= numSize.height * 0.3f;
	fTempY = fTopCenterY + nNumSprPadding * 2.5f + numSize.width * 3;
	for (int nIndex = 0; nIndex < SCORE_NUM_COUNT; ++nIndex)
	{
		Sprite* pSpr = CREATE_SPRITEWITHNAME(strZeroSprName);
		pSpr->setPosition(fRightX - numSize.height * 0.5f, fTempY - numSize.width * 0.5f);
		pSpr->setRotation(90);
		this->addChild(pSpr);
		m_vecMaxScoreSpr.pushBack(pSpr);

		fTempY -= numSize.width + nNumSprPadding;
	}
	fRightX -= numSize.height;

	////////////////////////// Bottom part //////////////////////////

	fRightX = visibleSize.width;
	// Small bricks
	Size smallBrickSize = Size::ZERO;
	for (int nRowIdx = 0; nRowIdx < SMALL_BRICK_ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < SMALL_BRICK_COLUMN_COUNT; ++nColIdx)
		{
			Sprite* pSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("empty.png"));
			if (smallBrickSize.equals(Size::ZERO))
			{
				smallBrickSize = GET_CONTENTSIZE(pSpr) * fSmallBrickScale;
				fRightX -= (nSmallBrickPadding + smallBrickSize.height * 0.7f);
			}
			if (nColIdx == 0)
			{
				fTempY = fBottomCenterY + nNumSprPadding * 1.5f + smallBrickSize.width * 2;
			}
			// Calc position
			pSpr->setRotation(90);
			pSpr->setScale(fSmallBrickScale);
			pSpr->setPosition(fRightX - smallBrickSize.height * 0.5f, fTempY - smallBrickSize.width * 0.5f);
			this->addChild(pSpr);
			m_vecSmallBrickSpr.pushBack(pSpr);

			fTempY -= (smallBrickSize.width + nSmallBrickPadding);
		}

		fRightX -= (smallBrickSize.height + nSmallBrickPadding);
	}

	// Speed label
	Sprite* pSpeedSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("speed.png"));
	Size speedSize = GET_CONTENTSIZE(pSpeedSpr) * fSpriteScale;
	fRightX -= speedSize.height * 1.6f;
	fTempY = fBottomCenterY + (speedSize.width + numSize.width * 2 + LEVEL_SPEED_NUM_PADDING * 2) * 0.5f;
	pSpeedSpr->setRotation(90);
	pSpeedSpr->setScale(fSpriteScale);
	pSpeedSpr->setPosition(fRightX + speedSize.height * 0.5f, fTempY - speedSize.width * 0.5f);
	this->addChild(pSpeedSpr);
	m_vecMiscSpr.pushBack(pSpeedSpr);

	// Speed num sprite
	fTempY -= speedSize.width;
	for (int nIndex = 0; nIndex < SPEED_NUM_COUNT; ++nIndex)
	{
		Sprite* pSpr = CREATE_SPRITEWITHNAME(strZeroSprName);
		pSpr->setRotation(90);
		pSpr->setPosition(fRightX + numSize.height * 0.5f, fTempY - numSize.width * 0.5f);
		this->addChild(pSpr);
		m_vecSpeedSpr.pushBack(pSpr);
		fTempY -= (numSize.width + LEVEL_SPEED_NUM_PADDING);
	}

	// Level label
	Sprite* pLevelSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("level.png"));
	Size levelSize = GET_CONTENTSIZE(pLevelSpr) * fSpriteScale;
	fTempY = fBottomCenterY + (levelSize.width + numSize.width * 2 + LEVEL_SPEED_NUM_PADDING * 2) * 0.5f;
	fRightX -= levelSize.height * 1.5f;
	pLevelSpr->setRotation(90);
	pLevelSpr->setScale(fSpriteScale);
	pLevelSpr->setPosition(fRightX + levelSize.height * 0.5f, fTempY - levelSize.width * 0.5f);
	this->addChild(pLevelSpr);
	m_vecMiscSpr.pushBack(pLevelSpr);

	// Level num sprite
	fTempY -= levelSize.width;
	for (int nIndex = 0; nIndex < SPEED_NUM_COUNT; ++nIndex)
	{
		Sprite* pSpr = CREATE_SPRITEWITHNAME(strZeroSprName);
		pSpr->setRotation(90);
		pSpr->setPosition(fRightX + numSize.height * 0.5f, fTempY - numSize.width * 0.5f);
		this->addChild(pSpr);
		m_vecLevelSpr.pushBack(pSpr);
		fTempY -= (numSize.width + LEVEL_SPEED_NUM_PADDING);
	}

	// Pause sprite
	fRightX -= levelSize.height * 0.5f;
	m_pPauseSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("tea.png"));
	Size pauseSize = GET_CONTENTSIZE(m_pPauseSpr) * fSpriteScale;
	m_pPauseSpr->setRotation(90);
	m_pPauseSpr->setScale(fSpriteScale);
	m_pPauseSpr->setPosition(fRightX - pauseSize.height * 0.5f, fBottomCenterY);
	this->addChild(m_pPauseSpr);
	m_pPauseSpr->setVisible(false);

	// Update level
	UpdateLevelOrSpeed(m_vecLevelSpr, m_stGameData.nLevel, m_stGameData.nLevel);
	// Update speed
	UpdateLevelOrSpeed(m_vecSpeedSpr, m_stGameData.nSpeed, m_stGameData.nLevel);
}


void CGameSceneEx::__ApplyRightHandModeEx()
{
	if (!GET_BOOLVALUE("RHMODE", false))
	{
		return;
	}

	Button* pRightBtn = m_vecDirBtn.at(BTNID_RIGHT);
	RETURN_IF_NULLPTR(pRightBtn);
	Button* pDownBtn = m_vecDirBtn.at(BTNID_DOWN);
	RETURN_IF_NULLPTR(pDownBtn);
	Button* pLeftBtn = m_vecDirBtn.at(BTNID_LEFT);
	RETURN_IF_NULLPTR(pLeftBtn);
	Button* pUpBtn = m_vecDirBtn.at(BTNID_UP);
	RETURN_IF_NULLPTR(pUpBtn);
	Button* pFireBtn = m_vecDirBtn.at(BTNID_FIRE);
	RETURN_IF_NULLPTR(pFireBtn);

	Size btnSize = GET_CONTENTSIZE(pUpBtn) * pUpBtn->getScale();
	Size visibleSize = GET_VISIBLESIZE();

	// Update controller position
	float fControllerCenterY = visibleSize.height - pUpBtn->getPositionY();
	pFireBtn->setPositionY(visibleSize.height - pFireBtn->getPositionY());
	pDownBtn->setPositionY(fControllerCenterY);
	pUpBtn->setPositionY(fControllerCenterY);
	pLeftBtn->setPositionY(fControllerCenterY + btnSize.height * 0.5f - CONTROLLER_INNER_PADDING);
	pRightBtn->setPositionY(fControllerCenterY - btnSize.height * 0.5f + CONTROLLER_INNER_PADDING);
	m_oControllerCenterPos.y = fControllerCenterY;

	// Update bottom button position
	m_pStartBtn->setPositionY(visibleSize.height - m_pStartBtn->getPositionY());
	m_pSoundBtn->setPositionY(visibleSize.height - m_pSoundBtn->getPositionY());
	m_pResetBtn->setPositionY(visibleSize.height - m_pResetBtn->getPositionY());
	m_pSetupBtn->setPositionY(visibleSize.height - m_pSetupBtn->getPositionY());
}


void CGameSceneEx::__InitTips()
{
	Size visibleSize = GET_VISIBLESIZE();
	m_pTipsLabel = Label::createWithSystemFont("123", FONT_NAME, TIPS_LABEL_SIZE);
	Color3B color = GET_BOOLVALUE("NIGHTMODE", false) ? Color3B::WHITE : Color3B::BLACK;
	m_pTipsLabel->setColor(color);
	m_pTipsLabel->setOpacity(0);
	m_pTipsLabel->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	m_pTipsLabel->setRotation(90);
	this->addChild(m_pTipsLabel);
}
