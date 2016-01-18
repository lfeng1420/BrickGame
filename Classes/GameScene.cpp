#include "GameScene.h"
#include "GameOver.h"
#include "DataManager.h"
#include "ChooseGame.h"
#include "RacingGame.h"

CGameScene::CGameScene() : m_iSceneIndex(SCENE_GAMEOVER)
{
}


CGameScene::~CGameScene()
{
	for (int i = 0; i < m_mapGameObj.size(); ++i)
	{
		SAFE_DELETE(m_mapGameObj[i]);
	}
}


Scene* CGameScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = CGameScene::create();
	scene->addChild(layer);
	return scene;
}


bool CGameScene::init()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Brick.plist");

	CC_RETURN_FALSE_IF(!LayerColor::initWithColor(Color4B::WHITE));

	CDataManager::getInstance();

	//初始化变量，获取数据等
	InitData();

	//UI初始化
	InitUI();

	//初始化方块
	InitBrick();

	//初始化控制器
	InitCotroller();

	//创建游戏对象
	CreateGameObj();

	//帧更新
	this->scheduleUpdate();

	return true;
}


void CGameScene::InitData()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
}


//初始化Brick
void CGameScene::InitBrick()
{
	float fCurY = m_visibleSize.height;
	for (int i = 0; i < ROW_NUM; ++i)
	{
		float fCurX = 0;
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			m_arrBrickState[i][j] = false;
			m_pArrBrick[i][j] = Sprite::create("empty.png");
			m_pArrBrick[i][j]->setPosition(fCurX + BRICK_WIDTH * 1.0f / 2, fCurY - BRICK_HEIGHT * 1.0f / 2);
			this->addChild(m_pArrBrick[i][j]);

			fCurX += BRICK_WIDTH;
		}

		fCurY -= BRICK_HEIGHT;
	}
}


//初始化UI:分数、等级等
void CGameScene::InitUI()
{
	TTFConfig ttfConfig("Fonts/DSDigital.ttf", 46);
	Size visibleSize = GET_VISIBLESIZE();

	//图片缩放
	float fSpriteScale = 0.38f;

	//分数
	auto pScore = Sprite::create("score.png");
	pScore->setScale(fSpriteScale);
	Size scoreSize = GET_CONTENTSIZE(pScore) * fSpriteScale;
	float fCurX = (visibleSize.width - COLUMN_NUM * BRICK_WIDTH) / 2 + COLUMN_NUM * BRICK_WIDTH;
	float fCurY = visibleSize.height - scoreSize.height * 3 / 2;
	pScore->setPosition(fCurX, fCurY + scoreSize.height / 2);
	this->addChild(pScore);

	//分数Label
	m_pScoreLabel = Label::createWithTTF(ttfConfig, "123456", TextHAlignment::CENTER);
	Size scoreLabelSize = GET_CONTENTSIZE(m_pScoreLabel);
	fCurY -= scoreLabelSize.height * 2.5f / 2;
	m_pScoreLabel->setPosition(fCurX, fCurY + scoreLabelSize.height / 2);
	m_pScoreLabel->setTextColor(Color4B::BLACK);
	this->addChild(m_pScoreLabel);

	//最高分
	auto pHighScore = Sprite::create("hiscore.png");
	pHighScore->setScale(fSpriteScale);
	Size highScoreSize = GET_CONTENTSIZE(pHighScore) * fSpriteScale;
	fCurY -= highScoreSize.height * 3 / 2;
	pHighScore->setPosition(fCurX, fCurY + highScoreSize.height / 2);
	this->addChild(pHighScore);

	//分数Label
	m_pHighScoreLabel = Label::createWithTTF(ttfConfig, "000000", TextHAlignment::CENTER);
	Size highScoreLabelSize = GET_CONTENTSIZE(m_pHighScoreLabel);
	fCurY -= highScoreLabelSize.height * 2.5f / 2;
	m_pHighScoreLabel->setPosition(fCurX, fCurY + highScoreLabelSize.height / 2);
	m_pHighScoreLabel->setTextColor(Color4B::BLACK);
	this->addChild(m_pHighScoreLabel);

	//小方块序列
	float fBrickScale = 0.7f;
	float fPadding = 4;
	fCurY -= highScoreLabelSize.height / 2;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			float fX = fCurX + (BRICK_HEIGHT * fBrickScale + fPadding) * (j - 1.5f);
			m_pArrSmallBrick[i][j] = Sprite::createWithSpriteFrameName("empty.png");
			m_pArrSmallBrick[i][j]->setScale(fBrickScale);
			m_pArrSmallBrick[i][j]->setPosition(fX, fCurY - BRICK_HEIGHT * fBrickScale / 2);
			this->addChild(m_pArrSmallBrick[i][j]);
		}

		fCurY -= BRICK_HEIGHT * fBrickScale + fPadding;
	}

	//速度
	auto pSpeed = Sprite::create("speed.png");
	pSpeed->setScale(fSpriteScale);
	Size speedSize = GET_CONTENTSIZE(pSpeed);
	fCurY -= speedSize.height;
	pSpeed->setPosition(fCurX, fCurY + speedSize.height / 2);
	this->addChild(pSpeed);

	//速度Label
	m_pSpeedLabel = Label::createWithTTF(ttfConfig, "10", TextHAlignment::CENTER);
	Size speedLabelSize = GET_CONTENTSIZE(m_pSpeedLabel);
	fCurY -= speedLabelSize.height;
	m_pSpeedLabel->setPosition(fCurX, fCurY + speedLabelSize.height / 2);
	m_pSpeedLabel->setTextColor(Color4B::BLACK);
	this->addChild(m_pSpeedLabel);

	//等级Label
	m_pLevelLabel = Label::createWithTTF(ttfConfig, "10", TextHAlignment::CENTER);
	Size levelLabelSize = GET_CONTENTSIZE(m_pLevelLabel);
	fCurY -= levelLabelSize.height;
	m_pLevelLabel->setPosition(fCurX, fCurY + levelLabelSize.height / 2);
	m_pLevelLabel->setTextColor(Color4B::BLACK);
	this->addChild(m_pLevelLabel);

	//等级
	auto pLevel = Sprite::create("level.png");
	pLevel->setScale(fSpriteScale);
	Size levelSize = GET_CONTENTSIZE(pLevel);
	fCurY -= levelSize.height;
	pLevel->setPosition(fCurX, fCurY + levelSize.height / 2);
	this->addChild(pLevel);

	//暂停图标
	m_pPauseSpr = Sprite::create("pause.png");
	m_pPauseSpr->setScale(0.32f);
	Size pauseSize = GET_CONTENTSIZE(m_pPauseSpr) * 0.32f;
	fCurY -= pauseSize.height;
	m_pPauseSpr->setPosition(fCurX, fCurY + pauseSize.height / 2);
	this->addChild(m_pPauseSpr);
}


