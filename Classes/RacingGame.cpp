#include "RacingGame.h"

USING_NS_CC;

CRacingGame::CRacingGame() : m_iRowIndex(0), m_iColIndex(0), m_iDirection(DIR_RIGHT), m_iTime(0)
{
}


CRacingGame::~CRacingGame()
{
}


Scene* CRacingGame::CreateScene()
{
	auto scene = Scene::create();
	auto layer = CRacingGame::create();
	scene->addChild(layer);
	return scene;
}


bool CRacingGame::init()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Brick.plist");

	CC_RETURN_FALSE_IF(!LayerColor::initWithColor(Color4B::WHITE));
	
	//初始化变量，获取数据等
	InitData();

	InitBrick();

	InitCotroller();

	this->scheduleUpdate();

	return true;
}


void CRacingGame::InitData()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
}


//初始化Brick
void CRacingGame::InitBrick()
{
	float fCurY = m_visibleSize.height;
	for (int i = 0; i < ROW_NUM; ++i)
	{
		float fCurX = 0;
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrCurBrick[i][j] = 0;

			m_pBrick[i][j] = Sprite::create("empty.png");
			m_pBrick[i][j]->setPosition(fCurX + BRICK_WIDTH * 1.0f / 2, fCurY - BRICK_HEIGHT * 1.0f / 2);
			this->addChild(m_pBrick[i][j]);

			fCurX += BRICK_WIDTH;
		}

		fCurY -= BRICK_HEIGHT;
	}
}


void CRacingGame::InitCotroller()
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


//播放游戏结束动画
void CRacingGame::PlayGameOverAnim()
{
	m_arrCurBrick[m_iRowIndex][m_iColIndex] = 1;
	m_pBrick[m_iRowIndex][m_iColIndex]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("black.png"));

	switch (m_iDirection)
	{
	case DIR_RIGHT:
		++m_iColIndex;
		if (m_iColIndex >= COLUMN_NUM)
		{
			m_iColIndex = COLUMN_NUM - 1;
			m_iDirection = DIR_DOWN;
			return;
		}

		if (m_arrCurBrick[m_iRowIndex][m_iColIndex] == 1)
		{
			--m_iColIndex;
			m_iDirection = DIR_DOWN;
		}

		break;
	case DIR_DOWN:
		++m_iRowIndex;
		if (m_iRowIndex >= ROW_NUM)
		{
			m_iRowIndex = ROW_NUM - 1;
			m_iDirection = DIR_LEFT;
			return;
		}

		if (m_arrCurBrick[m_iRowIndex][m_iColIndex] == 1)
		{
			--m_iRowIndex;
			m_iDirection = DIR_LEFT;
		}

		break;
	case DIR_LEFT:
		--m_iColIndex;
		if (m_iColIndex < 0)
		{
			m_iColIndex = 0;
			m_iDirection = DIR_UP;
			return;
		}

		if (m_arrCurBrick[m_iRowIndex][m_iColIndex] == 1)
		{
			++m_iColIndex;
			m_iDirection = DIR_UP;
		}

		break;
	case DIR_UP:
		--m_iRowIndex;
		if (m_iRowIndex < 0)
		{
			m_iRowIndex = 0;
			m_iDirection = DIR_RIGHT;
			return;
		}

		if (m_arrCurBrick[m_iRowIndex][m_iColIndex] == 1)
		{
			++m_iRowIndex;
			m_iDirection = DIR_RIGHT;
		}
		break;
	}

}


void CRacingGame::update(float dt)
{
	m_iTime += dt;
	if (m_iTime > 0.1f)
	{
		m_iTime = 0;
		PlayGameOverAnim();
	}
}