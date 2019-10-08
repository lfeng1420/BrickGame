#include "stdafx.h"
#include "GameScene.h"
#include "SetupScene.h"
#include "SetupSceneEx.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "base/CCGameController.h"
#endif


CGameScene::~CGameScene()
{
	g_oEventEngine.RemoveHandler(this);
}


cocos2d::Scene* CGameScene::CreateScene(const TGameSceneContext* pContext/* = nullptr*/)
{
	auto scene = Scene::create();
	auto layer = CGameScene::create(pContext);
	scene->addChild(layer);
	scene->setTag(GAME_SCENE_TAG);
	return scene;
}


cocos2d::LayerColor* CGameScene::create(const TGameSceneContext* pContext /*= nullptr*/)
{
	CGameScene* pLayer = new CGameScene();
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


bool CGameScene::init()
{
	bool bNightFlag = GET_BOOLVALUE("NIGHTMODE", false);
	if (!LayerColor::initWithColor(bNightFlag ? Color4B::BLACK : Color4B::WHITE))
	{
		return false;
	}

	// Subscribe event
	g_oEventEngine.AddHandler(EVENT_DATA_UPDATE, this);
	g_oEventEngine.AddHandler(EVENT_BRICKSTATE_UPDATE, this);
	g_oEventEngine.AddHandler(EVENT_GAME_OVER, this);
	g_oEventEngine.AddHandler(EVENT_PLAY_BGM, this);
	g_oEventEngine.AddHandler(EVENT_PLAY_EFFECT, this);
	g_oEventEngine.AddHandler(EVENT_VIBRATE, this);

	// Init ui
	InitUI();

	// Create key listener
	__CreateKeyListener();

	return true;
}


void CGameScene::OnEvent(int nEventID, const char* pContext, int nLen)
{
	switch (nEventID)
	{
	case EVENT_DATA_UPDATE:
		__OnGameDataUpdate(pContext, nLen);
		break;

	case EVENT_BRICKSTATE_UPDATE:
		__OnBrickStateUpdate(pContext, nLen);

	case EVENT_PLAY_BGM:
		__OnBGMUpdate(pContext, nLen);
		break;

	case EVENT_PLAY_EFFECT:
		__OnPlayEffect(pContext, nLen);
		break;

	case EVENT_GAME_OVER:
		__OnGameOver(pContext, nLen);
		break;

	case EVENT_VIBRATE:
		__OnVibrate(pContext, nLen);
		break;

	default:
		break;
	}
}


bool CGameScene::OnVote(int nEventID, char* pContext, int nLen)
{
	return true;
}


void CGameScene::update(float dt)
{
	if (m_pPauseSpr->isVisible())
	{
		return;
	}

	SEventContextFrameUpdate stContext = { dt * 1000 };
	g_oEventEngine.FireEvent(EVENT_FRAME_UPDATE, (const char*)&stContext, sizeof(stContext));
}


void CGameScene::ImportContextFirst(const TGameSceneContext* pContext)
{
	// Init game data
	m_stGameData.nCurScore = -1;
	m_stGameData.nMaxScore = -1;
	m_stGameData.nLevel = -1;
	m_stGameData.nSpeed = -1;

	// Bricks state
	if (pContext != nullptr)
	{
		memcpy(m_arrBrickState, pContext->arrBrickState, sizeof(m_arrBrickState));
		memcpy(m_arrSmallBrickState, pContext->arrSmallBrickState, sizeof(m_arrSmallBrickState));
	}
	else
	{
		INIT_BOOL_ARRAY(m_arrSmallBrickState);
		memcpy(m_arrBrickState, DEFAULT_SHAPE, sizeof(m_arrBrickState));
	}
}


void CGameScene::ImportContextLast(const TGameSceneContext* pContext)
{
	// Update pause sprite state
	bool bPauseFlag = ((pContext != nullptr) ? pContext->bPauseFlag : false);
	m_pPauseSpr->setVisible(bPauseFlag);

	// Init playing bgm flag
	m_bPlayingBGMFlag = false;

	// Update start button
	if (pContext != nullptr)
	{
		m_pStartBtn->setSelectedIndex(pContext->uiStartBtnIdx);
		m_bPlayingBGMFlag = pContext->bPlayingBGMFlag;
	}

	// Update data
	if (pContext != nullptr)
	{
		__OnGameDataUpdate((const char*)&pContext->stGameData, sizeof(pContext->stGameData));
	}
	else
	{
		SEventContextDataUpdate stDataInit = { 0, 0, 0, 0 };
		__OnGameDataUpdate((const char*)&stDataInit, sizeof(stDataInit));

		// Start over game
		SEventContextGameStart stContext = { GAMEID_OVER, 0, 0, 0, 0 };
		g_oEventEngine.FireEvent(EVENT_GAME_START, (const char*)&stContext, sizeof(stContext));
	}

	if (bPauseFlag)
	{
		return;
	}

	// Update per frame
	scheduleUpdate();

	// Play BGM
	SEventPlayBGM stBGM = { true, true };
	if (g_oEventEngine.FireVote(EVENT_PLAY_BGM, (char*)&stBGM, sizeof(stBGM)))
	{
		__PlayBGM(true, true);
	}
}


void CGameScene::InitUI()
{
	float fLeftX = 0;
	float fTopY = 0;
	__InitAllBricks(fLeftX, fTopY);
	float fBottomY = 0;
	__InitBottomMenu(fBottomY);
	__InitController(fTopY, fBottomY);
	__InitRightUI(fLeftX, fTopY);
	__ApplyRightHandMode();

	// Tips
	__InitTips();
}


void CGameScene::UpdateLevelOrSpeed(Vector<Sprite*> vecSpr, int& nOldVal, int nNewVal)
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
		float fPosX = pFirstSpr->getPositionX();
		Size sprSize = GET_CONTENTSIZE(pSecSpr);
		pSecSpr->setPositionX(fPosX + sprSize.width * 0.5f + LEVEL_SPEED_NUM_PADDING * 0.5f);
	}
	else
	{
		pFirstSpr->setSpriteFrame(CGlobalFunc::GetSpriteNameWithMode("1.png"));
		pFirstSpr->setVisible(true);
		pSecSpr->setSpriteFrame(CGlobalFunc::GetSpriteNameWithMode("0.png"));
		pSecSpr->setVisible(true);
		float fPosX = pFirstSpr->getPositionX();
		Size sprSize = GET_CONTENTSIZE(pSecSpr);
		pSecSpr->setPositionX(fPosX + sprSize.width + LEVEL_SPEED_NUM_PADDING);
	}

	// Update value
	nOldVal = nNewVal;
}


