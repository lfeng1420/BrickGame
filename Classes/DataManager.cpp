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
	
	//获取上一次的声音设置
	m_bSoundOn = GET_BOOLVALUE("SOUND", true);
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
		"Anims/e1_1.txt",
		"Anims/e1_2.txt",
		"Anims/e1_3.txt",
		"Anims/e1_4.txt",
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


//播放背景音乐
void CDataManager::PlayMusic(const char* strName, bool bLoop)
{
	if (m_bSoundOn)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(strName, bLoop);
	}
}

//播放音效
void CDataManager::PlayEffect(const char* strName)
{
	if (m_bSoundOn)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(strName);
	}
}


//设置声音状态
void CDataManager::SetSoundState(bool bState)
{
	m_bSoundOn = bState;
	SET_BOOLVALUE("SOUND", m_bSoundOn);
}


//获取声音状态
bool CDataManager::GetSoundState()
{
	return m_bSoundOn;
}
