#include "SetupLayer.h"
#include "GeneralManager.h"
#include "GameScene.h"

USING_NS_CC;

//菜单选项对应字符串ID
const int MENU_ITEM_STRING[MENU_MAX][2] =
{
	{ STRNAME_MAX },							//MENU_GLOBALSET
	{ STRNAME_CHINESE, STRNAME_ENGLISH },		//MENU_LANGUAGE
	{ STRNAME_OFF, STRNAME_ON },				//MENU_SOUND
	{ STRNAME_OFF, STRNAME_ON },				//MENU_NIGHTMODE
	{ STRNAME_PORTRAIT, STRNAME_LANDSCAPE },	//MENU_ORIENTATION
	{ STRNAME_MAX },							//MENU_TETRISSET
	{ STRNAME_OFF, STRNAME_ON },				//MENU_AUTORECOVER
	{ STRNAME_RIGHTARROW },						//MENU_SAVENOW
	{ STRNAME_ROTATE, STRNAME_QUICKLAND },		//MENU_UPBTN
	{ STRNAME_MAX },							//MENU_OTHERSET
	{ STRNAME_LFENG },							//MENU_AUTHOR
	{ STRNAME_RIGHTARROW },						//MENU_RATE
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

	//获取配置
	bool bNightMode = GET_BOOLVALUE("NIGHTMODE", false);
	Color4B stColor = Color4B::WHITE;
	if (bNightMode)
	{
		stColor = Color4B::BLACK;
	}

	//创建菜单和标签
	CreateAllMenuAndLabel();

	//设置位置
	AdjustMenuLabelPos();

	return true;
}


void CSetupLayer::CreateAllMenuAndLabel()
{
	int nLangID = GetValueByMenuIdx(MENU_LANGUAGE);
	bool bNightMode = GetValueByMenuIdx(MENU_NIGHTMODE);

	for (int nIndex = MENU_MIN; nIndex < MENU_MAX; ++nIndex)
	{
		int nFontSize = NORMAL_FONT_SIZE;
		if (CheckMenuTitleFlag(nIndex))
		{
			nFontSize = TITLE_FONT_SIZE;
		}

		//菜单
		int nStrID = GetStrIDByMenuIdx(nIndex);
		string strText = CGeneralManager::getInstance()->GetStringByID(nLangID, nStrID);
		Label* pMenuLabel = Label::createWithSystemFont(strText, FONT_NAME, nFontSize, Size::ZERO, TextHAlignment::RIGHT);
		pMenuLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
		MenuItem* pMenuItem = MenuItemLabel::create(pMenuLabel, CC_CALLBACK_1(CSetupLayer::OnClickMenu, this, nIndex), false);
		m_vecMenuItem.pushBack(pMenuItem);


		//状态
		int nCurChoiceIdx = GetValueByMenuIdx(nIndex);
		int nChoiceCount = _countof(MENU_ITEM_STRING[nIndex]);
		assert(nCurChoiceIdx < nChoiceCount);
		nStrID = MENU_ITEM_STRING[nIndex][nCurChoiceIdx];
		strText = CGeneralManager::getInstance()->GetStringByID(nLangID, nStrID);
		Label* pLabel = Label::createWithSystemFont(strText, FONT_NAME, nFontSize, Size::ZERO, TextHAlignment::LEFT);
		pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
		this->addChild(pLabel);
		m_vecLabel.pushBack(pLabel);
	}

	//添加到菜单
	Menu* pMenu = Menu::createWithArray(m_vecMenuItem);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);
}