bool CGameScene::AdjustClickBtnID(const Vec2* pos, int& nBtnID)
{
	//ֻ�������
	if (nBtnID >= BTNID_DIRMAX)
	{
		return true;
	}

	Vec2 dis = *pos - m_oControllerCenterPos;
	float fFactor = dis.y / dis.x;
	if (fFactor >= 1 || fFactor <= -1)
	{
		nBtnID = dis.y > 1e-6 ? BTNID_UP : BTNID_DOWN;
		return true;
	}

	if (fFactor < 1 && fFactor >= -1)
	{
		nBtnID = dis.x > 1e-6 ? BTNID_RIGHT : BTNID_LEFT;
		return true;
	}

	nBtnID = -1;
	return false;
}


void CGameScene::__OnGameOver(const char* pContext, int nLen)
{
	if (m_pPauseSpr->isVisible())
	{
		m_pStartBtn->activate();
	}

	// Recover start button state
	m_pStartBtn->setSelectedIndex(0);
}


void CGameScene::__OnGameDataUpdate(const char* pContext, int nLen)
{
	if (nLen != sizeof(SEventContextDataUpdate))
	{
		return;
	}

	const SEventContextDataUpdate* pDataUpdate = (const SEventContextDataUpdate*)pContext;
	// Update score
	__UpdateScore(m_vecScoreSpr, m_stGameData.nCurScore, pDataUpdate->nCurScore);
	
	// Update max score
	__UpdateScore(m_vecMaxScoreSpr, m_stGameData.nMaxScore, pDataUpdate->nMaxScore);

	// Update level
	UpdateLevelOrSpeed(m_vecLevelSpr, m_stGameData.nLevel, pDataUpdate->nLevel);
	
	// Update speed
	UpdateLevelOrSpeed(m_vecSpeedSpr, m_stGameData.nSpeed, pDataUpdate->nSpeed);
}


void CGameScene::__OnBrickStateUpdate(const char* pContext, int nLen)
{
	if (nLen != sizeof(SEventContextBrickStateUpdate))
	{
		return;
	}

	const SEventContextBrickStateUpdate* pStateUpdate = (const SEventContextBrickStateUpdate*)pContext;
	// Update bricks
	FOREACH_IN_CONST_CONTAINER (TMap_BrickState, pStateUpdate->mapUpdateBricks, itBricks)
	{
		int nBrickID = itBricks->first;
		bool bStateFlag = itBricks->second;
		bool& bSrcStateFlag = ((pStateUpdate->bSmallBrickFlag) ? m_arrSmallBrickState[nBrickID] : m_arrBrickState[nBrickID]);
		if (bSrcStateFlag != bStateFlag)
		{
			Sprite* pSpr = ((pStateUpdate->bSmallBrickFlag) ? m_vecSmallBrickSpr.at(nBrickID) : m_vecBrickSpr.at(nBrickID));
			if (pSpr != nullptr)
			{
				bSrcStateFlag = bStateFlag;
				const char* szDestFrameName = CGlobalFunc::GetSpriteNameWithMode(bStateFlag ? "black.png" : "empty.png");
				pSpr->setSpriteFrame(szDestFrameName);
			}
		}
	}
}


void CGameScene::__OnBGMUpdate(const char* pContext, int nLen)
{
	if (nLen != sizeof(SEventPlayBGM))
	{
		return;
	}

	const SEventPlayBGM* pBGMusic = (const SEventPlayBGM*)pContext;
	__PlayBGM(pBGMusic->bPlayFlag, pBGMusic->bLoopFlag);
}


void CGameScene::__OnPlayEffect(const char* pContext, int nLen)
{
	if (nLen != sizeof(SEventPlayEffect))
	{
		return;
	}

	const SEventPlayEffect* pPlayEffect = (const SEventPlayEffect*)pContext;
	__PlayEffect(pPlayEffect->szSound);
}


void CGameScene::__OnVibrate(const char* pContext, int nLen)
{
	if (nLen != sizeof(SEventVibrate))
	{
		return;
	}

	const SEventVibrate* pEvent = (const SEventVibrate*)pContext;
	log("Vibrate bShortFlag=%d", pEvent->bShortFlag);
	if (pEvent->bShortFlag)
	{
		CGlobalFunc::ShortVibrate();
	}
	else
	{
		CGlobalFunc::LongVibrate();
	}
}