void CGameScene::InitCotroller()
{
	//剩余高度，用于调整控制按钮位置
	float fHeight = m_visibleSize.height - BRICK_HEIGHT * ROW_NUM;

	auto upBtn = MenuItemSprite::create(
		Sprite::create("lineDark01.png"),
		Sprite::create("flatDark01.png"),
		CC_CALLBACK_1(CGameScene::OnBtnClick, this, BTN_UP)
		);
	Size upBtnSize = upBtn->getContentSize();

	auto rightBtn = MenuItemSprite::create(
		Sprite::create("lineDark02.png"),
		Sprite::create("flatDark02.png"),
		CC_CALLBACK_1(CGameScene::OnBtnClick, this, BTN_RIGHT)
		);
	Size rightBtnSize = rightBtn->getContentSize();

	auto downBtn = MenuItemSprite::create(
		Sprite::create("lineDark03.png"),
		Sprite::create("flatDark03.png"),
		CC_CALLBACK_1(CGameScene::OnBtnClick, this, BTN_DOWN)
		);
	Size downBtnSize = downBtn->getContentSize();

	auto leftBtn = MenuItemSprite::create(
		Sprite::create("lineDark04.png"),
		Sprite::create("flatDark04.png"),
		CC_CALLBACK_1(CGameScene::OnBtnClick, this, BTN_LEFT)
		);
	Size leftBtnSize = leftBtn->getContentSize();

	auto fireBtn = MenuItemSprite::create(
		Sprite::create("lineDark05.png"),
		Sprite::create("flatDark05.png"),
		CC_CALLBACK_1(CGameScene::OnBtnClick, this, BTN_FIRE)
		);
	Size fireBtnSize = fireBtn->getContentSize();


	float fScale = 1.5f;
	auto startBtn = MenuItemSprite::create(
		Sprite::create("start_0.png"),
		Sprite::create("start_1.png"),
		CC_CALLBACK_1(CGameScene::OnBtnClick, this, BTN_START)
		);
	startBtn->setScale(fScale);
	Size startBtnSize = startBtn->getContentSize() * fScale;

	auto soundBtn = MenuItemSprite::create(
		Sprite::create("sound_on_0.png"),
		Sprite::create("sound_on_1.png"),
		CC_CALLBACK_1(CGameScene::OnBtnClick, this, BTN_SOUND)
		);
	soundBtn->setScale(fScale);
	Size soundBtnSize = soundBtn->getContentSize() * fScale;

	auto resetBtn = MenuItemSprite::create(
		Sprite::create("reset_0.png"),
		Sprite::create("reset_1.png"),
		CC_CALLBACK_1(CGameScene::OnBtnClick, this, BTN_RESET)
		);
	resetBtn->setScale(fScale);
	Size resetBtnSize = resetBtn->getContentSize() * fScale;

	leftBtn->setPosition(leftBtnSize.width, downBtnSize.height * 2.0f + 6);
	rightBtn->setPosition(leftBtnSize.width * 1.5f + rightBtnSize.width / 2 + 12, downBtnSize.height * 2.0f + 6);
	downBtn->setPosition(leftBtnSize.width * 1.5f + 6, downBtnSize.height * 1.5f);
	upBtn->setPosition(leftBtnSize.width * 1.5f + 6, downBtnSize.height * 2.0f + 12 + upBtnSize.height / 2);
	fireBtn->setPosition(m_visibleSize.width - (leftBtnSize.width * 1.5f + 6), downBtnSize.height * 2.0f + 6);

	soundBtn->setPosition(m_visibleSize.width / 2, soundBtnSize.height / 2);
	startBtn->setPosition(m_visibleSize.width / 2 - 20 - soundBtnSize.width / 2 - startBtnSize.width / 2, soundBtnSize.height / 2);
	resetBtn->setPosition(m_visibleSize.width / 2 + 20 + soundBtnSize.width / 2 + resetBtnSize.width / 2, soundBtnSize.height / 2);

	auto menu = Menu::create(leftBtn, upBtn, rightBtn, downBtn, fireBtn, soundBtn, startBtn, resetBtn, NULL);
	float fCurHeight = downBtnSize.height * 2.0f + 12 + upBtnSize.height;
	menu->setPosition(Vec2(0, (fHeight - fCurHeight) / 3));
	this->addChild(menu);
}


