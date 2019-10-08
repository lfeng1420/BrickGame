#include "stdafx.h"
#include "SetupSceneEx.h"
#include "SetupVolumeEx.h"
#include "SetupControlBtnEx.h"


cocos2d::Scene* CSetupSceneEx::CreateScene(const TGameSceneContext* pContext /*= nullptr*/)
{
	Size visibleSize = GET_VISIBLESIZE();
	auto scene = Scene::create();
	auto layer = CSetupSceneEx::create(pContext);
	layer->setRotation(90);
	layer->setContentSize(Size(visibleSize.height, visibleSize.width));
	scene->addChild(layer);
	return scene;
}


cocos2d::Layer* CSetupSceneEx::create(const TGameSceneContext* pContext /*= nullptr*/)
{
	CSetupSceneEx* pLayer = new CSetupSceneEx();
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


void CSetupSceneEx::InitMainUI()
{
	Size visibleSize = GET_VISIBLESIZE();

	// Layer
    m_pLayer = LayerColor::create(Color4B::WHITE);
    //m_pLayer->setPosition(Vec2::ZERO);
	m_pLayer->setContentSize(Size(visibleSize.height, visibleSize.width));
	m_pLayer->setPosition(Vec2(-(visibleSize.height - visibleSize.width) * 0.5f, -(visibleSize.height - visibleSize.width) * 0.5f));
	m_pLayer->setVisible(!__GetItemState(enMenu_NightMode));
	this->addChild(m_pLayer);

	// List view
    int nMenuHeight = (enMenu_Max - MENU_TITLE_COUNT) * MENU_SPR_HEIGHT + MENU_TITLE_COUNT * MENU_TITLE_HEIGHT;
    TableView* pTableView = TableView::create(this, Size(visibleSize.width, visibleSize.height));
    pTableView->setDirection(extension::ScrollView::Direction::VERTICAL);
    pTableView->setPosition(visibleSize.width + (visibleSize.height - visibleSize.width) * 0.5f, -(visibleSize.height - visibleSize.width) * 0.5f);
    pTableView->setDelegate(this);
    pTableView->setCustomOffset(Vec2(0, visibleSize.width - nMenuHeight));
    pTableView->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(pTableView);
    pTableView->reloadData();

	// Tips
	m_pTipsLabel = Label::createWithSystemFont("", FONT_NAME, TIPS_LABEL_SIZE);
	m_pTipsLabel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.width - visibleSize.height * 0.5f));
	m_pTipsLabel->setOpacity(0);
	this->addChild(m_pTipsLabel);
}


void CSetupSceneEx::OnOrientationChange()
{
	Scene* pScene = CSetupScene::CreateScene(&m_stGameSceneContext);
	if (pScene != nullptr)
	{
		REPLACE_SCENE(pScene);
	}
}


void CSetupSceneEx::ShowVolumeLayer()
{
	Size visibleSize = GET_VISIBLESIZE();
	CSetupVolumeEx* pLayer = CSetupVolumeEx::create();
	pLayer->setContentSize(Size(visibleSize.height, visibleSize.width));
	pLayer->setPosition(Vec2(-(visibleSize.height - visibleSize.width) * 0.5f, -(visibleSize.height - visibleSize.width) * 0.5f));
	this->addChild(pLayer);
}


void CSetupSceneEx::ShowControlBtnSetup()
{
    Size visibleSize = GET_VISIBLESIZE();
    CSetupControlBtnEx* pLayer = CSetupControlBtnEx::create();
    pLayer->setContentSize(Size(visibleSize.height, visibleSize.width));
    pLayer->setPosition(Vec2(-(visibleSize.height - visibleSize.width) * 0.5f, -(visibleSize.height - visibleSize.width) * 0.5f));
    this->addChild(pLayer);
}

bool CSetupSceneEx::AddMenuItem(extension::TableViewCell* pCell, bool bNewCellFlag, int nStrID, int nStatusStrID /*= STRID_MAX*/, const char* szStr /*= nullptr*/)
{
    //pCell->setRotation(90);
	Size visibleSize = GET_VISIBLESIZE();
	float fMenuPosX = -visibleSize.height + MENU_LABEL_PADDING;
	float fStatusPosX = -MENU_LABEL_PADDING;
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
	//pLabel->setPosition(Vec2(fMenuPosX + contentSize.width * 0.5f, MENU_SPR_HEIGHT * 0.5f));
	pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	pCell->addChild(pLabel);
	pLabel->setTag(MENU_LABEL_TAG);

    Label* pStatus = Label::createWithSystemFont((szStr != nullptr) ? szStr : "", FONT_NAME, NORMAL_FONT);
	contentSize = GET_CONTENTSIZE(pStatus);
	pStatus->setHorizontalAlignment(TextHAlignment::RIGHT);
    pStatus->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    pStatus->setPosition(Vec2(fStatusPosX, 0));
	//pStatus->setPosition(Vec2(fStatusPosX - contentSize.width * 0.5f, MENU_SPR_HEIGHT * 0.5f));
	pStatus->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
    pCell->addChild(pStatus);
	pStatus->setTag(MENU_STATUS_TAG);

    return true;
}

cocos2d::Size CSetupSceneEx::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    Size visibleSize = GET_VISIBLESIZE();
    bool bTitleFlag = (idx == enMenu_GlobalSetting || idx == enMenu_OtherSetting || idx == enMenu_TetrisSetting);
    return Size(visibleSize.width, bTitleFlag ? MENU_TITLE_HEIGHT : MENU_SPR_HEIGHT);
}
