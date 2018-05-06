#include "stdafx.h"
#include "SetupScene.h"
#include "GameLogic.h"
#include "VolumeLayer.h"
#include "SetupSceneEx.h"
#include "GameScene.h"
#include "GameSceneEx.h"


CSetupScene::~CSetupScene()
{
}


cocos2d::Scene* CSetupScene::CreateScene(const TGameSceneContext* pContext /*= nullptr*/)
{
	auto scene = Scene::create();
	auto layer = CSetupScene::create(pContext);
	scene->addChild(layer);
	return scene;
}


cocos2d::Layer* CSetupScene::create(const TGameSceneContext* pContext /*= nullptr*/)
{
	CSetupScene* pLayer = new CSetupScene();
	if (pLayer == nullptr)
	{
		return nullptr;
	}

	// import context
	pLayer->ImportGameSceneContext(pContext);

	// init
	if (!pLayer->init())
	{
		delete pLayer;
		return nullptr;
	}

	pLayer->autorelease();
	return pLayer;
}


bool CSetupScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	InitMainUI();
	__CreateKeyListener();

	return true;
}


void CSetupScene::ImportGameSceneContext(const TGameSceneContext* pContext /*= nullptr*/)
{
	if (pContext != nullptr)
	{
		m_stGameSceneContext = *pContext;
		return;
	}

	INIT_BOOL_ARRAY(m_stGameSceneContext.arrBrickState);
	INIT_BOOL_ARRAY(m_stGameSceneContext.arrSmallBrickState);
	m_stGameSceneContext.stGameData.nCurScore = 0;
	m_stGameSceneContext.stGameData.nLevel = 0;
	m_stGameSceneContext.stGameData.nMaxScore = 0;
	m_stGameSceneContext.stGameData.nSpeed = 0;
}


void CSetupScene::InitMainUI()
{
	Size visibleSize = GET_VISIBLESIZE();

	// Layer
	m_pLayer = LayerColor::create(Color4B::WHITE);
	m_pLayer->setPosition(Vec2::ZERO);
	m_pLayer->setContentSize(visibleSize);
	m_pLayer->setVisible(!__GetItemState(enMenu_NightMode));
	this->addChild(m_pLayer);

	// ListView
	m_pListView = ListView::create();
	m_pListView->setContentSize(visibleSize);
	m_pListView->setBounceEnabled(false);
	m_pListView->setGravity(ListView::Gravity::TOP);
	m_pListView->setPosition(Vec2(0, -MENU_ITEM_MARGIN));
	m_pListView->setItemsMargin(MENU_ITEM_MARGIN);
	m_pListView->addEventListener((const ListView::ccListViewCallback&)CC_CALLBACK_2(CSetupScene::ListViewCallback, this));
	this->addChild(m_pListView);

	// Global settings title
	AddMenuItem(STRID_GLOBALSETTING);
	// Vibration
	AddMenuItem(STRID_VIBRATION, __GetItemStateStrID(enMenu_Vibration));
	// Sound
	AddMenuItem(STRID_SOUND, STRID_RIGHTARROW);
	// Orientation
	AddMenuItem(STRID_ORIENTATION, __GetItemStateStrID(enMenu_Orientation));
	// Night mode
	AddMenuItem(STRID_NIGHTMODE, __GetItemStateStrID(enMenu_NightMode));
	// Right hand mode
	AddMenuItem(STRID_RHMODE, __GetItemStateStrID(enMenu_RHMode));
	// Bricks offset
	char szText[10] = { 0 };
	sprintf(szText, "%d", GET_INTVALUE("BRICKS_OFFSET", 0));
	AddMenuItem(STRID_BRICKSOFFSET, STRID_MAX, szText);

	// Tetris settings
	AddMenuItem(STRID_TETRISSETTING);
	// Auto recover
	AddMenuItem(STRID_AUTORECOVER, __GetItemStateStrID(enMenu_AutoRecover));
	// Save now
	AddMenuItem(STRID_SAVENOW, STRID_RIGHTARROW);
	// Up button function
	AddMenuItem(STRID_UPBTN, __GetItemStateStrID(enMenu_UpBtn));

	// Other settings
	AddMenuItem(STRID_OTHER);
	// Author
	AddMenuItem(STRID_AUTHOR, STRID_LFENG);
	// Rate
	AddMenuItem(STRID_RATE, STRID_RIGHTARROW);
	// More
	AddMenuItem(STRID_MORE, STRID_RIGHTARROW);
	// Back
	AddMenuItem(STRID_BACK, STRID_RIGHTARROW);
	// Unavailable
	AddMenuItem(STRID_LINE, STRID_LINE);

	// Tips
	m_pTipsLabel = Label::createWithSystemFont("123", FONT_NAME, TIPS_LABEL_SIZE);
	m_pTipsLabel->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	m_pTipsLabel->setOpacity(0);
	this->addChild(m_pTipsLabel);
}


