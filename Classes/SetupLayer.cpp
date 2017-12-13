#include "SetupLayer.h"
#include "GeneralManager.h"
#include "GameScene.h"
#include "VolumeLayer.h"
#include "BarrierLayer.h"

USING_NS_CC;

//菜单选项对应字符串ID
const string MENU_SPRITE_STRING[MENU_MAX][2] =
{
	{ "vibrate_off.png", "vibrate.png"  },		//MENU_VIBRATION
	{ "sound.png", "sound.png" },				//MENU_SOUND
	{ "sun.png", "moon.png" },					//MENU_NIGHTMODE
	{ "portrait.png", "landscape.png" },		//MENU_ORIENTATION
	{ "lefthand.png", "righthand.png" },		//MENU_RHMODE
	{ "autorecover.png", "autorecover.png" },	//MENU_AUTORECOVER
	{ "save.png", "save.png" },					//MENU_SAVENOW
	{ "rotate.png", "down.png" },				//MENU_UPBTN
	{ "lfeng.png", "lfeng.png" },				//MENU_AUTHOR
	{ "like.png", "like.png" },					//MENU_RATE
	{ "back.png", "back.png" },					//MENU_BACK
};

//主菜单和子菜单关系
const int MENU_RELATION[MAINMENU_MAX][5] = 
{
	{ MENU_VIBRATION, MENU_SOUND, MENU_NIGHTMODE, MENU_ORIENTATION, MENU_RHMODE },
	{ MENU_AUTORECOVER, MENU_SAVENOW, MENU_UPBTN, -1, -1 },
	{ MENU_AUTHOR, MENU_RATE, MENU_BACK, -1, -1 },
};


const float LANDSCAPE_MENU_SCALE = 1.03f;
const float PORTRAIT_MENU_SCALE = 1.6f;



CSetupLayer::CSetupLayer() : m_pGameScene(nullptr)
{
}


CSetupLayer::~CSetupLayer()
{
}


bool CSetupLayer::Init(CGameScene* pGameScene)
{
	if (pGameScene == nullptr)
	{
		return false;
	}
	m_pGameScene = pGameScene;

	//创建所有菜单
	initAllMenu();

	//调整位置
	adjustMenuPos();

	return true;
}


void CSetupLayer::UpdateSingleMenu(int nMenuIdx)
{
	MenuItem* pMenuItem = m_vecMenuItem.at(nMenuIdx);
	if (pMenuItem == nullptr)
	{
		return;
	}
	
	updateSingleMenu(pMenuItem, nMenuIdx);
}


void CSetupLayer::updateAllMenu()
{
	for (int nIndex = 0; nIndex < m_vecMenuItem.size(); ++nIndex)
	{
		UpdateSingleMenu(nIndex);
	}

	//更新Label
	bool bNightMode = getValueByMenuIdx(MENU_NIGHTMODE);
	for (int nIndex = 0; nIndex < m_vecLabel.size(); ++nIndex)
	{
		Label* pLabel = m_vecLabel.at(nIndex);
		if (pLabel == nullptr)
		{
			continue;
		}

		pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
	}
}


int CSetupLayer::getValueByMenuIdx(int nMenuIdx)
{
	switch (nMenuIdx)
	{
	case MENU_VIBRATION:
		return GET_INTVALUE("VIBRATION", 0);		//0关闭 1开启
		break;
	case MENU_SOUND:
		return GET_BOOLVALUE("SOUND", true); 		//0关闭 1开启
		break;
	case MENU_NIGHTMODE:
		return GET_BOOLVALUE("NIGHTMODE", false);	//0关闭 1开启
		break;
	case MENU_ORIENTATION:
		return GET_BOOLVALUE("PORTRAIT", true);		//0横屏 1竖屏
		break;
	case MENU_RHMODE:
		return GET_BOOLVALUE("RHMODE", false);		//0关闭 1开启
		break;
	case MENU_AUTORECOVER:
		return GET_BOOLVALUE("TETRIS_RECORD_VALID", false);	//0关闭 1开启
		break;
	case MENU_UPBTN:
		return GET_INTVALUE("UPBUTTON", 0);			//0旋转 1直落
		break;
	default:
		return 0;
		break;
	}
}