void CSetupLayer::UpdateSingleMenuAndLabel(int nMenuIdx)
{
	int nLangID = GetValueByMenuIdx(MENU_LANGUAGE);
	bool bNightMode = GetValueByMenuIdx(MENU_NIGHTMODE);

	//菜单
	int nStrID = GetStrIDByMenuIdx(nMenuIdx);
	string strText = CGeneralManager::getInstance()->GetStringByID(nLangID, nStrID);
	MenuItem* pMenuItem = m_vecMenuItem.at(nMenuIdx);
	assert(pMenuItem != nullptr);
	MenuItemLabel* pMenuItemLabel = static_cast<MenuItemLabel*>(pMenuItem);
	Label* pLabel = static_cast<Label*>(pMenuItemLabel->getLabel());
	pLabel->setString(strText);
	pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);

	//获取状态标签
	pLabel = m_vecLabel.at(nMenuIdx);
	assert(pLabel != nullptr);

	//获取当前字符串
	int nCurChoiceIdx = GetValueByMenuIdx(nMenuIdx);
	int nChoiceCount = _countof(MENU_ITEM_STRING[nMenuIdx]);
	assert(nCurChoiceIdx < nChoiceCount);
	nStrID = MENU_ITEM_STRING[nMenuIdx][nCurChoiceIdx];
	strText = CGeneralManager::getInstance()->GetStringByID(nLangID, nStrID);
	//记录原始大小，后续需调整位置
	Size originSize = GET_CONTENTSIZE(pLabel);
	pLabel->setString(strText);
	//新的大小
	Size nowSize = GET_CONTENTSIZE(pLabel);
	float fWidthDistance = nowSize.width - originSize.width;
	Vec2 pos = pLabel->getPosition();
	////根据横竖屏调整位置
	//bool bPortraitFlag = GetValueByMenuIdx(MENU_ORIENTATION);
	//if (bPortraitFlag)
	//{
	//	pLabel->setPositionX(pos.x + fWidthDistance);
	//}
	//else
	//{
	//	pLabel->setPositionY(pos.y + fWidthDistance);
	//}
	pLabel->setColor(bNightMode ? Color3B::WHITE : Color3B::BLACK);
}


void CSetupLayer::UpdateAllMenuAndLabel()
{
	for (int nIndex = MENU_MIN; nIndex < MENU_MAX; ++nIndex)
	{
		UpdateSingleMenuAndLabel(nIndex);
	}
}


void CSetupLayer::AdjustMenuLabelPos()
{
	Size visibleSize = GET_VISIBLESIZE();
	Vec2 origin = GET_VISIBLEORIGIN();
	bool bPortraitFlag = GetValueByMenuIdx(MENU_ORIENTATION);
	if (bPortraitFlag)
	{
		//菜单和标签
		float fStartY = visibleSize.height;
		for (int nIndex = 0; nIndex < m_vecMenuItem.size(); ++nIndex)
		{
			MenuItem* pMenuItem = m_vecMenuItem.at(nIndex);
			if (pMenuItem == nullptr)
			{
				continue;
			}

			Size menuItemSize = GET_CONTENTSIZE(pMenuItem);
			pMenuItem->setAnchorPoint(Vec2(0, 1.0f));
			pMenuItem->setRotation(0);
			pMenuItem->setPosition(MENU_X_PADDING, fStartY - MENU_Y_PADDING);
			pMenuItem->setContentSize(Size(visibleSize.width, menuItemSize.height));

			//状态标签
			Label* pLabel = m_vecLabel.at(nIndex);
			assert(pLabel != nullptr);
			Size labelSize = GET_CONTENTSIZE(pLabel);
			pLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
			pLabel->setPosition(visibleSize.width - MENU_X_PADDING, fStartY - MENU_Y_PADDING);
			pLabel->setRotation(0);

			fStartY -= menuItemSize.height * PORTRAIT_MENU_SCALE;
		}
	}
	else
	{
		//菜单和标签
		float fStartX = visibleSize.width + origin.x;
		for (int nIndex = 0; nIndex < m_vecMenuItem.size(); ++nIndex)
		{
			MenuItem* pMenuItem = m_vecMenuItem.at(nIndex);
			if (pMenuItem == nullptr)
			{
				continue;
			}

			Size menuItemSize = GET_CONTENTSIZE(pMenuItem);
			pMenuItem->setRotation(90);
			pMenuItem->setAnchorPoint(Vec2(0, 1.0f));
			pMenuItem->setPosition(fStartX - MENU_X_PADDING, visibleSize.height - MENU_Y_PADDING);
			pMenuItem->setContentSize(Size(visibleSize.height, menuItemSize.height));

			//状态标签
			Label* pLabel = m_vecLabel.at(nIndex);
			assert(pLabel != nullptr);
			Size labelSize = GET_CONTENTSIZE(pLabel);
			pLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
			pLabel->setPosition(fStartX - MENU_X_PADDING, MENU_Y_PADDING);
			pLabel->setRotation(90);

			fStartX -= menuItemSize.height * LANDSCAPE_MENU_SCALE + MENU_X_PADDING;
		}
	}
}