void CSetupScene::OnOrientationChange()
{
	Scene* pScene = CSetupSceneEx::CreateScene(&m_stGameSceneContext);
	if (pScene != nullptr)
	{
		REPLACE_SCENE(pScene);
	}
}


void CSetupScene::ShowVolumeLayer()
{
	CVolumeLayer* pLayer = CVolumeLayer::create();
	pLayer->setPosition(Vec2::ZERO);
	this->addChild(pLayer);
}


void CSetupScene::AddMenuItem(int nStrID, int nStatusStrID /*= STRID_MAX*/, const char* szStr /*= nullptr*/)
{
	// Add menu id
	int nMenuID = __StrID2MenuID(nStrID);
	if (nMenuID == enMenu_Max)
	{
		return;
	}
	m_vecMenu.push_back(nMenuID);

	Size visibleSize = GET_VISIBLESIZE();
	float fMenuPosX = MENU_LABEL_PADDING;
	float fStatusPosX = visibleSize.width - MENU_LABEL_PADDING;
	bool bNightMode = GET_BOOLVALUE("NIGHTMODE", false);

	if (nStatusStrID == STRID_MAX && szStr == nullptr)
	{
		Text* pTitleText = Text::create(CGlobalFunc::GetString(nStrID), FONT_NAME, TITLE_FONT);
		pTitleText->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
		m_pListView->pushBackCustomItem(pTitleText);
		return;
	}

	Button* pButton = Button::create(BTN_SPR, BTN_CLICK_SPR, "", Widget::TextureResType::PLIST);
	pButton->setScale9Enabled(true);
	pButton->setContentSize(Size(visibleSize.width, MENU_SPR_HEIGHT));
	Text* pLabel = Text::create(CGlobalFunc::GetString(nStrID), FONT_NAME, NORMAL_FONT);
	Size contentSize = GET_CONTENTSIZE(pLabel);
	pLabel->setPosition(Vec2(fMenuPosX + contentSize.width * 0.5f, MENU_SPR_HEIGHT * 0.5f));
	pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	pButton->addChild(pLabel);
	pLabel->setTag(MENU_LABEL_TAG);
	Text* pStatus = Text::create(((szStr == nullptr) ? CGlobalFunc::GetString(nStatusStrID) : szStr), FONT_NAME, NORMAL_FONT);
	contentSize = GET_CONTENTSIZE(pStatus);
	pStatus->setPosition(Vec2(fStatusPosX - contentSize.width * 0.5f, MENU_SPR_HEIGHT * 0.5f));
	pStatus->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	pButton->addChild(pStatus);
	pStatus->setTag(MENU_STATUS_TAG);
	m_pListView->pushBackCustomItem(pButton);
}


void CSetupScene::ListViewCallback(Ref* pNode, ui::ListView::EventType enEventType)
{
	if (enEventType != ListView::EventType::ON_SELECTED_ITEM_END)
	{
		return;
	}

	unsigned int uiMenuIdx = m_pListView->getCurSelectedIndex();
	if (uiMenuIdx >= m_vecMenu.size())
	{
		return;
	}
	int nMenuID = m_vecMenu[uiMenuIdx];
	__UpdateItemState(nMenuID);
	
	switch (nMenuID)
	{
	case enMenu_NightMode:
		__OnNightModeChange();
		return;

	case enMenu_Sound:
		ShowVolumeLayer();
		return;

	case enMenu_Orientation:
		OnOrientationChange();
		return;

	case enMenu_Back:
		__OnClickBackMenu();
		return;

	case enMenu_SaveNow:
		__OnClickSaveMenu();
		return;

	case enMenu_Rate:
		CGlobalFunc::RateApp();
		return;

	case enMenu_More:
		CGlobalFunc::ShowMyApps();
		return;

	case enMenu_Line:
		__OnClickBackMenu();
		return;

	case enMenu_Author:
		CGlobalFunc::OpenURL("https://lfeng1420.github.io");
		return;

	default:
		break;
	}

	// Update single item
	__UpdateOneMenuItem(uiMenuIdx, nMenuID);
}