void CSetupLayer::onClickMenu(Ref* pSender, int nMenuIdx)
{
	MenuItem* pMenuItem = static_cast<MenuItem*>(pSender);
	int nValue = getValueByMenuIdx(nMenuIdx);
	nValue = ((nValue != 0) ? 0 : 1);
	bool bState = ((nValue == 1) ? true : false);

	//各个菜单其他处理
	switch (nMenuIdx)
	{
		case MENU_VIBRATION:
		{
			SET_INTVALUE("VIBRATION", nValue);			//0开启 1关闭
			updateSingleMenu(pMenuItem, nMenuIdx);
		}
		break;
		case MENU_SOUND:
		{
			CVolumeLayer* pLayer = CVolumeLayer::create();
			pLayer->setPosition(Size::ZERO);
			this->addChild(pLayer);
		}
		break;
		case MENU_NIGHTMODE:
		{
			SET_BOOLVALUE("NIGHTMODE", bState);				//0日间模式 1夜间模式
			m_pGameScene->ChangeColorMode();
			updateAllMenu();
		}
		break;
		case MENU_ORIENTATION:
		{
			SET_BOOLVALUE("PORTRAIT", bState);				//0横屏 1竖屏
			updateAllMenu();
			adjustMenuPos();
		}
		break;
		case MENU_RHMODE:
		{
			SET_BOOLVALUE("RHMODE", bState);	//0关闭 1开启
			m_pGameScene->ChangeControllerPos();
			updateSingleMenu(pMenuItem, nMenuIdx);
		}
		break;
		case MENU_AUTORECOVER:
		{
			SET_BOOLVALUE("TETRIS_RECORD_VALID", bState);	//0关闭 1开启
			m_pGameScene->ShowTips(bState ? CGameScene::TIPS_SAVEOPEN : CGameScene::TIPS_SAVECLOSE);
			updateSingleMenu(pMenuItem, nMenuIdx);
		}
		break;
		case MENU_SAVENOW:
		{
			m_pGameScene->SaveTetrisRecord();
		}
		break;
		case MENU_UPBTN:
		{
			SET_INTVALUE("UPBUTTON", nValue);			//0旋转 1直落
			updateSingleMenu(pMenuItem, nMenuIdx);
		}
		break;
		case MENU_AUTHOR:
		{
			m_pGameScene->ShowMyApps();
		}
		break;
		case MENU_RATE:
		{
			m_pGameScene->GiveRate();
		}
		break;
		case MENU_BACK:
		{
			m_pGameScene->QuitSetupLayer();
		}
		break;
		default:
			break;
	}
}


int CSetupLayer::getStrIDByMenuIdx(int nMenuIdx)
{
	switch (nMenuIdx)
	{
		case MENU_VIBRATION: 
			return STRNAME_VIBRATION;
			break;
		case MENU_SOUND: 
			return STRNAME_SOUND;
			break;
		case MENU_NIGHTMODE: 
			return STRNAME_NIGHTMODE;
			break;
		case MENU_ORIENTATION: 
			return STRNAME_ORIENTATION;
			break;
		case MENU_RHMODE:
			return STRNAME_RHMODE;
			break;
		case MENU_AUTORECOVER: 
			return STRNAME_AUTORECOVER;
			break;
		case MENU_SAVENOW: 
			return STRNAME_SAVENOW;
			break;
		case MENU_UPBTN: 
			return STRNAME_UPBTN;
			break;
		case MENU_AUTHOR: 
			return STRNAME_AUTHOR;
			break;
		case MENU_RATE: 
			return STRNAME_RATE;
			break;
		case MENU_BACK:
			return STRNAME_BACK;
			break;
		default: 
			return STRNAME_MAX;
			break;
	}
}


