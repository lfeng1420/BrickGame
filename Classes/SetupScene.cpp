#include "stdafx.h"
#include "SetupScene.h"
#include "GameLogic.h"
#include "SetupVolume.h"
#include "GameScene.h"
#include "SetupControlBtn.h"
#include "SetupSceneEx.h"
#include "GameSceneEx.h"


CSetupScene::~CSetupScene()
{
}

Size CSetupScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    Size visibleSize = GET_VISIBLESIZE();
    bool bTitleFlag = (idx == enMenu_GlobalSetting || idx == enMenu_OtherSetting || idx == enMenu_TetrisSetting);
    return Size(visibleSize.width, bTitleFlag ? MENU_TITLE_HEIGHT : MENU_SPR_HEIGHT);
}

extension::TableViewCell* CSetupScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    string strText;
    extension::TableViewCell* pCell = table->dequeueCell();
    bool bNewCellFlag = false;
    if (pCell == nullptr)
    {
        pCell = new extension::TableViewCell();
        pCell->autorelease();
        pCell->setAnchorPoint(Vec2::ZERO);
        pCell->setPosition(Vec2::ZERO);
        pCell->setContentSize(tableCellSizeForIndex(table, idx));
        bNewCellFlag = true;
    }

    bool bOKFlag = false;
    switch (idx)
    {
    case enMenu_GlobalSetting:
        // Global settings title
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_GLOBALSETTING);
        break;

    case enMenu_Vibration:
        // Vibration
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_VIBRATION, __GetItemStateStrID(enMenu_Vibration));
        break;

    case enMenu_Sound:
        // Sound
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_SOUND, STRID_RIGHTARROW);
        break;

    case enMenu_Orientation:
        // Orientation
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_ORIENTATION, __GetItemStateStrID(enMenu_Orientation));
        break;

    case enMenu_NightMode:
        // Night mode
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_NIGHTMODE, __GetItemStateStrID(enMenu_NightMode));
        break;

    case enMenu_RHMode:
        // Right hand mode
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_RHMODE, __GetItemStateStrID(enMenu_RHMode));
        break;

        // Bricks offset
    case enMenu_BricksOffset:
        __GetItemStateStr(enMenu_BricksOffset, strText);
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_BRICKSOFFSET, STRID_MAX, strText.c_str());
        break;

    case enMenu_DirBtn:
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_DIRBTN, STRID_RIGHTARROW);
        break;

    case enMenu_TetrisSetting:
        // Tetris settings
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_TETRISSETTING);
        break;

    case enMenu_AutoRecover:
        // Auto recover
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_AUTORECOVER, __GetItemStateStrID(enMenu_AutoRecover));
        break;

    case enMenu_SaveNow:
        // Save now
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_SAVENOW, STRID_RIGHTARROW);
        break;

    case enMenu_UpBtn:
        // Up button function
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_UPBTN, __GetItemStateStrID(enMenu_UpBtn));
        break;

    case enMenu_OtherSetting:
        // Other settings
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_OTHER);
        break;

    case enMenu_Author:
        // Author
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_AUTHOR, STRID_LFENG);
        break;

    case enMenu_Rate:
        // Rate
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_RATE, STRID_RIGHTARROW);
        break;

    case enMenu_More:
        // More
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_MORE, STRID_RIGHTARROW);
        break;

    case enMenu_Back:
        // Back
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_BACK, STRID_RIGHTARROW);
        break;

    case enMenu_Line:
        // Unavailable
        bOKFlag = AddMenuItem(pCell, bNewCellFlag, STRID_LINE, STRID_LINE);
        break;

    default:
        break;
    }

    return bOKFlag ? pCell : nullptr;
}

ssize_t CSetupScene::numberOfCellsInTableView(TableView *table)
{
    return enMenu_Max;
}

void CSetupScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    ssize_t index = cell->getIdx();
    __UpdateItemState(index);

    switch (index)
    {
    case enMenu_NightMode:
        __OnNightModeChange(table);
        return;

    case enMenu_Sound:
        ShowVolumeLayer();
        return;

    case enMenu_DirBtn:
        ShowControlBtnSetup();
        return;

    case enMenu_Orientation:
        OnOrientationChange();
        return;

    case enMenu_Back:
        __OnClickBackMenu();
        return;

    case enMenu_SaveNow:
        __OnClickSaveMenu(cell);
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
        CGlobalFunc::OpenURL("https://www.lfengs.com");
        return;

    default:
        break;
    }

    // Update single item
    string strText;
    __GetItemStateStr(index, strText);
    __UpdateOneMenuItem(cell, strText.c_str());
}

Scene* CSetupScene::CreateScene(const TGameSceneContext* pContext /*= nullptr*/)
{
	auto scene = Scene::create();
	auto layer = CSetupScene::create(pContext);
	scene->addChild(layer);
	return scene;
}


Layer* CSetupScene::create(const TGameSceneContext* pContext /*= nullptr*/)
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

	// TableView
    TableView* pTableView = TableView::create(this, Size(visibleSize.width, visibleSize.height));
    pTableView->setDirection(extension::ScrollView::Direction::VERTICAL);
    pTableView->setPosition(Vec2::ZERO);
    pTableView->setDelegate(this);
    pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(pTableView);
    pTableView->reloadData();

	// Tips
	m_pTipsLabel = Label::createWithSystemFont("", FONT_NAME, TIPS_LABEL_SIZE);
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
	CSetupVolume* pLayer = CSetupVolume::create();
	pLayer->setPosition(Vec2::ZERO);
	this->addChild(pLayer);
}


