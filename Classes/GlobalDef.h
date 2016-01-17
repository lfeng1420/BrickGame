#ifndef __GLOBAL_DEF_H_
#define __GLOBAL_DEF_H_

// ---- 头文件 ----
#include <map>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;


#define CC_RETURN_FALSE_IF(exp) if (exp){return false;}

#define SAFE_DELETE(p) if (p){delete p; p = NULL;}


// ---- cocos 常用函数宏定义 ----
#define DIRECTOR_INSTANCE Director::getInstance

#define GET_VISIBLESIZE Director::getInstance()->getVisibleSize

#define REPLACE_SCENE Director::getInstance()->replaceScene

#define PAUSE_SCENE Director::getInstance()->pause

#define RESUME_SCENE Director::getInstance()->resume

#define SET_TIMESCALE(s) Director::getInstance()->getScheduler()->setTimeScale(s)

#define ADD_ANIM(anim, name) AnimationCache::getInstance()->addAnimation(anim, name)

#define GET_ANIM(name) AnimationCache::getInstance()->getAnimation(name)

#define GET_SPRITEFRAME(name) SpriteFrameCache::getInstance()->getSpriteFrameByName(name)

#define ADD_SPRITEFRAME(name) SpriteFrameCache::getInstance()->addSpriteFramesWithFile(name)

#define CREATE_SPRITEWITHNAME(name) Sprite::createWithSpriteFrameName(name)

#define CREATE_SPRITE(name) Sprite::create(name)

#define AUDIO_INSTANCE CocosDenshion::SimpleAudioEngine::getInstance

#define PRELOAD_BGMUSIC(name) CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(name)

#define PLAY_BGMUSIC(name) CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(name)

#define LOOP_PLAY_BGMUSIC(name) CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(name, true)

#define STOP_BGMUSIC CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic

#define PAUSE_MUSIC CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic

#define PRELOAD_EFFECT(name) CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(name)

#define PLAY_EFFECT(name) CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(name)

#define GET_CONTENTSIZE(sprite) sprite->getContentSize()

#define USERDEFAULT_INSTANCE UserDefault::getInstance

#define GET_INTVALUE(key) UserDefault::getInstance()->getIntegerForKey(key)

#define SET_INTVALUE(key, value) UserDefault::getInstance()->setIntegerForKey(key, value)

#define GET_BOOLVALUE(key) UserDefault::getInstance()->getBoolForKey(key)

#define SET_BOOLVALUE(key, value) UserDefault::getInstance()->setBoolForKey(key, value)


//常量枚举
enum
{
	//行列
	ROW_NUM = 20,
	COLUMN_NUM = 13,

	//方块宽高
	BRICK_WIDTH = 32,
	BRICK_HEIGHT = 32,


};

//方向枚举
enum
{
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP,
};


//游戏索引枚举
enum SCENE_INDEX
{
	INVALID_SCENE = -1,
	GAME_OVER,		//游戏结束界面
	CHOOSE_GAME,	//选择界面
	GAME_RACING,	//游戏界面 - 赛车

	SCENE_MAX,		//最大值
};

enum GAME_LIST
{
	RACING,			//赛车
};

#endif //__GLOBAL_DEF_H_