bool CSetupScene::__GetItemState(int nMenuID)
{
	switch (nMenuID)
	{
	case enMenu_Vibration:
		return GET_BOOLVALUE("VIBRATION", false);
		break;

	case enMenu_Sound:
		return GET_BOOLVALUE("SOUND", true);
		break;

	case enMenu_Orientation:
		return GET_BOOLVALUE("PORTRAIT", true);
		break;

	case enMenu_NightMode:
		return GET_BOOLVALUE("NIGHTMODE", false);
		break;

	case enMenu_RHMode:
		return GET_BOOLVALUE("RHMODE", false);
		break;

	case enMenu_AutoRecover:
		return GET_BOOLVALUE("TETRIS_RECORD_VALID", false);
		break;

	case enMenu_UpBtn:
		return GET_BOOLVALUE("UPBUTTON", false);
		break;

	case enMenu_SaveNow:
	case enMenu_Author:
	case enMenu_Rate:
	case enMenu_More:
	case enMenu_Back:
	default:
		return false;
		break;
	}
}


int CSetupScene::__GetItemStateStrID(int nMenuID)
{
	switch (nMenuID)
	{
	case enMenu_Vibration:
		return GET_BOOLVALUE("VIBRATION", false) ? STRID_ON : STRID_OFF;
		break;

	case enMenu_Orientation:
		return GET_BOOLVALUE("PORTRAIT", true) ? STRID_PORTRAIT : STRID_LANDSCAPE;
		break;

	case enMenu_NightMode:
		return GET_BOOLVALUE("NIGHTMODE", false) ? STRID_ON : STRID_OFF;
		break;

	case enMenu_RHMode:
		return GET_BOOLVALUE("RHMODE", false) ? STRID_ON : STRID_OFF;
		break;

	case enMenu_AutoRecover:
		return GET_BOOLVALUE("TETRIS_RECORD_VALID", false) ? STRID_ON : STRID_OFF;
		break;

	case enMenu_UpBtn:
		return GET_BOOLVALUE("UPBUTTON", false) ? STRID_QUICKLAND : STRID_ROTATE;
		break;

	case enMenu_Author:
		return STRID_LFENG;
		break;

	case enMenu_Line:
		return STRID_LINE;
		break;

	case enMenu_Sound:
	case enMenu_SaveNow:
	case enMenu_Rate:
	case enMenu_More:
	case enMenu_Back:
		return STRID_RIGHTARROW;
		break;

	default:
		return STRID_MAX;
		break;
	}
}


void CSetupScene::__UpdateItemState(int nMenuID)
{
	bool bNowState = __GetItemState(nMenuID);
	switch (nMenuID)
	{
	case enMenu_Vibration:
		SET_BOOLVALUE("VIBRATION", !bNowState);
		break;

	case enMenu_NightMode:
		SET_BOOLVALUE("NIGHTMODE", !bNowState);
		break;

	case enMenu_Orientation:
		SET_BOOLVALUE("PORTRAIT", !bNowState);
		break;

	case enMenu_RHMode:
		SET_BOOLVALUE("RHMODE", !bNowState);
		break;

	case enMenu_AutoRecover:
		SET_BOOLVALUE("TETRIS_RECORD_VALID", !bNowState);
		break;

	case enMenu_UpBtn:
		SET_BOOLVALUE("UPBUTTON", !bNowState);
		break;

	case enMenu_BricksOffset:
		{
			int nOffset = GET_INTVALUE("BRICKS_OFFSET", 0) + 1;
			if (nOffset > BRICKS_OFFSET_MAX)
			{
				nOffset = 0;
			}
			SET_INTVALUE("BRICKS_OFFSET", nOffset);
		}
		break;

	default:
		break;
	}
}


void CSetupScene::__UpdateOneMenuItem(int nMenuIdx, int nMenuID, bool bUpdateColorFlag /*= false*/)
{
	Widget* pWidget = m_pListView->getItem(nMenuIdx);
	if (pWidget == nullptr)
	{
		return;
	}

	Node* pChild = pWidget->getChildByTag(MENU_STATUS_TAG);
	if (pChild == nullptr)
	{
		return;
	}

	string strText = CGlobalFunc::GetString(__GetItemStateStrID(nMenuID));
	if (nMenuID == enMenu_BricksOffset)
	{
		char szText[10] = { 0 };
		sprintf(szText, "%d", GET_INTVALUE("BRICKS_OFFSET", 0));
		strText.assign(szText);
	}

	// Update content
	Text* pText = (Text*)pChild;
	Size oldSize = GET_CONTENTSIZE(pText);
	pText->setString(strText);
	Size newSize = GET_CONTENTSIZE(pText);
	pText->setPositionX(pText->getPositionX() - (newSize.width * 0.5f - oldSize.width * 0.5f));
	
	// Update color
	if (bUpdateColorFlag)
	{
		Color3B color = GET_BOOLVALUE("NIGHTMODE", false) ? Color3B::WHITE : Color3B::BLACK;
		pText->setColor(color);
		pChild = pWidget->getChildByTag(MENU_LABEL_TAG);
		if (pChild != nullptr)
		{
			pChild->setColor(color);
		}
	}
}


