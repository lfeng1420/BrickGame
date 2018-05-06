#pragma once

#define SAFE_DELETE(x) {if (x) {delete (x); (x) = nullptr;}}

#define RETURN_IF_NULLPTR(p)	if ((p) == nullptr){return;}

#define FOREACH_IN_CONTAINER(containertype, container, it)   for (containertype::iterator it = (container).begin(); it != (container).end(); ++it)

#define FOREACH_IN_CONST_CONTAINER(containertype, container, it)   for (containertype::const_iterator it = (container).cbegin(); it != (container).cend(); ++it)

#define SCENE_COMMON_FUNC(cls)				\
	CREATE_FUNC(cls);						\
	static Scene* CreateScene()				\
	{										\
		auto scene = Scene::create();		\
		auto layer = cls::create();			\
		scene->addChild(layer);				\
		return scene;						\
	}										\
	bool init();							

#define INIT_BOOL_ARRAY(array)	std::fill((array), (array) + _countof(array), false)

// min / max
//#define max(a,b) (((a) > (b)) ? (a) : (b))
//#define min(a,b) (((a) < (b)) ? (a) : (b))

// _countof
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

// Get brick index
#define GET_BRICKID(row, col)	((row) * COLUMN_COUNT + (col))
#define GET_SMALLBRICKID(row, col)	((row) * SMALL_BRICK_COLUMN_COUNT + (col))
#define GET_BRICKIDBYPOS(pos)	((pos).x * COLUMN_COUNT + (pos).y)

// Check pos equal
#define EQUAL_POS(pos1, pos2)	((pos1).x == (pos2).x && (pos1).y == (pos2).y)


/////////////  Cocos macro  /////////////

#define DIRECTOR_INSTANCE Director::getInstance

#define GET_VISIBLESIZE Director::getInstance()->getVisibleSize

#define GET_VISIBLEORIGIN Director::getInstance()->getVisibleOrigin

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

#define PLAY_BGMUSIC(name, loop) CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(name, loop)

#define STOP_BGMUSIC CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic

#define PAUSE_BGMUSIC CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic

#define PLAYING_BGMUSIC CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying

#define PRELOAD_EFFECT(name) CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(name)

#define PLAY_EFFECT(name) CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(name)

#define SET_EFFECTVOLUME(value) CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(value)

#define GET_EFFECTVOLUME CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume

#define SET_BGMUSICVOLUME(value) CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value)

#define GET_BGMUSICVOLUME(value) CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume()

#define GET_CONTENTSIZE(sprite) sprite->getContentSize()

#define USERDEFAULT_INSTANCE UserDefault::getInstance

#define GET_INTVALUE(key, value) UserDefault::getInstance()->getIntegerForKey(key, value)

#define SET_INTVALUE(key, value) UserDefault::getInstance()->setIntegerForKey(key, value)

#define GET_BOOLVALUE(key, value) UserDefault::getInstance()->getBoolForKey(key, value)

#define SET_BOOLVALUE(key, value) UserDefault::getInstance()->setBoolForKey(key, value)

#define GET_STRVALUE(key, value) UserDefault::getInstance()->getStringForKey(key, value)

#define SET_STRVALUE(key, value) UserDefault::getInstance()->setStringForKey(key, value)