void CGameScene::__InitAllBricks(float& fLeftX, float& fTopY)
{
	Size visibleSize = GET_VISIBLESIZE();
	Size brickSize = Size::ZERO;
	fTopY = visibleSize.height;
	fLeftX = 0;

	for (int nRowIdx = 0; nRowIdx < ROW_COUNT; ++nRowIdx)
	{
		float fCurX = GET_INTVALUE("BRICKS_OFFSET", 0);
		for (int nColIdx = 0; nColIdx < COLUMN_COUNT; ++nColIdx)
		{
			int nBrickID = GET_BRICKID(nRowIdx, nColIdx);

			// Create sprite
			Sprite* pSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode(m_arrBrickState[nBrickID] ? "black.png" : "empty.png"));
			if (brickSize.equals(Size::ZERO))
			{
				brickSize = GET_CONTENTSIZE(pSpr);
			}
			pSpr->setPosition(fCurX + brickSize.width * 0.5f, fTopY - brickSize.height * 0.5f);
			pSpr->setTag(nBrickID);
			this->addChild(pSpr);
			m_vecBrickSpr.pushBack(pSpr);

			fCurX += brickSize.width;
			fLeftX = max(fLeftX, fCurX);
		}

		fTopY -= brickSize.height;
	}
}


void CGameScene::__InitController(float fTopY, float fBottomY)
{
	float fBtnInnerPadding = CONTROLLER_INNER_PADDING;
	float fControllerPadding = 10;
	const float CONTROLLER_SCALE_MAX = GET_INTVALUE("DIRBTN_SCALE", DIRBTN_DEFAULT_SCALE) / 100.0f;

	// Up button
	string strBtn0Name = CGlobalFunc::GetSpriteNameWithMode("btn_0.png");
	string strBtn1Name = CGlobalFunc::GetSpriteNameWithMode("btn_1.png");
    MenuItemSprite* pUpBtn = MenuItemSprite::create(
        CREATE_SPRITEWITHNAME(strBtn0Name), 
        CREATE_SPRITEWITHNAME(strBtn1Name),
        nullptr);
	Size btnSize = GET_CONTENTSIZE(pUpBtn);
	float fControlBtnScale = (fTopY - fBottomY - fControllerPadding * 2) / (2 * btnSize.height);
	if (fControlBtnScale > CONTROLLER_SCALE_MAX)
	{
		fControlBtnScale = CONTROLLER_SCALE_MAX;
		fControllerPadding = (fTopY - fBottomY - CONTROLLER_SCALE_MAX * btnSize.height * 2) * 0.5f;
	}
	pUpBtn->setScale(fControlBtnScale);
	Size upBtnSize = btnSize * fControlBtnScale;

	// Right button
    MenuItemSprite* pRightBtn = MenuItemSprite::create(
        CREATE_SPRITEWITHNAME(strBtn0Name),
        CREATE_SPRITEWITHNAME(strBtn1Name),
        nullptr);
	pRightBtn->setScale(fControlBtnScale);
	pRightBtn->setRotation(90);
	Size rightBtnSize = GET_CONTENTSIZE(pRightBtn) * fControlBtnScale;

	// Down button
    MenuItemSprite* pDownBtn = MenuItemSprite::create(
        CREATE_SPRITEWITHNAME(strBtn0Name),
        CREATE_SPRITEWITHNAME(strBtn1Name),
        nullptr);
	pDownBtn->setScale(fControlBtnScale);
	pDownBtn->setRotation(180);
	Size downBtnSize = GET_CONTENTSIZE(pDownBtn) * fControlBtnScale;

	// Left button
    MenuItemSprite* pLeftBtn = MenuItemSprite::create(
        CREATE_SPRITEWITHNAME(strBtn0Name),
        CREATE_SPRITEWITHNAME(strBtn1Name),
        nullptr);
	pLeftBtn->setScale(fControlBtnScale);
	pLeftBtn->setRotation(-90);
	Size leftBtnSize = GET_CONTENTSIZE(pLeftBtn) * fControlBtnScale;

	//Fire button
    string strFire0Name = CGlobalFunc::GetSpriteNameWithMode("fire_0.png");
    string strFire1Name = CGlobalFunc::GetSpriteNameWithMode("fire_1.png");
    MenuItemSprite* pFireBtn = MenuItemSprite::create(
        CREATE_SPRITEWITHNAME(strFire0Name),
        CREATE_SPRITEWITHNAME(strFire1Name),
        nullptr);
	Size fireBtnSize = pFireBtn->getContentSize();

    Menu* pMenu = Menu::create(pUpBtn, pRightBtn, pDownBtn, pLeftBtn, nullptr);
    pMenu->setTouchCallback(CC_CALLBACK_3(CGameScene::__OnMenuTouch, this));
    pMenu->setPosition(Vec2::ZERO);
    this->addChild(pMenu);

	Menu* pFireMenu = Menu::create(pFireBtn, nullptr);
	pFireMenu->setTouchCallback(CC_CALLBACK_3(CGameScene::__OnMenuTouch, this));
	pFireMenu->setPosition(Vec2::ZERO);
	this->addChild(pFireMenu);

	// Set position
	Size visibleSize = GET_VISIBLESIZE();
    float fCenterX = upBtnSize.height * 1.1f;
    float fCenterY = fTopY - fControllerPadding - upBtnSize.height + GET_INTVALUE("DIRBTN_YOFFSET", 0);
	pLeftBtn->setPosition(Vec2(fCenterX - upBtnSize.height * 0.5f + fBtnInnerPadding, fCenterY));
	pRightBtn->setPosition(Vec2(fCenterX + upBtnSize.height * 0.5f - fBtnInnerPadding, fCenterY));
	pDownBtn->setPosition(Vec2(fCenterX, fCenterY - upBtnSize.height * 0.5f + fBtnInnerPadding));
	pUpBtn->setPosition(Vec2(fCenterX, fCenterY + upBtnSize.height * 0.5f - fBtnInnerPadding));
	pFireBtn->setPosition(Vec2(visibleSize.width - fireBtnSize.width * 0.8f, fCenterY));

	// Center pos and area size
	m_oControllerCenterPos = Vec2(fCenterX, fCenterY);
	m_oControllerCenterSize = Size(upBtnSize.width, upBtnSize.width);

	// Add all buttons to vector
	// Order: BTNID_RIGHT -> BTNID_DOWN -> BTNID_LEFT -> BTNID_UP -> BTNID_FIRE
	m_vecDirBtn.pushBack(pRightBtn);
	m_vecDirBtn.pushBack(pDownBtn);
	m_vecDirBtn.pushBack(pLeftBtn);
	m_vecDirBtn.pushBack(pUpBtn);
	m_vecDirBtn.pushBack(pFireBtn);
}


