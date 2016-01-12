#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
	if (!LayerColor::initWithColor(Color4B::WHITE))
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float fY = visibleSize.height;
	for (int i = 0; i < ROW_NUM; i++)
    {
		float fX = 0;
		Size size = Size(32, 32);
		for (int j = 0; j < COLUMN_NUM; j++)
		{
			m_pBirck[i][j] = Sprite::create("empty.png");
			m_pBirck[i][j]->setPosition(fX + size.width / 2, fY - size.height / 2);

			this->addChild(m_pBirck[i][j]);
			fX += size.width;
		}
		fY -= size.height;
    }
    

	InitCotroller(fY);

    return true;
}


void HelloWorld::InitCotroller(float fHieght)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

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
	fireBtn->setPosition(visibleSize.width - (leftBtnSize.width * 1.5f + 6), downBtnSize.height * 2.0f + 6);

	selectBtn->setPosition(visibleSize.width / 2, selectBtnSize.height / 2);
	startBtn->setPosition(visibleSize.width / 2 - 4 - selectBtnSize.width / 2 - startBtnSize.width / 2, selectBtnSize.height / 2);
	resetBtn->setPosition(visibleSize.width / 2 + 4 + selectBtnSize.width / 2 + resetBtnSize.width / 2, selectBtnSize.height / 2);

	auto menu = Menu::create(leftBtn, upBtn, rightBtn, downBtn, fireBtn, selectBtn, startBtn, resetBtn, NULL);
	float fCurHeight = downBtnSize.height * 2.0f + 12 + upBtnSize.height;
	menu->setPosition(Vec2(0, (fHieght - fCurHeight) / 3));
	this->addChild(menu);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
