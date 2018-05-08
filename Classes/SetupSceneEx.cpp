#include "stdafx.h"
#include "SetupSceneEx.h"
#include "MyListView.h"
#include "VolumeLayerEx.h"


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
	m_pLayer->setPosition(Vec2::ZERO);
	m_pLayer->setContentSize(Size(visibleSize.height, visibleSize.width));
	m_pLayer->setPosition(Vec2(-(visibleSize.height - visibleSize.width) * 0.5f, -(visibleSize.height - visibleSize.width) * 0.5f));
	m_pLayer->setVisible(!__GetItemState(enMenu_NightMode));
	this->addChild(m_pLayer);

	// List view
	m_pListView = CMyListView::create();
	m_pListView->setBounceEnabled(false);
	m_pListView->setContentSize(Size(visibleSize.height, visibleSize.width));
	m_pListView->setGravity(ListView::Gravity::TOP);
	m_pListView->setPosition(Vec2(-(visibleSize.height - visibleSize.width) * 0.5f, -(visibleSize.height - visibleSize.width) * 0.5f));
	m_pListView->setItemsMargin(MENU_ITEM_MARGIN);
	m_pListView->addEventListener((const ListView::ccListViewCallback&)CC_CALLBACK_2(CSetupSceneEx::ListViewCallback, this));
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
	// Direction button scale
	string strText;
	__GetItemStateStr(enMenu_DirBtnScale, strText);
	AddMenuItem(STRID_DIRBTNSCALE, STRID_MAX, strText.c_str());

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
	CVolumeLayerEx* pLayer = CVolumeLayerEx::create();
	pLayer->setContentSize(Size(visibleSize.height, visibleSize.width));
	pLayer->setPosition(Vec2(-(visibleSize.height - visibleSize.width) * 0.5f, -(visibleSize.height - visibleSize.width) * 0.5f));
	this->addChild(pLayer);
}


void CSetupSceneEx::AddMenuItem(int nStrID, int nStatusStrID /*= STRID_MAX*/, const char* szStr /*= nullptr*/)
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
	float fStatusPosX = visibleSize.height - MENU_LABEL_PADDING;
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
	pButton->setContentSize(Size(visibleSize.height, MENU_SPR_HEIGHT));
	Text* pLabel = Text::create(CGlobalFunc::GetString(nStrID), FONT_NAME, NORMAL_FONT);
	Size contentSize = GET_CONTENTSIZE(pLabel);
	pLabel->setTextHorizontalAlignment(TextHAlignment::LEFT);
	pLabel->setPosition(Vec2(fMenuPosX + contentSize.width * 0.5f, MENU_SPR_HEIGHT * 0.5f));
	pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	pButton->addChild(pLabel);
	pLabel->setTag(MENU_LABEL_TAG);
	Text* pStatus = Text::create(((szStr != nullptr) ? szStr : CGlobalFunc::GetString(nStatusStrID)), FONT_NAME, NORMAL_FONT);
	contentSize = GET_CONTENTSIZE(pStatus);
	pStatus->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	pStatus->setPosition(Vec2(fStatusPosX - contentSize.width * 0.5f, MENU_SPR_HEIGHT * 0.5f));
	pStatus->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	pButton->addChild(pStatus);
	pStatus->setTag(MENU_STATUS_TAG);
	m_pListView->pushBackCustomItem(pButton);
}

