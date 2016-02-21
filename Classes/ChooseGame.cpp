#include "ChooseGame.h"
#include "GeneralManager.h"

CChooseGame::CChooseGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{

}


CChooseGame::~CChooseGame()
{

}


//初始化
void CChooseGame::Init()
{
	m_iGameIndex = GET_INTVALUE("GAME", GAME_RACING);
	
	//更新速度和等级显示
	m_iSpeed = 0;
	m_iLevel = 0;
	m_pGameScene->UpdateLevel(m_iLevel);
	m_pGameScene->UpdateSpeed(m_iSpeed);

	m_iAnimIndex = 0;
	m_pAnimData = nullptr;
	m_bFirstAnim = false;
	m_fCurTime = 0;

	//重置分数
	m_pGameScene->UpdateScore(0, false);
	m_pGameScene->UpdateHighScore(m_iGameIndex);

	//背景音乐
	if (!AUDIO_INSTANCE()->isBackgroundMusicPlaying())
	{
		PLAY_BGMUSIC(BGM_START, true);
	}

	//重置小方块状态
	m_pGameScene->UpdateSmallBricks();
}


//更新
void CChooseGame::Play(float dt)
{
	m_fCurTime += dt;
	if (!m_bFirstAnim)
	{
		m_bFirstAnim = true;
	}
	else
	{
		if (m_fCurTime < REFRESH_INTERVAL)
		{
			return;
		}

		m_fCurTime = 0;
	}

	m_pAnimData = CGeneralManager::getInstance()->GetAnimData(m_iGameIndex, m_iAnimIndex);

	//下一个动画
	if (++m_iAnimIndex >= ANIM_NUM)
	{
		m_iAnimIndex = 0;
	}

	//更新所有Brick
	m_pGameScene->UpdateBricks();
}

//获取当前Brick状态
bool CChooseGame::GetBrickState(int iRowIndex, int iColIndex)
{
	int iIndex = iRowIndex * COLUMN_NUM + iColIndex;
	return m_pAnimData->at(iIndex) == 1;
}


bool CChooseGame::GetSmallBrickState(int iRowIndex, int iColIndex)
{
	return false;
}

//获取游戏类型
SCENE_INDEX CChooseGame::GetSceneType()
{
	return SCENE_CHOOSEGAME;
}

//左
void CChooseGame::OnLeftBtnPressed()
{
	if (--m_iLevel < 0)
	{
		m_iLevel = LEVEL_MAX;
	}

	m_pGameScene->UpdateLevel(m_iLevel);
}


//右
void CChooseGame::OnRightBtnPressed()
{
	if (++m_iLevel > LEVEL_MAX)
	{
		m_iLevel = 0;
	}

	m_pGameScene->UpdateLevel(m_iLevel);
}


//上
void CChooseGame::OnUpBtnPressed()
{
	if (++m_iSpeed > SPEED_MAX)
	{
		m_iSpeed = 0;
	}

	m_pGameScene->UpdateSpeed(m_iSpeed);
}


//下
void CChooseGame::OnDownPressed()
{
	if (--m_iSpeed < 0)
	{
		m_iSpeed = SPEED_MAX;
	}

	m_pGameScene->UpdateSpeed(m_iSpeed);
}


//Fire
void CChooseGame::OnFireBtnPressed()
{
	if (++m_iGameIndex >= GAME_MAX)
	{
		m_iGameIndex = 0;
	}

	m_iAnimIndex = 0;

	//更新最高分显示
	m_pGameScene->UpdateHighScore(m_iGameIndex);

	Play(REFRESH_INTERVAL);
}


//游戏索引转换场景索引
const int GAMEIDX_TO_SCENEIDX[] =
{
	SCENE_TANK,			//对应GAME_TANK
	SCENE_RACING,		//对应GAME_RACING
	SCENE_SNAKE,		//对应GAME_SNAKE
	SCENE_MATCH,		//对应GAME_MATCH
	SCENE_FROGGER,		//对应GAME_FROGGER
	SCENE_PINBALL,		//对应GAME_PINBALL
	SCENE_TETRIS,		//对应GAME_TETRIS
};

//开始
void CChooseGame::OnStart()
{
	//停止背景音乐
	STOP_BGMUSIC();

	//播放开始音效
	PLAY_EFFECT(EFFECT_PAUSE);

	//设置游戏索引，生命，等级和速度
	SET_INTVALUE("GAME", m_iGameIndex);
	SET_INTVALUE("SPEED", m_iSpeed);
	SET_INTVALUE("LEVEL", m_iLevel);
	SET_INTVALUE("LIFE", 4);			//默认生命：4

	//切换新游戏
	m_pGameScene->RunScene(GAMEIDX_TO_SCENEIDX[m_iGameIndex]);
}