void CGameScene::update(float dt)
{
	m_mapGameObj[m_iSceneIndex]->Play(dt);
}


//创建各个游戏对象
void CGameScene::CreateGameObj()
{
	//游戏结束
	CGameOver* pGameOver = new CGameOver(this);
	pGameOver->Init();
	m_mapGameObj[SCENE_GAMEOVER] = pGameOver;

	//选择游戏
	CChooseGame* pChooseGame = new CChooseGame(this);
	pChooseGame->Init();
	m_mapGameObj[SCENE_CHOOSEGAME] = pChooseGame;

	//赛车
	CRacingGame* pRacingGame = new CRacingGame(this);
	pRacingGame->Init();
	m_mapGameObj[SCENE_RACING] = pRacingGame;
}


//更新单个Brick状态
void CGameScene::UpdateBrick(int iRowIndex, int iColIndex, bool bSmallBrickFlag, bool bShowFlag)
{
	Sprite* pBrick = nullptr;
	if (bSmallBrickFlag)
	{
		pBrick = m_pArrSmallBrick[iRowIndex][iColIndex];
	}
	else
	{
		//更新状态记录
		m_arrBrickState[iRowIndex][iColIndex] = bShowFlag;

		pBrick = m_pArrBrick[iRowIndex][iColIndex];
	}

	//设置显示或隐藏状态
	if (bShowFlag)
	{
		pBrick->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("black.png"));
	}
	else
	{
		pBrick->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("empty.png"));
	}
}


//更新所有Brick状态
void CGameScene::UpdateBricks()
{
	for (int i = 0; i < ROW_NUM; ++i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			bool bState = m_mapGameObj[m_iSceneIndex]->GetBrickState(i, j);
			if (m_arrBrickState[i][j] != bState)
			{
				UpdateBrick(i, j, false, bState);
			}
		}
	}
}


//重置所有Brick
void CGameScene::ResetBricks()
{
	for (int i = 0; i < ROW_NUM; ++i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			UpdateBrick(i, j, false, false);
		}
	}
}


//按钮响应
void CGameScene::OnBtnClick(Ref* pSender, int iBtnIndex)
{
	switch (iBtnIndex)
	{
	case BTN_DOWN:
		m_mapGameObj[m_iSceneIndex]->OnDown();
		break;

	case BTN_LEFT:
		m_mapGameObj[m_iSceneIndex]->OnLeft();
		break;

	case BTN_RIGHT:
		m_mapGameObj[m_iSceneIndex]->OnRight();
		break;

	case BTN_UP:
		m_mapGameObj[m_iSceneIndex]->OnUp();
		break;

	case BTN_FIRE:
		m_mapGameObj[m_iSceneIndex]->OnFire();
		break;

	case BTN_START:
		m_mapGameObj[m_iSceneIndex]->OnStart();
		break;

	case BTN_SOUND:
		break;

	case BTN_RESET:
		break;
	}
}

//显示新场景
void CGameScene::ShowNewScene(int iSceneIndex)
{
	log("Old Scene: %d   Current Scene: %d", m_iSceneIndex, iSceneIndex);
	m_iSceneIndex = iSceneIndex;
	m_mapGameObj[m_iSceneIndex]->Init();
}


//更新分数显示
void CGameScene::UpdateScore(int iScore)
{
	log("Current Score: %d", iScore);
	m_pScoreLabel->setString(StringUtils::format("%06d", iScore));
}


//更新等级显示
void CGameScene::UpdateLevel(int iLevel)
{
	log("Current Level: %d", iLevel);
	m_pLevelLabel->setString(StringUtils::format("%d", iLevel));
}


//更新速度显示
void CGameScene::UpdateSpeed(int iSpeed)
{
	log("Current Speed: %d", iSpeed);
	m_pSpeedLabel->setString(StringUtils::format("%d", iSpeed));
}


//更新小方块序列
void CGameScene::UpdateSmallBricks()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			bool bState = m_mapGameObj[m_iSceneIndex]->GetSmallBrickState(i, j);
			UpdateBrick(i, j, true, bState);
		}
	}
}


//重置所有小Brick
void CGameScene::ResetSmallBricks()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			UpdateBrick(i, j, true, false);
		}
	}
}
