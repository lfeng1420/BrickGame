#include "FroggerGame.h"


CFroggerGame::CFroggerGame(CGameScene* pGameScene) : CSceneBase(pGameScene)
{
}


CFroggerGame::~CFroggerGame()
{
}


//初始化
void CFroggerGame::Init();

//更新
void CFroggerGame::Play(float dt);

//获取当前Brick状态
bool CFroggerGame::GetBrickState(int iRowIndex, int iColIndex);

//生命数
bool CFroggerGame::GetSmallBrickState(int iRowIndex, int iColIndex);

//获取游戏类型
SCENE_INDEX CFroggerGame::GetSceneType();

//左按下
void CFroggerGame::OnLeftBtnPressed();

//右按下
void CFroggerGame::OnRightBtnPressed();

//上按下
void CFroggerGame::OnUpBtnPressed();

//下按下
void CFroggerGame::OnDownPressed();

//Fire按下
void CFroggerGame::OnFireBtnPressed();
