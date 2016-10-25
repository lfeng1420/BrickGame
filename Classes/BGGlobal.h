#pragma once

// ---- 头文件 ----
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../cocos/ui/CocosGUI.h"
#include "json/rapidjson.h"
#include "json/writer.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"
#include "../cocos/audio/include/SimpleAudioEngine.h"

#include <map>
#include <vector>
#include <algorithm>
#include <time.h>
#include <fstream>

using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;

using namespace ui;

#define FLOAT_EQ(a, b) (fabs(a - b) < 1e-6)

#define CC_RETURN_FALSE_IF(exp) if (exp){return false;}

#define FOR_EACH_CONTAINER(type, container, it) for (type::iterator it = container.begin(); it != container.end(); ++it)


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

#define PLAY_BGMUSIC(name, loop) CGeneralManager::getInstance()->PlayMusic(name, loop)

#define LOOP_PLAY_BGMUSIC(name) CGeneralManager::getInstance()->PlayMusic(name, true)

#define STOP_BGMUSIC CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic

#define PAUSE_BGMUSIC CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic

#define PRELOAD_EFFECT(name) CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(name)

#define PLAY_EFFECT(name) CGeneralManager::getInstance()->PlayEffect(name)

#define SET_SOUNDSTATE(state) CGeneralManager::getInstance()->SetSoundState(state)

#define GET_SOUNDSTATE CGeneralManager::getInstance()->GetSoundState

#define GET_CONTENTSIZE(sprite) sprite->getContentSize()

#define USERDEFAULT_INSTANCE UserDefault::getInstance

#define GET_INTVALUE(key, value) UserDefault::getInstance()->getIntegerForKey(key, value)

#define SET_INTVALUE(key, value) UserDefault::getInstance()->setIntegerForKey(key, value)

#define GET_BOOLVALUE(key, value) UserDefault::getInstance()->getBoolForKey(key, value)

#define SET_BOOLVALUE(key, value) UserDefault::getInstance()->setBoolForKey(key, value)


//通用结构体
struct POSITION
{
	int m_iRowIdx;			//行位置

	int m_iColIdx;			//列位置

	POSITION() : m_iRowIdx(0), m_iColIdx(0)
	{

	};

	POSITION(int iRowIdx, int iColIdx) : m_iRowIdx(iRowIdx), m_iColIdx(iColIdx)
	{

	};

	bool operator== (const POSITION& rhs) const
	{
		return this->m_iColIdx == rhs.m_iColIdx && this->m_iRowIdx == rhs.m_iRowIdx;
	}

	POSITION& operator+= (const POSITION& rhs)
	{
		this->m_iColIdx += rhs.m_iColIdx;
		this->m_iRowIdx += rhs.m_iRowIdx;

		return *this;
	}

	const POSITION operator+ (const POSITION& rhs) const
	{
		POSITION temp(*this);
		temp += rhs;
		return temp;
	}


	POSITION& operator-= (const POSITION& rhs)
	{
		this->m_iColIdx -= rhs.m_iColIdx;
		this->m_iRowIdx -= rhs.m_iRowIdx;

		return *this;
	}

	const POSITION operator- (const POSITION& rhs) const
	{
		POSITION temp(*this);
		temp -= rhs;
		return temp;
	}
};


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
	DIR_MIN = 0,
	DIR_RIGHT = DIR_MIN,
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

	SCENE_TANK,					//游戏界面 - 坦克大战

	SCENE_RACING,				//游戏界面 - 赛车

	SCENE_SNAKE,				//游戏界面 - 贪吃蛇

	SCENE_MATCH,				//游戏界面 - 匹配

	SCENE_FROGGER,				//游戏界面 - 青蛙过河

	SCENE_PINBALL,				//游戏界面 - 弹球

	SCENE_TETRIS,				//游戏界面 - 俄罗斯方块

	SCENE_FLAPPYBIRD,			//游戏界面 - flappybird

	SCENE_TETRIS2,				//游戏界面 - 俄罗斯方块2

	SCENE_PINBALL2,				//游戏界面 - 弹球2

	SCENE_HITBRICK,				//游戏界面 - 打砖块

	SCENE_ADDBRICK,				//游戏界面 - 加砖块

	SCENE_MAX,					//最大值
};

enum GAME_LIST
{
	GAME_INVALID = -1,			//无效

	GAME_TANK,					//坦克大战
	
	GAME_RACING,				//赛车

	GAME_SNAKE,					//贪吃蛇

	GAME_MATCH,					//匹配

	GAME_FROGGER,				//青蛙过河

	GAME_PINBALL,				//弹球

	GAME_TETRIS,				//俄罗斯方块

	GAME_FLAPPYBIRD,			//FlappyBird

	GAME_TETRIS2,				//俄罗斯方块2

	GAME_PINBALL2,				//弹球2

	GAME_HITBRICK,				//打砖块

	GAME_ADDBRICK,				//加砖块

	GAME_MAX,					//最大值
};

enum GAME_STATE
{
	GAMESTATE_RUNNING,				//进行中

	GAMESTATE_PAUSE,				//暂停

	GAMESTATE_OVER,					//结束

	GAMESTATE_PASS,					//通过
};

//提示类型
enum TIPS_TYPE
{
	TIPS_INVALID,
	TIPS_EXIT,
	TIPS_SAVEOPEN,
	TIPS_SAVECLOSE,
	TIPS_SAVEOK,
};


//全局常量
extern const bool BOOM_STATE[4][4];

//字符串常量
extern const char* BGM_START;

extern const char* EFFECT_ADD;
extern const char* EFFECT_BOOM;
extern const char* EFFECT_CHANGE;
extern const char* EFFECT_CHANGE2;
extern const char* EFFECT_DELETE;
extern const char* EFFECT_NEXT;
extern const char* EFFECT_PAUSE;
extern const char* EFFECT_SOUNDOFF;
extern const char* EFFECT_SOUNDON;
extern const char* EFFECT_WALL;

// ---- 通用函数 ---- 
int Random(int iStart, int iEnd, int iStep = 1);

//清空无效的数字
void ClearInvalidNums();

//添加无效的数字
void AddInvalidNum(int iNum);

//获取毫秒
double GetMillSecond();