void CGameScene::__InitBottomMenu(float& fBottomY)
{
	float fSpriteScale = 0.4f;
	Size visibleSize = GET_VISIBLESIZE();

	// Start button
	Sprite* pPlaySpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("play.png"));
	Sprite* pPauseSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("pause.png"));
	m_pStartBtn = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(CGameScene::__OnClickButton, this, BTNID_START),
		MenuItemSprite::create(pPlaySpr, pPlaySpr, nullptr),
		MenuItemSprite::create(pPauseSpr, pPauseSpr, nullptr),
		nullptr
	);
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
		CC_CALLBACK_1(CGameScene::__OnClickButton, this, BTNID_SOUND),
		pSoundOnMenu , pSoundOffMenu,
		nullptr
	);
	m_pSoundBtn->setScale(fSpriteScale);
	m_pSoundBtn->setSelectedIndex(GET_BOOLVALUE("SOUND", true) ? 0 : 1);
	Size soundBtnSize = m_pSoundBtn->getContentSize() * fSpriteScale;
	// Bottom y
	fBottomY = soundBtnSize.height * 1.3f;

	// Reset button
	Sprite* pResetSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("reset.png"));
	m_pResetBtn = MenuItemSprite::create(
		pResetSpr,
		pResetSpr,
		CC_CALLBACK_1(CGameScene::__OnClickButton, this, BTNID_RESET)
	);
	m_pResetBtn->setScale(fSpriteScale);
	Size resetBtnSize = m_pResetBtn->getContentSize() * fSpriteScale;

	// Setup button
	Sprite* pStarSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("setup.png"));
	m_pSetupBtn = MenuItemSprite::create(
		pStarSpr,
		pStarSpr,
		CC_CALLBACK_1(CGameScene::__OnClickButton, this, BTNID_SETUP)
	);
	m_pSetupBtn->setScale(fSpriteScale);
	Size setupBtnSize = m_pSetupBtn->getContentSize() * fSpriteScale;

	// Bottom button horizontal padding
	float fHorizonalPadding = (visibleSize.width - startBtnSize.width - soundBtnSize.width - resetBtnSize.width - setupBtnSize.width) / 5;
	float fSmallBtnTopHeight = soundBtnSize.height * 1.3f;
	float fSmallBtnCenterHeight = fSmallBtnTopHeight - soundBtnSize.height / 3.1f;
	m_pStartBtn->setPosition(fHorizonalPadding + startBtnSize.width  * 0.5f, fSmallBtnCenterHeight);
	m_pSoundBtn->setPosition(fHorizonalPadding * 2 + startBtnSize.width + soundBtnSize.width  * 0.5f, fSmallBtnCenterHeight);
	m_pResetBtn->setPosition(fHorizonalPadding * 3 + startBtnSize.width + soundBtnSize.width + resetBtnSize.width * 0.5f, fSmallBtnCenterHeight);
	m_pSetupBtn->setPosition(visibleSize.width - fHorizonalPadding - setupBtnSize.width * 0.5f, fSmallBtnCenterHeight);
	// Menu
	auto menu = Menu::create(m_pStartBtn, m_pSoundBtn, m_pResetBtn, m_pSetupBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}