void CSetupScene::__ShowTips(int nStrID)
{
	m_pTipsLabel->setString(CGlobalFunc::GetString(nStrID));
	m_pTipsLabel->setColor(__GetItemState(enMenu_NightMode) ? Color3B::WHITE : Color3B::BLACK);
	m_pTipsLabel->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(1.0f), FadeOut::create(0.5f), nullptr));
}


void CSetupScene::__CreateKeyListener()
{
	EventListenerKeyboard* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
	};

	keyListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (EventKeyboard::KeyCode::KEY_RETURN == keyCode ||
			EventKeyboard::KeyCode::KEY_ESCAPE == keyCode ||
			EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
		{
			__OnClickBackMenu();
			return;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}


void CSetupScene::__OnNightModeChange()
{
	// Update layer visiblity
	m_pLayer->setVisible(!__GetItemState(enMenu_NightMode));

	// Update labels
	Color3B color = __GetItemState(enMenu_NightMode) ? Color3B::WHITE : Color3B::BLACK;
	for (unsigned int uiMenuIdx = 0; uiMenuIdx < m_vecMenu.size(); ++uiMenuIdx)
	{
		int nMenuID = m_vecMenu[uiMenuIdx];
		if (nMenuID == enMenu_GlobalSetting
			|| nMenuID == enMenu_OtherSetting
			|| nMenuID == enMenu_TetrisSetting)
		{
			Widget* pWidget = m_pListView->getItem(uiMenuIdx);
			if (pWidget != nullptr)
			{
				pWidget->setColor(color);
				continue;
			}
		}

		__UpdateOneMenuItem(uiMenuIdx, nMenuID, true);
	}
}


void CSetupScene::__OnClickBackMenu()
{
	bool bPortraitFlag = GET_BOOLVALUE("PORTRAIT", true);
	REPLACE_SCENE(bPortraitFlag ? CGameScene::CreateScene(&m_stGameSceneContext) : CGameSceneEx::CreateScene(&m_stGameSceneContext));
}


void CSetupScene::__OnClickSaveMenu()
{
	// Save now
	SEventContextSaveData stContext = { true };
	bool bSaveOKFlag = g_oEventEngine.FireVote(EVENT_SAVE_DATA, (char*)&stContext, sizeof(stContext));
	int nStrID = (bSaveOKFlag ? STRID_SAVEOK : STRID_SAVEFAIL);
	__ShowTips(nStrID);

	if (bSaveOKFlag && !GET_BOOLVALUE("TETRIS_RECORD_VALID", false))
	{
		// Open auto recover
		SET_BOOLVALUE("TETRIS_RECORD_VALID", true);
		// Update auto recover menu
		for (unsigned int uiMenuIdx = 0; uiMenuIdx < m_vecMenu.size(); ++uiMenuIdx)
		{
			if (m_vecMenu[uiMenuIdx] == enMenu_AutoRecover)
			{
				__UpdateOneMenuItem(uiMenuIdx, enMenu_AutoRecover);
				break;
			}
		}
	}
}


int CSetupScene::__StrID2MenuID(int nStrID)
{
	switch (nStrID)
	{
	case STRID_GLOBALSETTING:
		return enMenu_GlobalSetting;

	case STRID_VIBRATION:
		return enMenu_Vibration;

	case STRID_SOUND:
		return enMenu_Sound;

	case STRID_ORIENTATION:
		return enMenu_Orientation;

	case STRID_NIGHTMODE:
		return enMenu_NightMode;

	case STRID_RHMODE:
		return enMenu_RHMode;

	case STRID_BRICKSOFFSET:
		return enMenu_BricksOffset;

	case STRID_TETRISSETTING:
		return enMenu_TetrisSetting;

	case STRID_AUTORECOVER:
		return enMenu_AutoRecover;

	case STRID_SAVENOW:
		return enMenu_SaveNow;

	case STRID_UPBTN:
		return enMenu_UpBtn;

	case STRID_OTHER:
		return enMenu_OtherSetting;

	case STRID_AUTHOR:
		return enMenu_Author;

	case STRID_RATE:
		return enMenu_Rate;

	case STRID_MORE:
		return enMenu_More;

	case STRID_BACK:
		return enMenu_Back;

	case STRID_LINE:
		return enMenu_Line;

	default:
		return enMenu_Max;
		break;
	}
}