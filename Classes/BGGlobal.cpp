#include "BGGlobal.h"

const bool BOOM_STATE[4][4] =
{
	{ true, false, false, true },
	{ false, true, true, false },
	{ false, true, true, false },
	{ true, false, false, true },
};

#if CC_TARGET_PLATFORM == CC_PLATFORM_WP8 
const char* BGM_START = "Sounds/bgm.wav";
const char* EFFECT_ADD = "Sounds/add.wav";
const char* EFFECT_BOOM = "Sounds/boom.wav";
const char* EFFECT_CHANGE = "Sounds/change.wav";
const char* EFFECT_CHANGE2 = "Sounds/change2.wav";
const char* EFFECT_DELETE = "Sounds/delete.wav";
const char* EFFECT_NEXT = "Sounds/next.wav";
const char* EFFECT_PAUSE = "Sounds/pause.wav";
const char* EFFECT_SOUNDOFF = "Sounds/sndoff.wav";
const char* EFFECT_SOUNDON = "Sounds/sndon.wav";
const char* EFFECT_WALL = "Sounds/wall.wav";
#else
const char* BGM_START = "Sounds/bgm.mp3";
const char* EFFECT_ADD = "Sounds/add.mp3";
const char* EFFECT_BOOM = "Sounds/boom.mp3";
const char* EFFECT_CHANGE = "Sounds/change.mp3";
const char* EFFECT_CHANGE2 = "Sounds/change2.mp3";
const char* EFFECT_DELETE = "Sounds/delete.mp3";
const char* EFFECT_NEXT = "Sounds/next.mp3";
const char* EFFECT_PAUSE = "Sounds/pause.mp3";
const char* EFFECT_SOUNDOFF = "Sounds/sndoff.mp3";
const char* EFFECT_SOUNDON = "Sounds/sndon.mp3";
const char* EFFECT_WALL = "Sounds/wall.mp3";
#endif

//无效的数字列表，随机时将排除在外
map<int, int> mapInvalidList;


int Random(int iStart, int iEnd, int iStep)
{
	//如果上一次获取随机数的时间和本次相同，则不再随机种子
	bool bSameTime = true;
	static time_t stLastTime = 0;
	time_t tCurTime = time(nullptr);
	if (difftime(tCurTime, stLastTime) > 0)
	{
		srand((unsigned)time(nullptr));
		stLastTime = tCurTime;
		
		bSameTime = false;
	}

	int iStartIdx = (iStart < iEnd ? iStart : iEnd);
	int iEndIdx = (iEnd > iStart ? iEnd : iStart);

	vector<int> vecNum;
	for (int i = iStartIdx; i < iEndIdx; i += iStep)
	{
		if (mapInvalidList[i] == 1)
		{
			continue;
		}

		vecNum.push_back(i);
	}

	random_shuffle(vecNum.begin(), vecNum.end());

	//返回结果，如果是相同时间，则返回不同位置的值
	int iIndex = 0;
	if (bSameTime)
	{
		iIndex = rand() % vecNum.size();
	}
	return vecNum[iIndex];
}


//清空无效的数字
void ClearInvalidNums()
{
	mapInvalidList.clear();
}


//添加无效的数字
void AddInvalidNum(int iNum)
{
	mapInvalidList[iNum] = 1;
}


double GetMillSecond()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);

	//log("CurrentTime MillSecond %f", (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000);
	return (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000;

}
