#include "LoadScene.h"
#include "GeneralManager.h"
#include "GameScene.h"

USING_NS_CC;

CLoadScene::CLoadScene()
{
}


CLoadScene::~CLoadScene()
{
}


Scene* CLoadScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = CLoadScene::create();
	scene->addChild(layer);

	return scene;
}

bool CLoadScene::init()
{
	if (!LayerColor::initWithColor(Color4B::WHITE))
	{
		return false;
	}

	ADD_SPRITEFRAME("Plists/Brick.plist");
	ADD_SPRITEFRAME("Plists/Font.plist");
	ADD_SPRITEFRAME("Plists/BGPic.plist");
	ADD_SPRITEFRAME("Plists/Tips.plist");

	Size visibleSize = GET_VISIBLESIZE();

	bool arrBrick[][COLUMN_NUM] = 
	{
		true, true, true, true, true, false, false, false, true, true, true, true, false, false,
		false, true, false, false, false, true, false, true, false, false, false, false, false, false,
		false, true, true, true, true, false, false, true, false, false, false, true, true, false,
		false, true, false, false, false, true, false, true, false, false, false, false, true, false,
		true, true, true, true, true, false, false, false, true, true, true, true, false, false,
	};

	auto pNode = Node::create();
	float fCurY = 5 * BRICK_HEIGHT;
	for (int i = 0; i < 5; ++i)
	{
		float fCurX = 0;
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			if (arrBrick[i][j])
			{
				auto sprite = CREATE_SPRITEWITHNAME("black.png");
				sprite->setPosition(fCurX + BRICK_WIDTH / 2, fCurY - BRICK_HEIGHT / 2);
				pNode->addChild(sprite);
			}

			fCurX += BRICK_WIDTH;
		}

		fCurY -= BRICK_HEIGHT;
	}

	pNode->setPosition((visibleSize.width - COLUMN_NUM * BRICK_WIDTH) / 2 + BRICK_WIDTH / 2, 
		(visibleSize.height - 5 * BRICK_HEIGHT) / 2 + BRICK_HEIGHT / 2);
	this->addChild(pNode);

	scheduleOnce(schedule_selector(CLoadScene::ToGameScene), 2.0f);

	return true;
}


void CLoadScene::ToGameScene(float dt)
{
	CGeneralManager::getInstance();

	Director::getInstance()->replaceScene(CGameScene::CreateScene());
}
