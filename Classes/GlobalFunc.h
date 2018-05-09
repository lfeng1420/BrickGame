#pragma once
#include "GameLogic.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8

#endif

class CGlobalFunc
{
public:
	static const char* GetSpriteNameWithMode(const char* szName)
	{
		static string s_strName;
		s_strName = szName;
		bool bNightMode = GET_BOOLVALUE("NIGHTMODE", false);
		if (bNightMode)
		{
			string::size_type nPos = s_strName.find(".png");
			if (nPos != string::npos)
			{
				s_strName = s_strName.substr(0, nPos) + "_night.png";
			}
		}

		return s_strName.c_str();
	}


	static int GetLanguage()
	{
		LanguageType enLangType = Application::getInstance()->getCurrentLanguage();
		if (enLangType == LanguageType::CHINESE)
		{
			return LANG_ZH;
		}

		return LANG_EN;
	}


	// Get string
	static const char* GetString(int nStrID)
	{
		CDataManager* pDataMgr = CGameLogic::GetInstance()->GetDataManager();
		if (pDataMgr == nullptr)
		{
			return "";
		}

		return pDataMgr->GetString(GetLanguage(), nStrID);
	}


	static void ShortVibrate()
	{
		if (!GET_BOOLVALUE("VIBRATION", false))
		{
			return;
		}

		//log("ShortVibrate");
#if CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
		JniMethodInfo minfo;
		bool bFuncExistFlag = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxHelper", "Vibrate", "(I)V");
		if (bFuncExistFlag)
		{
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, 70);
			minfo.env->DeleteLocalRef(minfo.classID);
		}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
		GLView::sharedOpenGLView()->Vibrate(true);
#endif
	}


	static void LongVibrate()
	{
		if (!GET_BOOLVALUE("VIBRATION", false))
		{
			return;
		}

		//log("LongVibrate");
#if CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
		JniMethodInfo minfo;
		bool bFuncExistFlag = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxHelper", "Vibrate", "(I)V");
		if (bFuncExistFlag)
		{
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, 1000);
			minfo.env->DeleteLocalRef(minfo.classID);
		}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
		GLView::sharedOpenGLView()->Vibrate(false);
#endif
	}


	static void RateApp()
	{
#if CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
		JniMethodInfo minfo;
		bool bFuncExistFlag = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxHelper", "RateApp", "()V");
		if (bFuncExistFlag)
		{
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
			minfo.env->DeleteLocalRef(minfo.classID);
		}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
		GLView::sharedOpenGLView()->RateApp();
#endif
	}


	static void ShowMyApps()
	{
#if CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
		JniMethodInfo minfo;
		bool bFuncExistFlag = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxHelper", "ShowMyApps", "()V");
		if (bFuncExistFlag)
		{
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
			minfo.env->DeleteLocalRef(minfo.classID);
		}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
		GLView::sharedOpenGLView()->ShowMyApps();
#endif
	}


	static void OpenURL(const char* szURL)
	{
#if CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
		JniMethodInfo minfo;
		bool bFuncExistFlag = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxHelper", "OpenURL", "(Ljava/lang/String;)V");
		if (bFuncExistFlag)
		{
			jstring str = minfo.env->NewStringUTF(szURL);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, str);
			minfo.env->DeleteLocalRef(minfo.classID);
			minfo.env->DeleteLocalRef(str);
		}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
		GLView::sharedOpenGLView()->OpenURL();
#endif
	}
};
