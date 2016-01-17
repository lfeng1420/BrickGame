#include "DataManager.h"

CDataManager* _dataManager = nullptr;


CDataManager::CDataManager()
{
}


CDataManager::~CDataManager()
{
}


CDataManager* CDataManager::getInstance()
{
	if (_dataManager == nullptr)
	{
		_dataManager = new CDataManager();
		_dataManager->Init();
	}

	return _dataManager;
}

//初始化·
void CDataManager::Init()
{
	LoadGameAnim();
	
	//测试内容输出
	/*for (int i = 0; i < m_mapGameAnim.size(); ++i)
	{
		string strTemp;
		TVECTOR_ANIMDATA_ITER pAnimIter = m_mapGameAnim[i].begin();
		log("%d", m_mapGameAnim[i].size());
		for (int iCount = 0; pAnimIter != m_mapGameAnim[i].end(); ++pAnimIter, ++iCount)
		{
			if (iCount == COLUMN_NUM)
			{
				log("%s", strTemp.c_str());
				strTemp = "";
				iCount = 0;
			}

			char ch = *pAnimIter + 48;
			strTemp += ch;
		}
	}*/
	
}

//加载动画
bool CDataManager::LoadGameAnim()
{
	//动画文件
	const string arrAnimFilePath[ANIM_NUM * GAME_NUM] =
	{
		"Anims/b1_1.txt",
		"Anims/b1_2.txt",
		"Anims/b1_3.txt",
		"Anims/b1_4.txt",
	};

	for (int i = 0; i < GAME_NUM * ANIM_NUM; ++i)
	{
		ifstream oFileStream(arrAnimFilePath[i].c_str());
		if (!oFileStream.is_open())
		{
			log("Read Data From '%s' FAILED.", arrAnimFilePath[i].c_str());
			return false;
		}

		TVECTOR_ANIMDATA& vecAnimData = m_mapGameAnim[i];
		char ch = '\0';
		while (!oFileStream.eof())
		{
			oFileStream.get(ch);
			if (ch == '0')
			{
				vecAnimData.push_back(1);
			}
			else if (ch == '-')
			{
				vecAnimData.push_back(0);
			}
		}
	}

	return true;
}


vector<int>* CDataManager::GetAnimData(int iGameIndex, int iAnimIndex)
{
	int iIndex = iGameIndex * ANIM_NUM + iAnimIndex;
	return &m_mapGameAnim[iIndex];
}