void CSetupLayer::initAllMenu()
{
	Size visibleSize = GET_VISIBLESIZE();
	float fCurHeight = visibleSize.height;

	int nLangID = (CCApplication::getInstance()->getCurrentLanguage() != LanguageType::CHINESE);
	bool bNightMode = getValueByMenuIdx(MENU_NIGHTMODE);
	float fMenuHeight = 0.0f;

	for (int nMainMenuIdx = MAINMENU_MIN; nMainMenuIdx < MAINMENU_MAX; ++nMainMenuIdx)
	{
		fCurHeight -= fMenuHeight;

		//主菜单Label
		int nStrID = getStrIDByMainMenuIdx(nMainMenuIdx);
		string strText = CGeneralManager::getInstance()->GetStringByID(nLangID, nStrID);
		Label* pMainTitle = Label::createWithSystemFont(strText, FONT_NAME, TITLE_FONT_SIZE);
		Size mainTitleSize = GET_CONTENTSIZE(pMainTitle);
		pMainTitle->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
		pMainTitle->setPosition(mainTitleSize.width * 0.5f + MENU_X_PADDING, fCurHeight - mainTitleSize.height);
		this->addChild(pMainTitle);
		m_vecLabel.pushBack(pMainTitle);

		fCurHeight -= mainTitleSize.height * 1.5f;
		float fCurWidth = MENU_X_PADDING;
		fMenuHeight = 0;

		for (int nIndex = 0; nIndex < _countof(MENU_RELATION[nMainMenuIdx]); ++nIndex)
		{
			int nMenuIdx = MENU_RELATION[nMainMenuIdx][nIndex];
			if (nMenuIdx < 0)
			{
				continue;
			}

			//根据菜单ID获取字符串
			nStrID = getStrIDByMenuIdx(nMenuIdx);
			strText = CGeneralManager::getInstance()->GetStringByID(nLangID, nStrID);

			//获取记录值
			int nValue = getValueByMenuIdx(nMenuIdx);
			nValue = (nValue != 0) ? 1 : 0;

			//菜单
			const string& strSprName = MENU_SPRITE_STRING[nMenuIdx][nValue];
			MenuItemSprite* pMenuItem = MenuItemSprite::create(
				CREATE_SPRITEWITHNAME(CGeneralManager::getInstance()->GetSpriteName(strSprName, bNightMode)),
				CREATE_SPRITEWITHNAME(CGeneralManager::getInstance()->GetSpriteName(strSprName, bNightMode)),
				CC_CALLBACK_1(CSetupLayer::onClickMenu, this, nMenuIdx)
				);
			Size menuItemSize = GET_CONTENTSIZE(pMenuItem);
			pMenuItem->setPosition(fCurWidth + menuItemSize.width * 0.5f, fCurHeight - menuItemSize.height * 0.5f - MENU_Y_PADDING);

			//图标底部标签
			Label* pTitle = Label::createWithSystemFont(strText, FONT_NAME, NORMAL_FONT_SIZE, Size::ZERO, TextHAlignment::CENTER);
			Size titleSize = GET_CONTENTSIZE(pTitle);
			pTitle->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
			pTitle->setPosition(menuItemSize.width * 0.5f, -MENU_Y_PADDING);
			pTitle->setTag(TITLE_TAG);
			pMenuItem->addChild(pTitle);

			//宽度调整
			if (fCurWidth + menuItemSize.width >= visibleSize.width)
			{
				fCurWidth = MENU_X_PADDING;
				//高度减去一个菜单高度和标签高度
				fCurHeight -= menuItemSize.height + titleSize.height + MENU_Y_PADDING * 2;
				//设置位置
				pMenuItem->setPosition(fCurWidth + menuItemSize.width * 0.5f, fCurHeight - menuItemSize.height * 0.5f - MENU_Y_PADDING);
			}

			//更新菜单总高度
			if (FLOAT_EQ(fMenuHeight, 0))
			{
				fMenuHeight = menuItemSize.height + titleSize.height + MENU_Y_PADDING * 2;
			}
			fCurWidth += menuItemSize.width * 1.2f + MENU_X_PADDING;

			//保存
			m_vecMenuItem.pushBack(pMenuItem);
		}
	}

	//添加到菜单
	Menu* pMenu = Menu::createWithArray(m_vecMenuItem);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);
}


