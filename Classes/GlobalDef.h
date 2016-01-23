#ifndef __GLOBAL_DEF_H_
#define __GLOBAL_DEF_H_

// ---- 头文件 ----
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../cocos/ui/CocosGUI.h"

#include <map>
#include <vector>
#include <algorithm>

using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

#define FLOAT_EQ(a, b) (fabs(a - b) < 1e-6)

#define CC_RETURN_FALSE_IF(exp) if (exp){return false;}

#define SAFE_DELETE(p) do{ delete p; p = nullptr; } while(0)


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

#define PLAY_BGMUSIC(name) CDataManager::getInstance()->PlayMusic(name)

#define LOOP_PLAY_BGMUSIC(name) CDataManager::getInstance()->PlayMusic(name, true)

#define STOP_BGMUSIC CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic

#define PAUSE_MUSIC CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic

#define PRELOAD_EFFECT(name) CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(name)

#define PLAY_EFFECT(name) CDataManager::getInstance()->PlayEffect(name)

#define SET_SOUNDSTATE(state) CDataManager::getInstance()->SetSoundState(state)

#define GET_SOUNDSTATE CDataManager::getInstance()->GetSoundState

#define GET_CONTENTSIZE(sprite) sprite->getContentSize()

#define USERDEFAULT_INSTANCE UserDefault::getInstance

#define GET_INTVALUE(key, value) UserDefault::getInstance()->getIntegerForKey(key, value)

#define SET_INTVALUE(key, value) UserDefault::getInstance()->setIntegerForKey(key, value)

#define GET_BOOLVALUE(key, value) UserDefault::getInstance()->getBoolForKey(key, value)

#define SET_BOOLVALUE(key, value) UserDefault::getInstance()->setBoolForKey(key, value)


//常量枚举
enum
{
	//行列
	ROW_NUM = 20,
	COLUMN_NUM = 14,

	//方块宽高
	BRICK_WIDTH = 32,
	BRICK_HEIGHT = 32,

	//速度和等级最大值
	SPEED_MAX = 10,
	LEVEL_MAX = 10,
};

//方向枚举
enum DIRECTION
{
	DIR_RIGHT,
	DIR_MIN = DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP,
	DIR_MAX,
};


//游戏索引枚举
enum SCENE_INDEX
{
	SCENE_INVALID = -1,

	SCENE_GAMEOVER,				//游戏结束界面

	SCENE_CHOOSEGAME,			//选择界面

	SCENE_RACING,				//游戏界面 - 赛车

	SCENE_FROGGER,				//游戏界面 - 青蛙过河

	SCENE_TANK,					//游戏界面 - 坦克大战

	SCENE_MAX,					//最大值
};

enum GAME_LIST
{
	GAME_INVALID = -1,			//无效

	GAME_RACING,				//赛车

	GAME_FROGGER,				//青蛙过河

	GAME_TANK,					//坦克大战

	GAME_MAX,					//最大值
};

enum GAME_STATE
{
	GAMESTATE_RUNNING,				//进行中

	GAMESTATE_OVER,					//结束

	GAMESTATE_PASS,					//通过
};


// ---- 通用函数 ---- 
int Random(int iStart, int iEnd);


#endif //__GLOBAL_DEF_H_
