#include "AppUtils.h"

#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "AppUtils_ios.h"
#endif

void AppUtils::openShare(const std::string& shareText, const std::string& shareImage,  const std::string& shareTitle, const std::string& shareUrl)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/turnaround/AppActivity", "openShare", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
		jstring str1 = t.env->NewStringUTF(shareText.c_str());
		jstring str2 = t.env->NewStringUTF(shareImage.c_str());
		jstring str3 = t.env->NewStringUTF(shareTitle.c_str());
		jstring str4 = t.env->NewStringUTF(shareUrl.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, str1, str2, str3, str4);		
		t.env->DeleteLocalRef(str1);
		t.env->DeleteLocalRef(str2);
		t.env->DeleteLocalRef(str3);
		t.env->DeleteLocalRef(str4);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AppUtils_ios::openShare(shareText, shareImage, shareTitle, shareUrl);
#endif
}

void AppUtils::playVideo()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/turnaround/AppActivity", "playVideo", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);		
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

bool AppUtils::isVideoPlayable()
{
	bool ret = false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/turnaround/AppActivity", "isVideoPlayable", "()Z")) {
		ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
	return ret;
}

void AppUtils::showAd1()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/turnaround/AppActivity", "showAd1", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

void AppUtils::showAd2()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/turnaround/AppActivity", "showAd2", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

static int tolua_AppUtils_openShare(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	argc = lua_gettop(tolua_S);
	if (argc == 4)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S, 1, 0, &tolua_err) || !tolua_isstring(tolua_S, 2, 0, &tolua_err) || !tolua_isstring(tolua_S, 3, 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		const std::string shareText = ((const std::string)  tolua_tocppstring(tolua_S, 1, 0));
		const std::string shareImage = ((const std::string) tolua_tocppstring(tolua_S, 2, 0));
		const std::string shareTitle = ((const std::string) tolua_tocppstring(tolua_S, 3, 0));
		const std::string shareUrl = ((const std::string) tolua_tocppstring(tolua_S, 4, 0));		
		AppUtils::openShare(shareText, shareImage, shareTitle, shareUrl);

		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "AppDelegate.openShare", argc, 3);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_AppUtils_openShare'.\n", &tolua_err);
#endif
	return 0;
}

static int tolua_AppUtils_playVideo(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	AppUtils::playVideo();
	return 1;
}

static int tolua_AppUtils_isVideoPlayable(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	bool ret = AppUtils::isVideoPlayable();
	tolua_pushboolean(tolua_S, ret);

	return 1;
}

static int tolua_AppUtils_showAd1(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	AppUtils::showAd1();
	return 1;
}

static int tolua_AppUtils_showAd2(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	AppUtils::showAd2();
	return 1;
}

int register_apputils_module(lua_State* L)
{
	lua_getglobal(L, "_G");
	if (lua_istable(L, -1))//stack:...,_G,
	{
		tolua_open(L);

		tolua_usertype(L, "AppUtils");
		tolua_cclass(L, "AppUtils", "AppUtils", "", nullptr);

		tolua_beginmodule(L, "AppUtils");
		tolua_function(L, "openShare", tolua_AppUtils_openShare);
		tolua_function(L, "playVideo", tolua_AppUtils_playVideo);
		tolua_function(L, "isVideoPlayable", tolua_AppUtils_isVideoPlayable);
		tolua_function(L, "showAd1", tolua_AppUtils_showAd1);
		tolua_function(L, "showAd2", tolua_AppUtils_showAd2);
		tolua_endmodule(L);
		std::string typeName = typeid(AppUtils).name();
		g_luaType[typeName] = "AppUtils";
		g_typeCast["AppUtils"] = "AppUtils";

		tolua_endmodule(L);
	}
	lua_pop(L, 1);

	return 1;
}
