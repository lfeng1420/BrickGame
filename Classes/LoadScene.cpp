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
	ADD_SPRITEFRAME("Plists/Images.plist");
	//ADD_SPRITEFRAME("Plists/Tips.plist");

	Size visibleSize = GET_VISIBLESIZE();

	bool arrBrick[][COLUMN_NUM] = 
	{
		true, true, true, true, true, false, false, false, true, true, true, true, false, false,
		false, true, false, false, false, true, false, true, false, false, false, false, false, false,
		false, true, true, true, true, false, false, true, false, false, false, true, true, false,
		false, true, false, false, false, true, false, true, false, false, false, false, true, false,
		true, true, true, true, true, false, false, false, true, true, true, true, false, false,
	};

	bool bPortraitFlag = GET_BOOLVALUE("PORTRAIT", true);
	if (bPortraitFlag)
	{
		float fStartY = (visibleSize.height - 5 * BRICK_HEIGHT) / 2;
		float fStartX = (visibleSize.width - COLUMN_NUM * BRICK_WIDTH) / 2;
		for (int i = 4; i >= 0; --i)
		{
			float fCurX = fStartX;
			for (int j = 0; j < COLUMN_NUM; ++j)
			{
				if (arrBrick[i][j])
				{
					auto sprite = CREATE_SPRITEWITHNAME("black.png");
					sprite->setPosition(fCurX + BRICK_WIDTH / 2, fStartY + BRICK_HEIGHT / 2);
					this->addChild(sprite);
				}

				fCurX += BRICK_WIDTH;
			}

			fStartY += BRICK_HEIGHT;
		}
	}
	else
	{
		float fStartY = (visibleSize.height - COLUMN_NUM * BRICK_HEIGHT) / 2;
		float fCurX = (visibleSize.width - 5 * BRICK_WIDTH) / 2;
		for (int i = 4; i >= 0; --i)
		{
			float fCurY = fStartY;
			for (int j = COLUMN_NUM - 1; j >= 0; --j)
			{
				if (arrBrick[i][j])
				{
					auto sprite = CREATE_SPRITEWITHNAME("black.png");
					sprite->setPosition(fCurX + BRICK_WIDTH / 2, fCurY + BRICK_HEIGHT / 2);
					this->addChild(sprite);
				}

				fCurY += BRICK_HEIGHT;
			}

			fCurX += BRICK_WIDTH;
		}
	}

	scheduleOnce(schedule_selector(CLoadScene::ToGameScene), 2.0f);

	return true;
}


void CLoadScene::ToGameScene(float dt)
{
	CGeneralManager::getInstance();

	Director::getInstance()->replaceScene(CGameScene::CreateScene());
}
