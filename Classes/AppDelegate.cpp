#include "stdafx.h"
#include "AppDelegate.h"
#include "LoadScene.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
		glview->setFrameSize(480, 852);
        director->setOpenGLView(glview);
    }

	Size oFrameSize = glview->getFrameSize();
	glview->setDesignResolutionSize(640, 1136, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = CLoadScene::CreateScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() 
{
	Scene* pRunningScene = Director::getInstance()->getRunningScene();
	if (pRunningScene == nullptr)
	{
		return;
	}

	if (pRunningScene->getTag() != GAME_SCENE_TAG)
	{
		return;
	}

	CGameScene* pGameScene = (CGameScene*)pRunningScene;
	pGameScene->unscheduleUpdate();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() 
{
	Scene* pRunningScene = Director::getInstance()->getRunningScene();
	if (pRunningScene == nullptr)
	{
		return;
	}

	if (pRunningScene->getTag() != GAME_SCENE_TAG)
	{
		return;
	}

	CGameScene* pGameScene = (CGameScene*)pRunningScene;
	pGameScene->scheduleUpdate();
}