int CSetupLayer::GetValueByMenuIdx(int nMenuIdx)
{
	switch (nMenuIdx)
	{
	case MENU_LANGUAGE:
		return GET_INTVALUE("LANGUAGE", LANG_CH);	//0中文 1英语
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
	case MENU_AUTORECOVER:
		return GET_BOOLVALUE("TETRIS_RECORD_VALID", false);	//0关闭 1开启
		break;
	case MENU_SAVENOW:
		return 0;
		break;
	case MENU_UPBTN:
		return GET_INTVALUE("UPBUTTON", 0);			//0旋转 1直落
		break;
	case MENU_AUTHOR:
	case MENU_RATE:
	default:
		return 0;
		break;
	}
}


void CSetupLayer::OnClickMenu(Ref* pSender, int nMenuIdx)
{
	log("%s", __FUNCTION__);
	int nValue = GetValueByMenuIdx(nMenuIdx);
	int nChoiceCount = _countof(MENU_ITEM_STRING[nMenuIdx]);
	if (nChoiceCount > 1)
	{
		nValue = ((nValue == 1) ? 0 : 1);
	}
	bool bState = ((nValue == 1) ? true : false);

	//各个菜单其他处理
	switch (nMenuIdx)
	{
		case MENU_LANGUAGE:
		{
			SET_INTVALUE("LANGUAGE", nValue);			//0中文 1英语
			UpdateAllMenuAndLabel();
			AdjustMenuLabelPos();
		}
		break;
		case MENU_SOUND:
		{
			m_pGameScene->OnButtonClick(nullptr, CGameScene::BTN_SOUND);
		}
		break;
		case MENU_NIGHTMODE:
		{
			m_pGameScene->ChangeColorMode();
			UpdateAllMenuAndLabel();
		}
		break;
		case MENU_ORIENTATION:
		{
			SET_BOOLVALUE("PORTRAIT", bState);				//0横屏 1竖屏
			UpdateAllMenuAndLabel();
			AdjustMenuLabelPos();
		}
		break;
		case MENU_AUTORECOVER:
		{
			SET_BOOLVALUE("TETRIS_RECORD_VALID", bState);	//0关闭 1开启
			m_pGameScene->ShowTips(bState ? CGameScene::TIPS_SAVEOPEN : CGameScene::TIPS_SAVECLOSE);
			UpdateSingleMenuAndLabel(nMenuIdx);
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
			UpdateSingleMenuAndLabel(nMenuIdx);
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
		case MENU_GLOBALSET:
		case MENU_TETRISSET:
		case MENU_OTHERSET:
		{
			m_pGameScene->QuitSetupLayer();
		}
		break;
		default:
			break;
	}
}


int CSetupLayer::GetStrIDByMenuIdx(int nMenuIdx)
{
	switch (nMenuIdx)
	{
		case MENU_GLOBALSET:
			return STRNAME_GLOBALSETTING;
			break;
		case MENU_LANGUAGE: 
			return STRNAME_LANGUAGE;
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
		case MENU_TETRISSET: 
			return STRNAME_TETRISSETTING;
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
		case MENU_OTHERSET: 
			return STRNAME_OTHER;
			break;
		case MENU_AUTHOR: 
			return STRNAME_AUTHOR;
			break;
		case MENU_RATE: 
			return STRNAME_RATE;
			break;
		default: 
			return STRNAME_MAX;
			break;
	}
}


bool CSetupLayer::CheckMenuTitleFlag(int nMenuIdx)
{
	if (nMenuIdx == MENU_GLOBALSET || nMenuIdx == MENU_TETRISSET || nMenuIdx == MENU_OTHERSET)
	{
		return true;
	}

	return false;
}
