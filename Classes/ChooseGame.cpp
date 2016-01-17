#include "ChooseGame.h"
#include "DataManager.h"

CChooseGame::CChooseGame() : m_pAnimData(nullptr), m_iAnimIndex(0), m_bFirstAnim(false)
{

}


CChooseGame::~CChooseGame()
{

}


//初始化
void CChooseGame::Init()
{

}


//游戏进行，返回false时表示游戏结束
bool CChooseGame::Play()
{
	m_pAnimData = CDataManager::getInstance()->GetAnimData(RACING, m_iAnimIndex);
	
	//下一个动画
	if (++m_iAnimIndex >= ANIM_NUM)
	{
		m_iAnimIndex = 0;
	}

	return true;
}

//获取当前Brick状态
bool CChooseGame::GetBrickState(int iRowIndex, int iColIndex)
{
	int iIndex = iRowIndex * COLUMN_NUM + iColIndex;
	return m_pAnimData->at(iIndex);
}

//获取游戏类型
SCENE_INDEX CChooseGame::GetSceneType()
{
	return CHOOSE_GAME;
}


//获取每次执行完Play后等待的时间
float CChooseGame::GetRefreshTime()
{
	if (!m_bFirstAnim)
	{
		m_bFirstAnim = true;
		return 0;
	}
	return 1000;
}
