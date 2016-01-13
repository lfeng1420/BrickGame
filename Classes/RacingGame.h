#pragma once
#include "cocos2d.h"
#include "GlobalDef.h"

class CRacingGame : public cocos2d::LayerColor
{
public:
	CRacingGame();
	~CRacingGame();

	static cocos2d::Scene* CreateScene();

	virtual bool init();

	//初始化数据
	void InitData();

	//初始化Brick
	void InitBrick();

	//根据指定的高度上限创建控制按钮
	void InitCotroller();

	//播放游戏结束动画
	void PlayGameOverAnim();

	void update(float dt);

	CREATE_FUNC(CRacingGame);

private:
	cocos2d::Sprite* m_pBrick[ROW_NUM][COLUMN_NUM];		//Sprite数组
	
	int m_arrCurBrick[ROW_NUM][COLUMN_NUM];				//当前Brick状态

	int m_arrNextBrick[ROW_NUM][COLUMN_NUM];			//下一次刷新时Birck状态

	cocos2d::Size m_visibleSize;						//屏幕大小

	int m_iColIdx;
	int m_iRowIdx;

	int m_iBeginColIdx;
	int m_iBeginRowIdx;
	int m_iEndColIdx;
	int m_iEndRowIdx;
	int m_iDirection;
	
	int m_iAllCount;
};