void CSetupScene::ShowControlBtnSetup()
{
    CSetupControlBtn* pLayer = CSetupControlBtn::create();
    pLayer->setPosition(Vec2::ZERO);
    this->addChild(pLayer);
}

bool CSetupScene::AddMenuItem(extension::TableViewCell* pCell, bool bNewCellFlag,  int nStrID, int nStatusStrID /*= STRID_MAX*/, const char* szStr /*= nullptr*/)
{
	Size visibleSize = GET_VISIBLESIZE();
	float fMenuPosX = MENU_LABEL_PADDING;
	float fStatusPosX = visibleSize.width - MENU_LABEL_PADDING;
	bool bNightMode = GET_BOOLVALUE("NIGHTMODE", false);
    szStr = (szStr == nullptr && nStatusStrID != STRID_MAX) ? CGlobalFunc::GetString(nStatusStrID) : szStr;

    if (!bNewCellFlag)
    {
        Label* pLabel = (Label*)pCell->getChildByTag(MENU_LABEL_TAG);
        if (pLabel != nullptr)
        {
            pLabel->setString(CGlobalFunc::GetString(nStrID));
            pLabel->setSystemFontSize((szStr == nullptr) ? TITLE_FONT : NORMAL_FONT);
            pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
        }

        pLabel = (Label*)pCell->getChildByTag(MENU_STATUS_TAG);
        if (pLabel != nullptr)
        {
            pLabel->setString((szStr != nullptr) ? szStr : "");
            pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
        }
        return true;
    }

    Scale9Sprite* pButton = Scale9Sprite::createWithSpriteFrameName(BTN_SPR);
	pButton->setContentSize(Size(visibleSize.width, MENU_SPR_HEIGHT));
    pButton->setPosition(Vec2::ZERO);
    pCell->addChild(pButton);

	Label* pLabel = Label::createWithSystemFont(CGlobalFunc::GetString(nStrID), FONT_NAME, (szStr == nullptr) ? TITLE_FONT : NORMAL_FONT);
	Size contentSize = GET_CONTENTSIZE(pLabel);
    pLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    pLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	pLabel->setPosition(Vec2(fMenuPosX, 0));
	pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
    pCell->addChild(pLabel);
	pLabel->setTag(MENU_LABEL_TAG);
    
    Label* pStatus = Label::createWithSystemFont((szStr != nullptr) ? szStr : "", FONT_NAME, NORMAL_FONT);
    contentSize = GET_CONTENTSIZE(pStatus);
    pStatus->setHorizontalAlignment(TextHAlignment::RIGHT);
    pStatus->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    pStatus->setPosition(Vec2(fStatusPosX, 0));
    pStatus->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
    pCell->addChild(pStatus);
    pStatus->setTag(MENU_STATUS_TAG);

    return true;
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
	case enMenu_DirBtn:
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


void CSetupScene::__GetItemStateStr(int nMenuID, string& strText)
{
    switch (nMenuID)
    {
    case enMenu_BricksOffset:
        strText = std::to_string(GET_INTVALUE("BRICKS_OFFSET", 0));
        return;

    default:
        break;
    }

	int nStrID = __GetItemStateStrID(nMenuID);
	strText = CGlobalFunc::GetString(__GetItemStateStrID(nMenuID));
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


void CSetupScene::__UpdateOneMenuItem(Node* pNode, const char* szText, bool bUpdateColorFlag /*= false*/)
{
	Node* pChild = pNode->getChildByTag(MENU_STATUS_TAG);
	if (pChild == nullptr)
	{
		return;
	}

	// Update content
	Label* pText = (Label*)pChild;
	Size oldSize = GET_CONTENTSIZE(pText);
	pText->setString(szText);
	Size newSize = GET_CONTENTSIZE(pText);
	pText->setPositionX(pText->getPositionX() - (newSize.width * 0.5f - oldSize.width * 0.5f));
	
	// Update color
	if (bUpdateColorFlag)
	{
		Color3B color = GET_BOOLVALUE("NIGHTMODE", false) ? Color3B::WHITE : Color3B::BLACK;
		pText->setColor(color);
		pChild = pNode->getChildByTag(MENU_LABEL_TAG);
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


void CSetupScene::__OnNightModeChange(extension::TableView* table)
{
	// Update layer visiblity
	m_pLayer->setVisible(!__GetItemState(enMenu_NightMode));

	// Update labels
    string strText;
	for (unsigned int u32MenuIdx = 0; u32MenuIdx < enMenu_Max; ++u32MenuIdx)
	{
        TableViewCell* pCell = table->cellAtIndex(u32MenuIdx);
        if (pCell != nullptr)
        {
            __GetItemStateStr(u32MenuIdx, strText);
            __UpdateOneMenuItem(pCell, strText.c_str(), true);
        }
	}
}


void CSetupScene::__OnClickBackMenu()
{
	bool bPortraitFlag = GET_BOOLVALUE("PORTRAIT", true);
	REPLACE_SCENE(bPortraitFlag ? CGameScene::CreateScene(&m_stGameSceneContext) : CGameSceneEx::CreateScene(&m_stGameSceneContext));
}


void CSetupScene::__OnClickSaveMenu(extension::TableViewCell* cell)
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
        __UpdateOneMenuItem(cell, CGlobalFunc::GetString(__GetItemStateStrID(enMenu_AutoRecover)));
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

    case STRID_DIRBTN:
        return enMenu_DirBtn;

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