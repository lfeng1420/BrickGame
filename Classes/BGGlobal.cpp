#include "BGGlobal.h"

int Random(int iStart, int iEnd, int iStep)
{
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
