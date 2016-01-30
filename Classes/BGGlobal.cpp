#include "BGGlobal.h"

int Random(int iStart, int iEnd, int iStep)
{
	//如果上一次获取随机数的时间和本次相同，则不再随机种子
	static time_t stLastTime = 0;
	time_t tCurTime = time(NULL);
	if (difftime(tCurTime, stLastTime) > 0)
	{
		srand((unsigned)time(NULL));
		stLastTime = tCurTime;
	}

	vector<int> vecNum;
	for (int i = iStart < iEnd ? iStart : iEnd; i < (iEnd > iStart ? iEnd : iStart); i += iStep)
	{
		vecNum.push_back(i);
	}

	random_shuffle(vecNum.begin(), vecNum.end());
	
	//返回第一个
	vector<int>::iterator pIter = vecNum.begin();
	return *pIter;
}