void CGameScene::__InitRightUI(float fLeftX, float fBottomY)
{
	float fSpriteScale = 0.38f;
	float fSmallBrickScale = 0.7f;
	int nSmallBrickPadding = 2;
	int nNumSprPadding = 1;
	Size visibleSize = GET_VISIBLESIZE();
	float fCenterX = fLeftX + (visibleSize.width - fLeftX) * 0.5f;
	float fTopY = visibleSize.height;
	string strZeroSprName = CGlobalFunc::GetSpriteNameWithMode("0.png");
	
	// Score label
	Sprite* pScoreSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("score.png"));
	Size scoreSize = GET_CONTENTSIZE(pScoreSpr) * fSpriteScale;
	pScoreSpr->setScale(fSpriteScale);
	pScoreSpr->setPosition(fCenterX, fTopY - scoreSize.height);
	fTopY -= scoreSize.height * 1.5f;
	this->addChild(pScoreSpr);
	m_vecMiscSpr.pushBack(pScoreSpr);

	// Score num sprite
	Size numSize = Size::ZERO;
	float fTempX = fLeftX;
	for (int nIndex = 0; nIndex < SCORE_NUM_COUNT; ++nIndex)
	{
		Sprite* pSpr = CREATE_SPRITEWITHNAME(strZeroSprName);
		if (numSize.equals(Size::ZERO))
		{
			numSize = GET_CONTENTSIZE(pSpr);
			fTopY -= numSize.height * 0.3f;
			fTempX = fCenterX - nNumSprPadding * 2.5f - numSize.width * 3;
		}
		pSpr->setPosition(fTempX + numSize.width * 0.5f, fTopY - numSize.height * 0.5f);
		this->addChild(pSpr);
		m_vecScoreSpr.pushBack(pSpr);

		fTempX += numSize.width + nNumSprPadding;
	}
	fTopY -= numSize.height;

	// Max score label
	Sprite* pMaxScoreSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("hiscore.png"));
	Size maxScoreSize = GET_CONTENTSIZE(pMaxScoreSpr) * fSpriteScale;
	pMaxScoreSpr->setPosition(fCenterX, fTopY - maxScoreSize.height);
	pMaxScoreSpr->setScale(fSpriteScale);
	fTopY -= maxScoreSize.height * 1.5f;
	this->addChild(pMaxScoreSpr);
	m_vecMiscSpr.pushBack(pMaxScoreSpr);

	// Max score num sprite
	fTopY -= numSize.height * 0.3f;
	fTempX = fCenterX - nNumSprPadding * 2.5f - numSize.width * 3;
	for (int nIndex = 0; nIndex < SCORE_NUM_COUNT; ++nIndex)
	{
		Sprite* pSpr = CREATE_SPRITEWITHNAME(strZeroSprName);
		pSpr->setPosition(fTempX + numSize.width * 0.5f, fTopY - numSize.height * 0.5f);
		this->addChild(pSpr);
		m_vecMaxScoreSpr.pushBack(pSpr);

		fTempX += numSize.width + nNumSprPadding;
	}
	fTopY -= numSize.height + numSize.height * 0.5f;

	// Small bricks
	Size smallBrickSize = Size::ZERO;
	for (int nRowIdx = 0; nRowIdx < SMALL_BRICK_ROW_COUNT; ++nRowIdx)
	{
		for (int nColIdx = 0; nColIdx < SMALL_BRICK_COLUMN_COUNT; ++nColIdx)
		{
			int nSmallBrickID = GET_SMALLBRICKID(nRowIdx, nColIdx);
			const char* szSprName = m_arrSmallBrickState[nSmallBrickID] ? "black.png" : "empty.png";
			Sprite* pSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode(szSprName));
			if (smallBrickSize.equals(Size::ZERO))
			{
				smallBrickSize = GET_CONTENTSIZE(pSpr) * fSmallBrickScale;
				fTopY -= (nSmallBrickPadding + smallBrickSize.height * 0.7f);
			}
			if (nColIdx == 0)
			{
				fTempX = fCenterX - nNumSprPadding * 1.5f - smallBrickSize.width * 2;
			}
			// Calc position
			pSpr->setScale(fSmallBrickScale);
			pSpr->setPosition(fTempX + smallBrickSize.width * 0.5f, fTopY - smallBrickSize.height * 0.5f);
			this->addChild(pSpr);
			m_vecSmallBrickSpr.pushBack(pSpr);

			fTempX += smallBrickSize.width + nSmallBrickPadding;
		}

		fTopY -= (smallBrickSize.height + nSmallBrickPadding);
	}

	// Speed label
	fTopY -= numSize.height * 0.5f;
	Sprite* pSpeedSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("speed.png"));
	Size speedSize = GET_CONTENTSIZE(pSpeedSpr) * fSpriteScale;
	fTopY -= speedSize.height * 1.5f;
	pSpeedSpr->setScale(fSpriteScale);
	pSpeedSpr->setPosition(fCenterX, fTopY + speedSize.height * 0.5f);
	this->addChild(pSpeedSpr);
	m_vecMiscSpr.pushBack(pSpeedSpr);

	// Speed num sprite
	fTopY -= numSize.height * 0.8f;
	fTempX = fCenterX - numSize.width - LEVEL_SPEED_NUM_PADDING * 0.5f;
	for (int nIndex = 0; nIndex < SPEED_NUM_COUNT; ++nIndex)
	{
		Sprite* pSpr = CREATE_SPRITEWITHNAME(strZeroSprName);
		pSpr->setPosition(fTempX + numSize.width * 0.5f, fTopY - numSize.height * 0.5f);
		this->addChild(pSpr);
		m_vecSpeedSpr.pushBack(pSpr);
		fTempX += numSize.width + LEVEL_SPEED_NUM_PADDING;
	}
	fTopY -= numSize.height * 1.8f;

	// Level num sprite
	fTempX = fCenterX - numSize.width - LEVEL_SPEED_NUM_PADDING * 0.5f;
	for (int nIndex = 0; nIndex < SPEED_NUM_COUNT; ++nIndex)
	{
		Sprite* pSpr = CREATE_SPRITEWITHNAME(strZeroSprName);
		pSpr->setPosition(fTempX + numSize.width * 0.5f, fTopY - numSize.height * 0.5f);
		this->addChild(pSpr);
		m_vecLevelSpr.pushBack(pSpr);
		fTempX += numSize.width + LEVEL_SPEED_NUM_PADDING;
	}
	fTopY -= numSize.height * 1.3f;

	// Level label
	Sprite* pLevelSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("level.png"));
	Size levelSize = GET_CONTENTSIZE(pLevelSpr) * fSpriteScale;
	fTopY -= levelSize.height * 1.5f;
	pLevelSpr->setScale(fSpriteScale);
	pLevelSpr->setPosition(fCenterX, fTopY + levelSize.height * 0.5f);
	this->addChild(pLevelSpr);
	m_vecMiscSpr.pushBack(pLevelSpr);

	// Pause sprite
	m_pPauseSpr = CREATE_SPRITEWITHNAME(CGlobalFunc::GetSpriteNameWithMode("tea.png"));
	Size pauseSize = GET_CONTENTSIZE(m_pPauseSpr) * fSpriteScale;
	m_pPauseSpr->setScale(fSpriteScale);
	m_pPauseSpr->setPosition(fCenterX, fBottomY + pauseSize.height * 0.5f);
	this->addChild(m_pPauseSpr);
}


