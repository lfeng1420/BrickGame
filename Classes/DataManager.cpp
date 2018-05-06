#include "stdafx.h"
#include "DataManager.h"
#include "GameLogic.h"

const char* HIGHSCORE_FILEPATH = "./HighScore.json";
const char* STRINGS_FILEPATH = "Strings/strings.json";


bool CDataManager::Create()
{
	if (!__LoadGameAnim()
		|| !__LoadHighScore()
		|| !__LoadStrings())
	{
		return false;
	}

	return true;
}


bool CDataManager::GetGameAnimData(int nGameID, bool* arrAnimData, int nFrameIdx)
{
	TMap_GameAnimData::const_iterator itFrameAnimData = m_mapGameAnimData.find(nGameID);
	if (itFrameAnimData == m_mapGameAnimData.cend())
	{
		return false;
	}

	const TMap_FrameAnimData& mapFrameAnimData = itFrameAnimData->second;
	TMap_FrameAnimData::const_iterator itAnimData = mapFrameAnimData.find(nFrameIdx);
	if (itAnimData == mapFrameAnimData.cend())
	{
		return false;
	}

	memcpy(arrAnimData, itAnimData->second.arrBrickState, ROW_COUNT * COLUMN_COUNT * sizeof(bool));
	return true;
}


bool CDataManager::SaveHighScore()
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

		for (int nGameID = 0; nGameID < GAMEID_MAX; ++nGameID)
		{
			oArray.PushBack(m_arrHighScore[nGameID], allocator);
		}
		oDoc.AddMember("HighScore", oArray, allocator);
	}
	else
	{
		for (rapidjson::SizeType nGameID = 0; nGameID < oDoc["HighScore"].Capacity(); ++nGameID)
		{
			if (oDoc["HighScore"][nGameID].IsNull())
			{
				return false;
			}
			oDoc["HighScore"][nGameID].SetInt(m_arrHighScore[nGameID]);
		}
	}

	rapidjson::StringBuffer oBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> oWriter(oBuffer);
	oDoc.Accept(oWriter);

	FILE* pFile = fopen(strPath.c_str(), "wb+");
	if (pFile == NULL)
	{
		return false;
	}

	fputs(oBuffer.GetString(), pFile);
	fclose(pFile);

	return true;
}


int CDataManager::GetHighScore(int nGameID)
{
	if (nGameID < 0 || nGameID >= GAMEID_MAX)
	{
		return 0;
	}

	return m_arrHighScore[nGameID];
}


void CDataManager::SetHighScore(int nGameID, int nHighScore)
{
	if (nGameID < 0 || nGameID >= GAMEID_MAX
		|| m_arrHighScore[nGameID] >= nHighScore)
	{
		return;
	}

	m_arrHighScore[nGameID] = nHighScore;
	// Delay write record
	CGameLogic::GetInstance()->GetTimerManager()->SetTimer(this, enTimerID_SaveData, enInterval_SaveData, 1, __FUNCTION__);
}


const char* CDataManager::GetString(int nLangID, int nStrID)
{
	if (nLangID < LANG_MIN || nLangID >= LANG_MAX
		|| nStrID < STRID_MIN || nStrID >= STRID_MAX)
	{
		return "";
	}

	return m_arrStrings[nStrID][nLangID].c_str();
}


bool CDataManager::SaveTetrisData(const char* szPath, bool* arrBricksData, int nCurShapeID, int nNextShapeID, int nCurScore)
{
	string strPath = FileUtils::getInstance()->getWritablePath();
	strPath.append(szPath);
	string strContent = FileUtils::getInstance()->getStringFromFile(strPath);
	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (oDoc.HasParseError() || oDoc.IsNull())
	{
		oDoc.SetObject();
		rapidjson::Document::AllocatorType& allocator = oDoc.GetAllocator();

		// shape id
		oDoc.AddMember("CurShape", nCurShapeID, allocator);
		oDoc.AddMember("NextShape", nNextShapeID, allocator);

		// score
		oDoc.AddMember("CurScore", nCurScore, allocator);

		// data
		rapidjson::Value oArray(rapidjson::kArrayType);
		for (int nBrickID = 0; nBrickID < ROW_COUNT * COLUMN_COUNT; ++nBrickID)
		{
			oArray.PushBack(arrBricksData[nBrickID], allocator);
		}
		oDoc.AddMember("data", oArray, allocator);
	}
	else
	{
		// Current shape id
		if (oDoc["CurShape"].IsNull())
		{
			return false;
		}
		oDoc["CurShape"].SetInt(nCurShapeID);

		// Next shape id
		if (oDoc["NextShape"].IsNull())
		{
			return false;
		}
		oDoc["NextShape"].SetInt(nNextShapeID);

		// Score
		if (oDoc["CurScore"].IsNull())
		{
			return false;
		}
		oDoc["CurScore"].SetInt(nCurScore);

		// data
		if (oDoc["data"].IsNull())
		{
			return false;
		}
		for (int nBrickID = 0; nBrickID < ROW_COUNT * COLUMN_COUNT; ++nBrickID)
		{
			if (oDoc["data"][nBrickID].IsNull())
			{
				return false;
			}
			oDoc["data"][nBrickID].SetBool(arrBricksData[nBrickID]);
		}
	}

	rapidjson::StringBuffer oBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> oWriter(oBuffer);
	oDoc.Accept(oWriter);

	FILE* pFile = fopen(strPath.c_str(), "wb+");
	if (pFile == NULL)
	{
		return false;
	}

	fputs(oBuffer.GetString(), pFile);
	fclose(pFile);

	return true;
}