int CSetupLayer::getStrIDByMainMenuIdx(int nMainMenuIdx)
{
	switch (nMainMenuIdx)
	{
	case MAINMENU_GLOBALSET:
		return STRNAME_GLOBALSETTING;
		break;

	case MAINMENU_TETRISSET:
		return STRNAME_TETRISSETTING;
		break;

	case MAINMENU_OTHERSET:
		return STRNAME_OTHER;
		break;

	default:
		return 0;
		break;
	}
}


void CSetupLayer::updateSingleMenu(MenuItem* pMenuItem, int nMenuIdx)
{
	bool bNightMode = getValueByMenuIdx(MENU_NIGHTMODE);
	MenuItemSprite* pMenuItemSpr = static_cast<MenuItemSprite*>(pMenuItem);

	//获取值
	int nValue = getValueByMenuIdx(nMenuIdx);
	nValue = ((nValue != 0) ? 1 : 0);

	//更新菜单
	const string& strSprName = MENU_SPRITE_STRING[nMenuIdx][nValue];
	pMenuItemSpr->setNormalImage(CREATE_SPRITEWITHNAME(CGeneralManager::getInstance()->GetSpriteName(strSprName, bNightMode)));
	pMenuItemSpr->setSelectedImage(CREATE_SPRITEWITHNAME(CGeneralManager::getInstance()->GetSpriteName(strSprName, bNightMode)));

	//获取Label
	Node* pLabelNode = pMenuItem->getChildByTag(TITLE_TAG);
	if (pLabelNode == nullptr)
	{
		return;
	}

	Label* pLabel = static_cast<Label*>(pLabelNode);
	pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
}


int CSetupLayer::getMenuCountByMainMenuIdx(int nMainMenuIdx)
{
	if (nMainMenuIdx < MAINMENU_MIN || nMainMenuIdx >= MAINMENU_MAX)
	{
		return 0;
	}

	int nCount = _countof(MENU_RELATION[nMainMenuIdx]);
	for (int nIndex = 0; nIndex < nCount; ++nIndex)
	{
		if (MENU_RELATION[nMainMenuIdx][nIndex] == -1)
		{
			return nIndex;
		}
	}

	return nCount;
}


void CSetupLayer::adjustMenuPosForPortrait()
{
	Size visibleSize = GET_VISIBLESIZE();
	float fCurHeight = visibleSize.height;
	float fMenuHeight = 0.0f;
	int nMenuIdx = 0;

	for (int nMainMenuIdx = 0; nMainMenuIdx < m_vecLabel.size(); ++nMainMenuIdx)
	{
		fCurHeight -= fMenuHeight;

		//主菜单Label
		Label* pMainTitle = m_vecLabel.at(nMainMenuIdx);
		assert(pMainTitle != nullptr);
		Size mainTitleSize = GET_CONTENTSIZE(pMainTitle);
		pMainTitle->setRotation(0);
		pMainTitle->setPosition(mainTitleSize.width * 0.5f + MENU_X_PADDING, fCurHeight - mainTitleSize.height);

		fCurHeight -= mainTitleSize.height * 1.5f;
		float fCurWidth = MENU_X_PADDING;
		fMenuHeight = 0;

		int nMenuCount = getMenuCountByMainMenuIdx(nMainMenuIdx);
		for (int nCount = 0; nCount < nMenuCount; ++nCount, ++nMenuIdx)
		{
			MenuItem* pMenuItem = m_vecMenuItem.at(nMenuIdx);
			assert(pMenuItem != nullptr);
			Size menuItemSize = GET_CONTENTSIZE(pMenuItem);
			pMenuItem->setPosition(fCurWidth + menuItemSize.width * 0.5f, fCurHeight - menuItemSize.height * 0.5f - MENU_Y_PADDING);
			pMenuItem->setRotation(0);

			Node* pLabelNode = pMenuItem->getChildByTag(TITLE_TAG);
			assert(pLabelNode != nullptr);
			Size labelSize = GET_CONTENTSIZE(pLabelNode);

			//宽度调整
			if (fCurWidth + menuItemSize.width >= visibleSize.width)
			{
				fCurWidth = MENU_X_PADDING;
				//高度减去一个菜单高度和标签高度
				fCurHeight -= menuItemSize.height + labelSize.height + MENU_Y_PADDING * 2;
				//设置位置
				pMenuItem->setPosition(fCurWidth + menuItemSize.width * 0.5f, fCurHeight - menuItemSize.height * 0.5f - MENU_Y_PADDING);
			}

			//更新菜单总高度
			if (FLOAT_EQ(fMenuHeight, 0))
			{
				fMenuHeight = menuItemSize.height + labelSize.height + MENU_Y_PADDING * 2;
			}
			fCurWidth += menuItemSize.width * 1.2f + MENU_X_PADDING;
		}
	}
}