void CGameScene::__InitTips()
{
	Size visibleSize = GET_VISIBLESIZE();
	m_pTipsLabel = Label::createWithSystemFont("123", FONT_NAME, TIPS_LABEL_SIZE);
	Color3B color = GET_BOOLVALUE("NIGHTMODE", false) ? Color3B::WHITE : Color3B::BLACK;
	m_pTipsLabel->setColor(color);
	m_pTipsLabel->setOpacity(0);
	m_pTipsLabel->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	this->addChild(m_pTipsLabel);
}


void CGameScene::__ApplyRightHandMode()
{
	if (!GET_BOOLVALUE("RHMODE", false))
	{
		return;
	}

	MenuItem* pRightBtn = m_vecDirBtn.at(BTNID_RIGHT);
	RETURN_IF_NULLPTR(pRightBtn);
    MenuItem* pDownBtn = m_vecDirBtn.at(BTNID_DOWN);
	RETURN_IF_NULLPTR(pDownBtn);
    MenuItem* pLeftBtn = m_vecDirBtn.at(BTNID_LEFT);
	RETURN_IF_NULLPTR(pLeftBtn);
    MenuItem* pUpBtn = m_vecDirBtn.at(BTNID_UP);
	RETURN_IF_NULLPTR(pUpBtn);
    MenuItem* pFireBtn = m_vecDirBtn.at(BTNID_FIRE);
	RETURN_IF_NULLPTR(pFireBtn);

	Size btnSize = GET_CONTENTSIZE(pUpBtn);
	Size visibleSize = GET_VISIBLESIZE();

	// Update position
	float fExternPadding = 10.0f;
	float fControllerCenterX = (visibleSize.width - pUpBtn->getPositionX() - fExternPadding);
	pFireBtn->setPositionX(visibleSize.width - pFireBtn->getPositionX() - fExternPadding);
	pDownBtn->setPositionX(fControllerCenterX);
	pUpBtn->setPositionX(fControllerCenterX);
	pLeftBtn->setPositionX(fControllerCenterX - btnSize.height * 0.5f + CONTROLLER_INNER_PADDING);
	pRightBtn->setPositionX(fControllerCenterX + btnSize.height * 0.5f - CONTROLLER_INNER_PADDING);
	m_oControllerCenterPos.x = fControllerCenterX;
}