bool CDataManager::LoadTetrisData(const char* szPath, bool* arrBricksData, int& nCurShapeID, int& nNextShapeID, int& nCurScore)
{
	string strPath = FileUtils::getInstance()->getWritablePath();
	strPath.append(szPath);
	string strContent = FileUtils::getInstance()->getStringFromFile(strPath);
	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (oDoc.HasParseError()
		|| oDoc.IsNull())
	{
		return false;
	}

	// Current shape id
	if (oDoc["CurShape"].IsNull())
	{
		return false;
	}
	nCurShapeID = oDoc["CurShape"].GetInt();

	// Next shape id
	if (oDoc["NextShape"].IsNull())
	{
		return false;
	}
	nNextShapeID = oDoc["NextShape"].GetInt();

	// Score
	if (oDoc["CurScore"].IsNull())
	{
		return false;
	}
	nCurScore = oDoc["CurScore"].GetInt();

	// Data
	for (unsigned int uiBrickID = 0; uiBrickID < oDoc["data"].Size() && uiBrickID < ROW_COUNT * COLUMN_COUNT; ++uiBrickID)
	{
		if (oDoc["data"][uiBrickID].IsNull())
		{
			return false;
		}

		arrBricksData[uiBrickID] = oDoc["data"][uiBrickID].GetBool();
	}

	return true;
}


void CDataManager::OnTimer(int nTimerID)
{
	if (nTimerID != enTimerID_SaveData)
	{
		return;
	}

	SaveHighScore();
}


bool CDataManager::__LoadGameAnim()
{
    for (int nGameID = GAMEID_IDMIN; nGameID < GAMEID_MAX; ++nGameID)
	{
		TMap_FrameAnimData& mapFrameAnimData = m_mapGameAnimData[nGameID];
		for (int nAnimIdx = 0; nAnimIdx < GAMEID_ANIM_COUNT; ++nAnimIdx)
		{
			_TAnimData& stAnimData = mapFrameAnimData[nAnimIdx];
			string strFilePath = StringUtils::format("Anims/%c1_%d.txt", (nGameID - GAMEID_IDMIN) + 'a', nAnimIdx + 1);
			string strFullPath = FileUtils::getInstance()->fullPathForFilename(strFilePath);
			string strContent = FileUtils::getInstance()->getStringFromFile(strFullPath);

			int nBrickID = 0;
			for (unsigned int uiIdx = 0; uiIdx < strContent.size(); ++uiIdx)
			{
				if (strContent[uiIdx] == '0')
				{
					stAnimData.arrBrickState[nBrickID] = true;
					++nBrickID;
				}
				else if (strContent[uiIdx] == '-')
				{
					stAnimData.arrBrickState[nBrickID] = false;
					++nBrickID;
				}
			}
		}
	}

	return true;
}


bool CDataManager::__LoadHighScore()
{
	memset(m_arrHighScore, 0, sizeof(m_arrHighScore));

	string strPath = FileUtils::getInstance()->getWritablePath() + string(HIGHSCORE_FILEPATH);
	string strContent = FileUtils::getInstance()->getStringFromFile(strPath);
	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (!oDoc.HasParseError() 
		&& !oDoc.IsNull()
		&& !oDoc["HighScore"].IsNull())
	{
		for (unsigned int nGameID = 0; nGameID < oDoc["HighScore"].Size(); ++nGameID)
		{
			if (oDoc["HighScore"][nGameID].IsNull())
			{
				return false;
			}

			m_arrHighScore[nGameID] = oDoc["HighScore"][nGameID].GetInt();
		}
	}

	return true;
}


bool CDataManager::__LoadStrings()
{
	string strContent = FileUtils::getInstance()->getStringFromFile(STRINGS_FILEPATH);
	rapidjson::Document oDoc;
	oDoc.Parse<0>(strContent.c_str());
	if (oDoc.HasParseError() || oDoc.IsNull())
	{
		return false;
	}

	// zh
	if (oDoc["zh"].IsNull())
	{
		return false;
	}
	for (unsigned int nStrID = 0; nStrID < oDoc["zh"].Size(); ++nStrID)
	{
		if (oDoc["zh"][nStrID].IsNull())
		{
			return false;
		}

		m_arrStrings[nStrID][LANG_ZH] = oDoc["zh"][nStrID].GetString();
	}

	// en
	if (oDoc["en"].IsNull())
	{
		return false;
	}
	for (unsigned int nStrID = 0; nStrID < oDoc["en"].Size(); ++nStrID)
	{
		if (oDoc["en"][nStrID].IsNull())
		{
			return false;
		}

		m_arrStrings[nStrID][LANG_EN] = oDoc["en"][nStrID].GetString();
	}

	return true;
}