void CSetupLayer::adjustMenuPosForLandscape()
{
	Size visibleSize = GET_VISIBLESIZE();
	float fCurWidth = visibleSize.width;
	float fMenuWidth = 0.0f;
	int nMenuIdx = 0;

	for (int nMainMenuIdx = 0; nMainMenuIdx < m_vecLabel.size(); ++nMainMenuIdx)
	{
		fCurWidth -= fMenuWidth;

		//主菜单Label
		Label* pMainTitle = m_vecLabel.at(nMainMenuIdx);
		assert(pMainTitle != nullptr);
		Size mainTitleSize = GET_CONTENTSIZE(pMainTitle);
		pMainTitle->setRotation(90);
		pMainTitle->setPosition(fCurWidth - mainTitleSize.height * 0.8f, visibleSize.height - mainTitleSize.width * 0.5f - MENU_X_PADDING);

		fCurWidth -= mainTitleSize.height * 1.3f;
		float fCurHeight = visibleSize.height - MENU_X_PADDING;
		fMenuWidth = 0;

		int nMenuCount = getMenuCountByMainMenuIdx(nMainMenuIdx);
		for (int nCount = 0; nCount < nMenuCount; ++nCount, ++nMenuIdx)
		{
			MenuItem* pMenuItem = m_vecMenuItem.at(nMenuIdx);
			assert(pMenuItem != nullptr);
			Size menuItemSize = GET_CONTENTSIZE(pMenuItem);
			pMenuItem->setPosition(fCurWidth - menuItemSize.height * 0.5f - MENU_Y_PADDING, fCurHeight - menuItemSize.width * 0.5f - MENU_X_PADDING * 2);
			pMenuItem->setRotation(90);

			Node* pLabelNode = pMenuItem->getChildByTag(TITLE_TAG);
			Size labelSize = GET_CONTENTSIZE(pLabelNode);
			assert(pLabelNode != nullptr);
			//pLabelNode->setPosition(-MENU_Y_PADDING, menuItemSize.width * 0.5f);

			//宽度调整
			if (fCurHeight - menuItemSize.width <= 0)
			{
				fCurHeight = visibleSize.height - MENU_X_PADDING;
				//高度减去一个菜单高度和标签高度
				fCurWidth -= menuItemSize.height + labelSize.height + MENU_Y_PADDING;
				//设置位置
				pMenuItem->setPosition(fCurWidth - menuItemSize.height * 0.5f - MENU_Y_PADDING, fCurHeight - menuItemSize.width * 0.5f - MENU_X_PADDING);
			}

			//更新菜单总高度
			if (FLOAT_EQ(fMenuWidth, 0))
			{
				fMenuWidth = menuItemSize.height + labelSize.height + MENU_Y_PADDING;
			}
			fCurHeight -= menuItemSize.width * 1.0f + MENU_X_PADDING * 2;
		}
	}
}


void CSetupLayer::adjustMenuPos()
{
	int nValue = getValueByMenuIdx(MENU_ORIENTATION);
	if (nValue == 1)
	{
		adjustMenuPosForPortrait();
	}
	else
	{
		adjustMenuPosForLandscape();
	}
}