void CGameScene::__CreateKeyListener()
{
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (EventKeyboard::KeyCode::KEY_A == keyCode)
		{
			//log("A Pressed");
			__OnButtonPressed(BTNID_LEFT);
		}
		else if (EventKeyboard::KeyCode::KEY_D == keyCode)
		{
			//log("D Pressed");
			__OnButtonPressed(BTNID_RIGHT);
		}
		else if (EventKeyboard::KeyCode::KEY_S == keyCode)
		{
			//log("S Pressed");
			__OnButtonPressed(BTNID_DOWN);
		}
		else if (EventKeyboard::KeyCode::KEY_W == keyCode)
		{
			//log("W Pressed");
			__OnButtonPressed(BTNID_UP);
		}
		else if (EventKeyboard::KeyCode::KEY_K == keyCode)
		{
			//log("K Pressed");
			__OnButtonPressed(BTNID_FIRE);
		}
		else if (EventKeyboard::KeyCode::KEY_SPACE == keyCode)
		{
			m_pStartBtn->activate();
		}
	};

	keyListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (EventKeyboard::KeyCode::KEY_A == keyCode)
		{
			//log("A Released");
			__OnButtonReleased(BTNID_LEFT);
		}
		else if (EventKeyboard::KeyCode::KEY_D == keyCode)
		{
			//log("D Released");
			__OnButtonReleased(BTNID_RIGHT);
		}
		else if (EventKeyboard::KeyCode::KEY_S == keyCode)
		{
			//log("S Released");
			__OnButtonReleased(BTNID_DOWN);
		}
		else if (EventKeyboard::KeyCode::KEY_W == keyCode)
		{
			//log("W Released");
			__OnButtonReleased(BTNID_UP);
		}
		else if (EventKeyboard::KeyCode::KEY_K == keyCode)
		{
			//log("K Released");
			__OnButtonReleased(BTNID_FIRE);
		}
		else if (EventKeyboard::KeyCode::KEY_RETURN == keyCode ||
			EventKeyboard::KeyCode::KEY_ESCAPE == keyCode ||
			EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
		{
			if (m_pTipsLabel->getOpacity() != 0)
			{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8
				GLView::sharedOpenGLView()->QuitGame();
#else
				Director::getInstance()->end();
#endif
				return;
			}

			m_pTipsLabel->setString(CGlobalFunc::GetString(STRID_QUIT));
			m_pTipsLabel->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(1.0f), FadeOut::create(0.5f), nullptr));
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);


	// �ֱ�����
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_enLeftBtnID = BTNID_MAX;
	m_enRightBtnID = BTNID_MAX;
	auto eventController = EventListenerController::create();
	eventController->onKeyUp = [&](Controller* pController, int nKey, Event* pEvent)
	{
		switch (nKey)
		{
		case Controller::BUTTON_DPAD_UP:
			__OnButtonReleased(BTNID_UP);
			break;

		case Controller::BUTTON_DPAD_DOWN:
			__OnButtonReleased(BTNID_DOWN);
			break;

		case Controller::BUTTON_DPAD_RIGHT:
			__OnButtonReleased(BTNID_RIGHT);
			break;

		case Controller::BUTTON_DPAD_LEFT:
			__OnButtonReleased(BTNID_LEFT);
			break;

		case Controller::BUTTON_A:
		case Controller::BUTTON_B:
		case Controller::BUTTON_X:
		case Controller::BUTTON_Y:
			__OnButtonReleased(BTNID_FIRE);
			break;

		default:
			break;
		}
	};

	eventController->onKeyDown = [&](Controller* pController, int nKey, Event* pEvent)
	{
		switch (nKey)
		{
		case Controller::BUTTON_DPAD_UP:
			__OnButtonPressed(BTNID_UP);
			break;

		case Controller::BUTTON_DPAD_DOWN:
			__OnButtonPressed(BTNID_DOWN);
			break;

		case Controller::BUTTON_DPAD_RIGHT:
			__OnButtonPressed(BTNID_RIGHT);
			break;

		case Controller::BUTTON_DPAD_LEFT:
			__OnButtonPressed(BTNID_LEFT);
			break;

		case Controller::BUTTON_START:
		case Controller::AXIS_RIGHT_TRIGGER:
			m_pStartBtn->activate();
			break;

		case Controller::BUTTON_SELECT:
		case Controller::AXIS_LEFT_TRIGGER:
			__OnClickResetBtn();
			break;

		case Controller::BUTTON_A:
			__OnButtonPressed(BTNID_FIRE);
			break;

		case Controller::BUTTON_B:
			__OnClickSoundBtn();
			break;

		case Controller::BUTTON_X:
			__OnClickResetBtn();
			break;

		case Controller::BUTTON_Y:
			__OnClickSetupBtn();
			break;

		default:
			break;
		}
	};

	eventController->onAxisEvent = [&](Controller* pController, int nKey, Event* pEvent)
	{
		EnButtonID enBtnID = BTNID_MAX;
		bool bLeftFlag = (nKey == Controller::JOYSTICK_LEFT_X || Controller::JOYSTICK_LEFT_Y == nKey);
		const Controller::KeyStatus& stKeyStatus = pController->getKeyStatus(nKey);
		if (nKey == Controller::JOYSTICK_LEFT_X
			|| nKey == Controller::JOYSTICK_RIGHT_X)
		{
			if (stKeyStatus.value < -0.5f)
			{
				enBtnID = BTNID_LEFT;
			}
			else if (stKeyStatus.value > 0.5f)
			{
				enBtnID = BTNID_RIGHT;
			}
			else
			{
				enBtnID = BTNID_MAX;
			}
		}
		else if (nKey == Controller::JOYSTICK_LEFT_Y
			|| nKey == Controller::JOYSTICK_RIGHT_Y)
		{
			if (stKeyStatus.value < -0.5f)
			{
				enBtnID = BTNID_UP;
			}
			else if (stKeyStatus.value > 0.5f)
			{
				enBtnID = BTNID_DOWN;
			}
			else
			{
				enBtnID = BTNID_MAX;
			}
		}

		EnButtonID& enCurBtnID = (bLeftFlag ? m_enLeftBtnID : m_enRightBtnID);
		if (enCurBtnID != enBtnID)
		{
			if (enCurBtnID != BTNID_MAX)
			{
				__OnButtonReleased(enCurBtnID);
			}

			enCurBtnID = enBtnID;
			__OnButtonPressed(enCurBtnID);
		}
	};

	eventController->onConnected = [&](Controller* pController, Event* pEvent)
	{
		log("onConnected");
	};
	eventController->onDisconnected = [&](Controller* pController, Event* pEvent)
	{
		log("onDisconnected");
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventController, this);
	Controller::startDiscoveryController();
#endif
}


void CGameScene::__OnClickButton(Ref* pSender, int nBtnID)
{
	// Vibrate
	CGlobalFunc::ShortVibrate();

	switch (nBtnID)
	{
	case BTNID_START:
		__OnClickStartBtn();
		break;

	case BTNID_SOUND:
		__OnClickSoundBtn();
		break;

	case BTNID_RESET:
		__OnClickResetBtn();
		break;

	case BTNID_SETUP:
		__OnClickSetupBtn();
		break;

	default:
		break;
	}
}

void CGameScene::__OnMenuTouch(Ref* pSender, MenuItem** pSelectedMenuItem, Menu::TouchState enTouchState)
{
    if (m_pPauseSpr != nullptr && m_pPauseSpr->isVisible())
    {
        return;
    }

    const Vec2* pPos = nullptr;
    Menu* pMenu = (Menu*)pSender;
    int nBtnID = m_vecDirBtn.getIndex(*pSelectedMenuItem);
    if (nBtnID == -1)
    {
        return;
    }

    switch (enTouchState)
    {
    case Menu::TouchState::BEGIN:
    {
        //log("-----------------\n[BEGAN]  nBtnID=%d", nBtnID);
        pPos = pMenu->getTouchBeganPos();
        if (!AdjustClickBtnID(pPos, nBtnID))
        {
            return;
        }

        // Record relation
        //log("m_mapBtnRelation[%d]=%d", nOldBtnID, nBtnID);
        __OnButtonPressed(nBtnID);
        *pSelectedMenuItem = m_vecDirBtn.at(nBtnID);
    }
    break;

    case Menu::TouchState::MOVE:
        //log("-----------------\n[MOVED]  nBtnID=%d", nBtnID);
        break;

    case Menu::TouchState::END:
    case Menu::TouchState::CANCEL:
    {
        __OnButtonReleased(nBtnID);
    }
    break;
    }
}

