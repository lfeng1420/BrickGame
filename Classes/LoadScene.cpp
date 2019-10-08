#include "stdafx.h"
#include "LoadScene.h"
#include "GameScene.h"
#include "GameSceneEx.h"
#include "GameLogic.h"


bool CLoadScene::init()
{
	if (!LayerColor::initWithColor(GET_BOOLVALUE("NIGHTMODE", false) ? Color4B::BLACK : Color4B::WHITE))
	{
		return false;
	}

	ADD_SPRITEFRAME("Plists/Brick.plist");
	ADD_SPRITEFRAME("Plists/Font.plist");
	ADD_SPRITEFRAME("Plists/Images.plist");

    PRELOAD_BGMUSIC(BGM_START);
    PRELOAD_EFFECT(EFFECT_ADD);
    PRELOAD_EFFECT(EFFECT_BOOM);
    PRELOAD_EFFECT(EFFECT_CHANGE);
    PRELOAD_EFFECT(EFFECT_DELETE);
    PRELOAD_EFFECT(EFFECT_NEXT);
    PRELOAD_EFFECT(EFFECT_START);
    PRELOAD_EFFECT(EFFECT_SOUNDOFF);
    PRELOAD_EFFECT(EFFECT_SOUNDON);
    PRELOAD_EFFECT(EFFECT_WALL);


	// Game logic
	CGameLogic::GetInstance();

	// Volume
	SET_BGMUSICVOLUME(GET_INTVALUE("BGM_VOLUME", 100) * 0.01f);
	SET_EFFECTVOLUME(GET_INTVALUE("EFFECT_VOLUME", 100) * 0.01f);

	// UI
	__InitUI();

	// Delay switch
	scheduleOnce(schedule_selector(CLoadScene::__SwitchToGameScene), 2.0f);

	return true;
}


void CLoadScene::__InitUI()
{
	Size visibleSize = GET_VISIBLESIZE();
	bool arrBrick[UI_BRICKS_ROW_COUNT][UI_BRICKS_COLUMN_COUNT] =
	{
		true, true, true, true, true, false, false, false, true, true, true, true, false,
		false, true, false, false, false, true, false, true, false, false, false, false, false,
		false, true, true, true, true, false, false, true, false, false, false, true, true,
		false, true, false, false, false, true, false, true, false, false, false, false, true,
		true, true, true, true, true, false, false, false, true, true, true, true, false,
	};

	string strSprName = CGlobalFunc::GetSpriteNameWithMode("black.png");
	Sprite* pTempSpr = CREATE_SPRITEWITHNAME(strSprName);
	Size brickSize = GET_CONTENTSIZE(pTempSpr);

	bool bPortraitFlag = GET_BOOLVALUE("PORTRAIT", true);
	if (bPortraitFlag)
	{
		float fStartY = (visibleSize.height - UI_BRICKS_ROW_COUNT * brickSize.height) * 0.5f;
		float fStartX = (visibleSize.width - UI_BRICKS_COLUMN_COUNT * brickSize.width) * 0.5f;
		for (int i = UI_BRICKS_ROW_COUNT - 1; i >= 0; --i)
		{
			float fCurX = fStartX;
			for (int j = 0; j < UI_BRICKS_COLUMN_COUNT; ++j)
			{
				if (arrBrick[i][j])
				{
					Sprite* pSpr = CREATE_SPRITEWITHNAME(strSprName);
					pSpr->setPosition(fCurX + brickSize.width * 0.5f, fStartY + brickSize.height * 0.5f);
					this->addChild(pSpr);
				}

				fCurX += brickSize.width;
			}

			fStartY += brickSize.height;
		}
	}
	else
	{
		float fStartY = (visibleSize.height - UI_BRICKS_COLUMN_COUNT * brickSize.height) / 2;
		float fCurX = (visibleSize.width - UI_BRICKS_ROW_COUNT * brickSize.width) / 2;
		for (int i = UI_BRICKS_ROW_COUNT - 1; i >= 0; --i)
		{
			float fCurY = fStartY;
			for (int j = UI_BRICKS_COLUMN_COUNT - 1; j >= 0; --j)
			{
				if (arrBrick[i][j])
				{
					auto sprite = CREATE_SPRITEWITHNAME(strSprName);
					sprite->setPosition(fCurX + brickSize.width / 2, fCurY + brickSize.height / 2);
					this->addChild(sprite);
				}

				fCurY += brickSize.height;
			}

			fCurX += brickSize.width;
		}
	}
}


void CLoadScene::__SwitchToGameScene(float dt)
{
	bool bPortraitFlag = GET_BOOLVALUE("PORTRAIT", true);
	REPLACE_SCENE(bPortraitFlag ? CGameScene::CreateScene() : CGameSceneEx::CreateScene());
}
