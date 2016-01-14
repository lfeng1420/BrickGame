#include "GameScene.h"
#include "GameOver.h"

USING_NS_CC;

CGameScene::CGameScene() : m_iGameIndex(GAME_OVER)
{
}


CGameScene::~CGameScene()
{
	for (int i = 0; i < m_mapGameObj.size(); ++i)
	{
		log("Delete. m_mapGameObj[%d] : %d", i, m_mapGameObj[i]);
		SAFE_DELETE(m_mapGameObj[i]);
		log("Delete Done. m_mapGameObj[%d] : %d", i, m_mapGameObj[i]);
	}
}


Scene* CGameScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = CGameScene::create();
	scene->addChild(layer);
	return scene;
}


bool CGameScene::init()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Brick.plist");

	CC_RETURN_FALSE_IF(!LayerColor::initWithColor(Color4B::WHITE));
	
	//初始化变量，获取数据等
	InitData();

	InitBrick();

	InitCotroller();

	CreateGameObj();

	this->scheduleUpdate();

	return true;
}


void CGameScene::InitData()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
}


//初始化Brick
void CGameScene::InitBrick()
{
	float fCurY = m_visibleSize.height;
	for (int i = 0; i < ROW_NUM; ++i)
	{
		float fCurX = 0;
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_pBrick[i][j] = Sprite::create("empty.png");
			m_pBrick[i][j]->setPosition(fCurX + BRICK_WIDTH * 1.0f / 2, fCurY - BRICK_HEIGHT * 1.0f / 2);
			this->addChild(m_pBrick[i][j]);

			fCurX += BRICK_WIDTH;
		}

		fCurY -= BRICK_HEIGHT;
	}
}


void CGameScene::InitCotroller()
{
	//剩余高度，用于调整控制按钮位置
	float fHeight = m_visibleSize.height - BRICK_HEIGHT * ROW_NUM;

	auto upBtn = MenuItemSprite::create(
		Sprite::create("lineDark01.png"),
		Sprite::create("flatDark01.png"),
		NULL
		);
	Size upBtnSize = upBtn->getContentSize();

	auto rightBtn = MenuItemSprite::create(
		Sprite::create("lineDark02.png"),
		Sprite::create("flatDark02.png"),
		NULL
		);
	Size rightBtnSize = rightBtn->getContentSize();

	auto downBtn = MenuItemSprite::create(
		Sprite::create("lineDark03.png"),
		Sprite::create("flatDark03.png"),
		NULL
		);
	Size downBtnSize = downBtn->getContentSize();

	auto leftBtn = MenuItemSprite::create(
		Sprite::create("lineDark04.png"),
		Sprite::create("flatDark04.png"),
		NULL
		);
	Size leftBtnSize = leftBtn->getContentSize();

	auto fireBtn = MenuItemSprite::create(
		Sprite::create("lineDark05.png"),
		Sprite::create("flatDark05.png"),
		NULL
		);
	Size fireBtnSize = fireBtn->getContentSize();

	auto startBtn = MenuItemSprite::create(
		Sprite::create("lineDark06.png"),
		Sprite::create("flatDark06.png"),
		NULL
		);
	startBtn->setScale(0.8f);
	Size startBtnSize = startBtn->getContentSize();

	auto selectBtn = MenuItemSprite::create(
		Sprite::create("lineDark07.png"),
		Sprite::create("flatDark07.png"),
		NULL
		);
	selectBtn->setScale(0.8f);
	Size selectBtnSize = selectBtn->getContentSize();

	auto resetBtn = MenuItemSprite::create(
		Sprite::create("lineDark08.png"),
		Sprite::create("flatDark08.png"),
		NULL
		);
	resetBtn->setScale(0.8f);
	Size resetBtnSize = resetBtn->getContentSize();

	leftBtn->setPosition(leftBtnSize.width, downBtnSize.height * 2.0f + 6);
	rightBtn->setPosition(leftBtnSize.width * 1.5f + rightBtnSize.width / 2 + 12, downBtnSize.height * 2.0f + 6);
	downBtn->setPosition(leftBtnSize.width * 1.5f + 6, downBtnSize.height * 1.5f);
	upBtn->setPosition(leftBtnSize.width * 1.5f + 6, downBtnSize.height * 2.0f + 12 + upBtnSize.height / 2);
	fireBtn->setPosition(m_visibleSize.width - (leftBtnSize.width * 1.5f + 6), downBtnSize.height * 2.0f + 6);

	selectBtn->setPosition(m_visibleSize.width / 2, selectBtnSize.height / 2);
	startBtn->setPosition(m_visibleSize.width / 2 - 4 - selectBtnSize.width / 2 - startBtnSize.width / 2, selectBtnSize.height / 2);
	resetBtn->setPosition(m_visibleSize.width / 2 + 4 + selectBtnSize.width / 2 + resetBtnSize.width / 2, selectBtnSize.height / 2);

	auto menu = Menu::create(leftBtn, upBtn, rightBtn, downBtn, fireBtn, selectBtn, startBtn, resetBtn, NULL);
	float fCurHeight = downBtnSize.height * 2.0f + 12 + upBtnSize.height;
	menu->setPosition(Vec2(0, (fHeight - fCurHeight) / 3));
	this->addChild(menu);
}


void CGameScene::update(float dt)
{
	GAME_INDEX enGameIdx = m_mapGameObj[m_iGameIndex]->GetGameType();
	bool bRet = m_mapGameObj[m_iGameIndex]->Play();
	if (bRet)
	{
		int iRowIndex = 0;
		int iColIndex = 0;
		m_mapGameObj[m_iGameIndex]->GetCurPos(iRowIndex, iColIndex);
		UpdateBrick(iRowIndex, iColIndex, true);
	}
}


//创建各个游戏对象
void CGameScene::CreateGameObj()
{
	CGameOver* pGameOver = new CGameOver();
	m_mapGameObj[0] = pGameOver;
}


//更新单个Brick状态
void CGameScene::UpdateBrick(int iRowIndex, int iColIndex, bool bShowFlag)
{
	if (bShowFlag)
	{
		m_pBrick[iRowIndex][iColIndex]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("black.png"));
	}
	else
	{
		m_pBrick[iRowIndex][iColIndex]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("empty.png"));
	}
}