void CGameScene::__ExportContext(TGameSceneContext* pContext)
{
	memcpy(pContext->arrBrickState, m_arrBrickState, sizeof(m_arrBrickState));
	memcpy(pContext->arrSmallBrickState, m_arrSmallBrickState, sizeof(m_arrSmallBrickState));
	pContext->stGameData = m_stGameData;
	pContext->bPauseFlag = m_pPauseSpr->isVisible();
	pContext->uiStartBtnIdx = m_pStartBtn->getSelectedIndex();
	pContext->bPlayingBGMFlag = m_bPlayingBGMFlag;
}


void CGameScene::__UpdateScore(Vector<Sprite*>& vecSpr, int& nOldScore, int nNewScore)
{
	if (nOldScore == nNewScore)
	{
		return;
	}

	// Old score string
	char szOldScore[64] = { 0 };
	sprintf(szOldScore, "%06d", nOldScore);

	// New score string
	char szScore[64] = { 0 };
	sprintf(szScore, "%06d", nNewScore);

	// Check difference, then update sprite
	char szSpriteName[64] = { 0 };
	for (int nIndex = 0; szScore[nIndex] != 0 && szOldScore[nIndex] != 0; ++nIndex)
	{
		if (szOldScore[nIndex] == szScore[nIndex])
		{
			continue;
		}

		sprintf(szSpriteName, "%c.png", szScore[nIndex]);
		Sprite* pSpr = vecSpr.at(nIndex);
		if (pSpr != nullptr)
		{
			pSpr->setSpriteFrame(CGlobalFunc::GetSpriteNameWithMode(szSpriteName));
		}
	}

	// Update score
	nOldScore = nNewScore;
}


void CGameScene::__OnButtonPressed(int nButtonID)
{
	// Play effect
	__PlayEffect(EFFECT_CHANGE);

	// Vibrate
	CGlobalFunc::ShortVibrate();

	// Fire event except pause state
	if (!m_pPauseSpr->isVisible())
	{
        SEventContextButton stButtonEvent = { nButtonID, true };
        g_oEventEngine.FireEvent(EVENT_BUTTON, (const char*)&stButtonEvent, sizeof(stButtonEvent));
	}
}


void CGameScene::__OnButtonReleased(int nButtonID)
{
	if (!m_pPauseSpr->isVisible())
	{
        SEventContextButton stButtonEvent = { nButtonID, false };
        g_oEventEngine.FireEvent(EVENT_BUTTON, (const char*)&stButtonEvent, sizeof(stButtonEvent));
	}
}


void CGameScene::__OnClickStartBtn()
{
	// If pause, start update per frame
	bool bPauseFlag = m_pPauseSpr->isVisible();
	if (bPauseFlag)
	{
		m_pPauseSpr->setVisible(false);
		scheduleUpdate();
		return;
	}

	bool bClickPauseFlag = (m_pStartBtn->getSelectedIndex() == 0);
	if (bClickPauseFlag)
	{
		m_pPauseSpr->setVisible(true);
		unscheduleUpdate();
		return;
	}

	SEventContextButton stContext = { BTNID_START, true };
	if (!g_oEventEngine.FireVote(EVENT_BUTTON, (char*)&stContext, sizeof(stContext)))
	{
		// Recover button state
		m_pStartBtn->setSelectedIndex(0);
		return;
	}

	if (stContext.nButtonID != BTNID_START)
	{
		// Recover button state
		m_pStartBtn->setSelectedIndex(0);
	}

	// Fire event
	SEventContextButton stExecContext = { BTNID_START, true };
	g_oEventEngine.FireEvent(EVENT_BUTTON, (const char*)&stExecContext, sizeof(stExecContext));
}


void CGameScene::__OnClickSoundBtn()
{
	bool bState = !GET_BOOLVALUE("SOUND", true);
	SET_BOOLVALUE("SOUND", bState);
	// Play effect
	__PlayEffect(bState ? EFFECT_SOUNDON : EFFECT_SOUNDOFF);
	// Update button
	m_pSoundBtn->setSelectedIndex(bState ? 0 : 1);

	if (!bState)
	{
		__PlayBGM(false);
		return;
	}
	
	SEventPlayBGM stBGM = { true, true };
	if (g_oEventEngine.FireVote(EVENT_PLAY_BGM, (char*)&stBGM, sizeof(stBGM)))
	{
		// BGM
		__PlayBGM(true);
	}
}


void CGameScene::__OnClickResetBtn()
{
	SEventContextButton stContext = { BTNID_RESET, true };
	if (g_oEventEngine.FireVote(EVENT_BUTTON, (char*)&stContext, sizeof(stContext)))
	{
		g_oEventEngine.FireEvent(EVENT_BUTTON, (const char*)&stContext, sizeof(stContext));
	}
}


void CGameScene::__OnClickSetupBtn()
{
	// Stop update
	unscheduleUpdate();

	// Export context
	TGameSceneContext stContext;
	__ExportContext(&stContext);

	bool bPortraitFlag = GET_BOOLVALUE("PORTRAIT", true);
	REPLACE_SCENE(bPortraitFlag ? CSetupScene::CreateScene(&stContext) : CSetupSceneEx::CreateScene(&stContext));
}


void CGameScene::__PlayEffect(const char* szEffect)
{
	bool bState = GET_BOOLVALUE("SOUND", true);
	if (bState)
	{
		PLAY_EFFECT(szEffect);
	}
}


void CGameScene::__PlayBGM(bool bPlayFlag, bool bLoopFlag /*= true*/)
{
	if (!bPlayFlag)
	{
		m_bPlayingBGMFlag = false;
		PAUSE_BGMUSIC();
		return;
	}

	bool bState = GET_BOOLVALUE("SOUND", true);
	if (bState && !m_bPlayingBGMFlag)
	{
		m_bPlayingBGMFlag = true;
		PLAY_BGMUSIC(BGM_START, bLoopFlag);
	}
}

