#include "RacingGame.h"

USING_NS_CC;

CRacingGame::CRacingGame() : m_iRowIdx(0), m_iColIdx(0), m_iDirection(DIR_RIGHT), m_iAllCount(0),
m_iEndRowIdx(ROW_NUM - 1), m_iEndColIdx(COLUMN_NUM - 1), m_iBeginColIdx(-1), m_iBeginRowIdx(0)
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
	if (m_iAllCount == ROW_NUM * COLUMN_NUM)
	{
		return;
	}

	++m_iAllCount;
	m_arrCurBrick[m_iRowIdx][m_iColIdx] = 1;
	m_pBrick[m_iRowIdx][m_iColIdx]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("black.png"));

	switch (m_iDirection)
	{
	case DIR_RIGHT:
		++m_iColIdx;
		break;
	case DIR_DOWN:
		++m_iRowIdx;
		break;
	case DIR_LEFT:
		--m_iColIdx;
		break;
	case DIR_UP:
		--m_iRowIdx;
		break;
	}
	//到达右上角
	if (m_iColIdx == m_iEndColIdx && m_iRowIdx == m_iBeginRowIdx)
	{
		m_iDirection = DIR_DOWN;
		++m_iBeginColIdx;
	}
	else if (m_iColIdx == m_iEndColIdx && m_iRowIdx == m_iEndRowIdx)
	{
		m_iDirection = DIR_LEFT;
		++m_iBeginRowIdx;
	}
	else if (m_iColIdx == m_iBeginColIdx && m_iRowIdx == m_iEndRowIdx)
	{
		m_iDirection = DIR_UP;
		--m_iEndColIdx;
	}
	else if (m_iColIdx == m_iBeginColIdx && m_iRowIdx == m_iBeginRowIdx)
	{
		m_iDirection = DIR_RIGHT;
		--m_iEndRowIdx;
	}
}


void CRacingGame::update(float dt)
{
	PlayGameOverAnim();
}