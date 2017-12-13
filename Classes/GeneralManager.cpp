#include "GeneralManager.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

CGeneralManager* _dataManager = nullptr;


const string HIGHSCORE_FILEPATH = "./HighScore.json";
const char* STRINGS_FILEPATH = "Strings/strings.json";

const std::string MENU_ITEM_NAME[STRNAME_MAX] = 
{
	"off",
	"on",
	"globalsetting",
	"vibration",
	"sound",
	"nightmode",
	"orientation",
	"portrait",
	"landscape",
	"rhmode",
	"tetrissetting",
	"autorecover",
	"savenow",
	"upbtn",
	"rotate",
	"quickland",
	"other",
	"author",
	"lfeng",
	"rate",
	"rightarrow",
	"quit",
	"recoveron",
	"recoveroff",
	"saveok",
	"savefail",
	"bgmusic",
	"effect",
	"back",
};

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

	LoadStrings();

	//清除之前记录的一些数据
	SET_INTVALUE("GAME", 0);
	SET_INTVALUE("SPEED", 0);
	SET_INTVALUE("LEVEL", 0);
}

//加载动画
bool CGeneralManager::LoadGameAnim()
{
	//加载动画文件
	for (int i = 0; i < GAME_MAX; ++i)
	{
		for (int j = 0; j < GAME_ANIM_NUM; ++j)
		{
			string strFilePath = StringUtils::format("Anims/%c1_%d.txt", i + 'a', j + 1);
			string strFullPath = FileUtils::getInstance()->fullPathForFilename(strFilePath);
			string strContent = FileUtils::getInstance()->getStringFromFile(strFullPath);

			TVECTOR_ANIMDATA& vecAnimData = m_mapGameAnim[i * GAME_ANIM_NUM + j];
			for (unsigned int k = 0; k < strContent.size(); ++k)
			{
				if (strContent[k] == '0')
				{
					vecAnimData.push_back(1);
				}
				else if (strContent[k] == '-')
				{
					vecAnimData.push_back(0);
				}
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
		//log("CGeneralManager::LoadUserData Load 'oDoc' Error.");
		return false;
	}

	for (int i = 0; i < oDoc["HighScore"].Size(); i++)
	{
		if (oDoc["HighScore"][i].IsNull())
		{
			//log("CGeneralManager::LoadUserData Load 'oDoc[HighScore][%d]' Error.", i);
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
		//log("Open File '%s' Failed.", strPath.c_str());
		return false;
	}

	fputs(oBuffer.GetString(), pFile);
	fclose(pFile);

	return true;
}


const char* CGeneralManager::GetStringByID(int nLangID, int nStrID)
{
	TMAP_LANGUAGESTRING::iterator itLangString = m_mapStrings.find(nLangID);
	if (itLangString == m_mapStrings.end())
	{
		return "";
	}

	TMAP_STRING& mapString = itLangString->second;
	TMAP_STRING::iterator itString = mapString.find(nStrID);
	if (itString == mapString.end())
	{
		return "";
	}

	return itString->second.c_str();
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


void CGeneralManager::LoadStrings()
{
	std::string strContent = FileUtils::getInstance()->getStringFromFile(STRINGS_FILEPATH);
	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (oDoc.HasParseError() || oDoc.IsNull())
	{
		return;
	}

	for (int nIdx = STRNAME_MIN; nIdx < STRNAME_MAX; ++nIdx)
	{
		std::string strChName("ch_" + MENU_ITEM_NAME[nIdx]);
		std::string strEnName("en_" + MENU_ITEM_NAME[nIdx]);
		m_mapStrings[LANG_CH][nIdx] = oDoc[strChName.c_str()].GetString();
		m_mapStrings[LANG_ENG][nIdx] = oDoc[strEnName.c_str()].GetString();
	}
}


vector<int>* CGeneralManager::GetAnimData(int iGameIndex, int iAnimIndex)
{
	int iIndex = iGameIndex * GAME_ANIM_NUM + iAnimIndex;
	return &m_mapGameAnim[iIndex];
}


//播放背景音乐
void CGeneralManager::PlayMusic(const char* strName, bool bLoop)
{
	if (GET_BOOLVALUE("SOUND", true))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(strName, bLoop);
	}
}

//播放音效
void CGeneralManager::PlayEffect(const char* strName)
{
	if (GET_BOOLVALUE("SOUND", true))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(strName);
	}
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


void CGeneralManager::SaveTetrisData(const bool(&arrState)[ROW_NUM][COLUMN_NUM])
{
	string strPath = FileUtils::getInstance()->getWritablePath() + "/record.txt";
	fstream oFile(strPath.c_str(), ios::out | ios::binary);
	if (!oFile.is_open())
	{
		//log("Open %s Error.", strPath.c_str());
		return;
	}

	for (int i = 0; i < ROW_NUM; ++i)
	{
		for (int j = 0; j < COLUMN_NUM; ++j)
		{
			oFile << arrState[i][j];
		}
	}

	oFile.close();
}


void CGeneralManager::LoadTetrisData(bool(&arrState)[ROW_NUM][COLUMN_NUM])
{
	string strPath = FileUtils::getInstance()->getWritablePath() + "/record.txt";
	//log("%s", strPath.c_str());

	fstream oFile(strPath.c_str(), ios::in | ios::binary);
	if (!oFile.is_open())
	{
		//log("Open %s Error.", strPath.c_str());
		return;
	}

	int iCount = 0;
	while (!oFile.eof())
	{
		char ch = '\0';
		oFile.get(ch);

		int iRowIdx = iCount / COLUMN_NUM;
		int iColIdx = iCount - iRowIdx * COLUMN_NUM;
		arrState[iRowIdx][iColIdx] = ch - '0';
		++iCount;
	}

	oFile.close();
}


std::string CGeneralManager::GetSpriteName(const string& strName, bool bNightMode)
{
	int nIndex = strName.find(bNightMode ? ".png" : "_night");
	if (nIndex != string::npos)
	{
		return strName.substr(0, nIndex) + (bNightMode ? "_night.png" : ".png");
	}

	return strName;
}


bool CGeneralManager::CheckAndroidNavBarExist()
{
#if CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
	JniMethodInfo t;
	bool bFuncExistFlag = JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "IsNavBarExist", "()Z");
	if (bFuncExistFlag)
	{
		jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		return ret;
	}

	return false;
#endif

	return false;
}
