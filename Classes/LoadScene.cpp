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

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Plists/Brick.plist");

	scheduleOnce(schedule_selector(CLoadScene::ToGameScene), 2.0f);

	return true;
}


void CLoadScene::ToGameScene(float dt)
{
	CGeneralManager::getInstance();

	Director::getInstance()->replaceScene(CGameScene::CreateScene());
}
