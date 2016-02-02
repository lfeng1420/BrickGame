#include "GeneralManager.h"

CGeneralManager* _dataManager = nullptr;


const string HIGHSCORE_FILEPATH = "./HighScore.json";

CGeneralManager::CGeneralManager()
{
}


CGeneralManager::~CGeneralManager()
{
}


CGeneralManager* CGeneralManager::getInstance()
{
	if (_dataManager == nullptr)
	{
		_dataManager = new CGeneralManager();
		_dataManager->Init();
	}

	return _dataManager;
}

//初始化·
void CGeneralManager::Init()
{
	LoadGameAnim();
	
	LoadHighScore();

	LoadSound();

	//获取上一次的声音设置
	m_bSoundOn = GET_BOOLVALUE("SOUND", true);

	//清除之前记录的一些数据
	SET_INTVALUE("GAME", 0);
	SET_INTVALUE("SPEED", 0);
	SET_INTVALUE("LEVEL", 0);
}

//加载动画
bool CGeneralManager::LoadGameAnim()
{
	//动画文件
	const string arrAnimFilePath[ANIM_NUM * GAME_MAX] =
	{
		"Anims/a1_1.txt",
		"Anims/a1_2.txt",
		"Anims/a1_3.txt",
		"Anims/a1_4.txt",
		"Anims/b1_1.txt",
		"Anims/b1_2.txt",
		"Anims/b1_3.txt",
		"Anims/b1_4.txt",
		"Anims/c1_1.txt",
		"Anims/c1_2.txt",
		"Anims/c1_3.txt",
		"Anims/c1_4.txt",
		"Anims/d1_1.txt",
		"Anims/d1_2.txt",
		"Anims/d1_3.txt",
		"Anims/d1_4.txt",
		"Anims/e1_1.txt",
		"Anims/e1_2.txt",
		"Anims/e1_3.txt",
		"Anims/e1_4.txt",
		"Anims/f1_1.txt",
		"Anims/f1_2.txt",
		"Anims/f1_3.txt",
		"Anims/f1_4.txt",
	};

	for (int i = 0; i < GAME_MAX * ANIM_NUM; ++i)
	{
		string strPath = FileUtils::getInstance()->fullPathForFilename(arrAnimFilePath[i]);
		string strContent = FileUtils::getInstance()->getStringFromFile(strPath);

		TVECTOR_ANIMDATA& vecAnimData = m_mapGameAnim[i];
		for (unsigned int i = 0; i < strContent.size(); ++i)
		{
			if (strContent[i] == '0')
			{
				vecAnimData.push_back(1);
			}
			else if (strContent[i] == '-')
			{
				vecAnimData.push_back(0);
			}
		}
	}

	return true;
}


//加载最高分
bool CGeneralManager::LoadHighScore()
{
	string strPath = FileUtils::getInstance()->getWritablePath() + string(HIGHSCORE_FILEPATH);

	string strContent = FileUtils::getInstance()->getStringFromFile(strPath);
	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (oDoc.HasParseError() || oDoc.IsNull())
	{
		log("CGeneralManager::LoadUserData Load 'oDoc' Error.");
		return false;
	}

	for (int i = 0; i < oDoc["HighScore"].Size(); i++)
	{
		if (oDoc["HighScore"][i].IsNull())
		{
			log("CGeneralManager::LoadUserData Load 'oDoc[HighScore][%d]' Error.", i);
			return false;
		}

		int iValue = oDoc["HighScore"][i].GetInt();
		m_mapHighScore[i] = iValue;
	}

	return true;
}


//保存最高分到文件中
bool CGeneralManager::SaveHighScoreToFile()
{
	string strPath = FileUtils::getInstance()->getWritablePath() + string(HIGHSCORE_FILEPATH);
	string strContent = FileUtils::getInstance()->getStringFromFile(strPath);
	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (oDoc.HasParseError() || oDoc.IsNull())
	{
		oDoc.SetObject();
		rapidjson::Document::AllocatorType& allocator = oDoc.GetAllocator();
		rapidjson::Value oArray(rapidjson::kArrayType);

		for (int i = 0; i < GAME_MAX; ++i)
		{
			oArray.PushBack(m_mapHighScore[i], allocator);
		}
		oDoc.AddMember("HighScore", oArray, allocator);
	}
	else
	{
		//更新数据
		for (rapidjson::SizeType i = 0; i < oDoc["HighScore"].Capacity(); i++)
		{
			if (oDoc["HighScore"][i].IsNull())
			{
				return false;
			}
			oDoc["HighScore"][i].SetInt(m_mapHighScore[i]);
		}
	}

	rapidjson::StringBuffer oBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> oWriter(oBuffer);
	oDoc.Accept(oWriter);

	FILE* pFile = fopen(strPath.c_str(), "wb+");
	if (pFile == NULL)
	{
		log("Open File '%s' Failed.", strPath.c_str());
		return false;
	}

	fputs(oBuffer.GetString(), pFile);
	fclose(pFile);

	return true;
}


//加载声效
void CGeneralManager::LoadSound()
{
	PRELOAD_BGMUSIC(BGM_START);

	const char* EFFECT_LIST[] =
	{
		EFFECT_ADD,
		EFFECT_BOOM,
		EFFECT_CHANGE,
		EFFECT_CHANGE2,
		EFFECT_DELETE,
		EFFECT_NEXT,
		EFFECT_PAUSE,
		EFFECT_SOUNDOFF,
		EFFECT_SOUNDON,
		EFFECT_WALL,
	};

	for (int i = 0; i < 10; ++i)
	{
		PRELOAD_EFFECT(EFFECT_LIST[i]);
	}
}


vector<int>* CGeneralManager::GetAnimData(int iGameIndex, int iAnimIndex)
{
	int iIndex = iGameIndex * ANIM_NUM + iAnimIndex;
	return &m_mapGameAnim[iIndex];
}


//播放背景音乐
void CGeneralManager::PlayMusic(const char* strName, bool bLoop)
{
	if (m_bSoundOn)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(strName, bLoop);
	}
}

//播放音效
void CGeneralManager::PlayEffect(const char* strName)
{
	if (m_bSoundOn)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(strName);
	}
}


//设置声音状态
void CGeneralManager::SetSoundState(bool bState)
{
	m_bSoundOn = bState;
	SET_BOOLVALUE("SOUND", m_bSoundOn);
}


//获取声音状态
bool CGeneralManager::GetSoundState()
{
	return m_bSoundOn;
}


int CGeneralManager::GetHighScore(int iGameIdx)
{
	return m_mapHighScore[iGameIdx];
}


void CGeneralManager::SetHighScore(int iGameIdx, int iScore)
{
	int& iHighScore = m_mapHighScore[iGameIdx];
	if (iHighScore < iScore)
	{
		iHighScore = iScore;
	}
}
