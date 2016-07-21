#include "GBUtils.h"
#include "GBDefine.h"

#ifdef USE_ANYSDK
#include "GBAnySDK.h"
#endif

#include <jni.h>

std::vector<std::string> GBUtils::readDirectory(const std::string& relativePath, const std::string keyWord)
{
	std::vector<std::string> v;
	return v;
}

std::string GBUtils::coding(const std::string &str,const char* toCode,const char* formCode)
{
	std::string temp = str;
	return temp;  
}

void GBUtils::openURL(std::string &url)
{

}

std::string GBUtils::getChannel()
{
	std::string channelId = "wanpishu";
	
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/gamebox/GBActivity", "getChannelId", "()Ljava/lang/String;")) {
	    jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
	    channelId = StringUtils::getStringUTFCharsJNI(t.env, str);
	    
	    t.env->DeleteLocalRef(t.classID);
	}
	
	return channelId;
}

std::string GBUtils::getDeviceId()
{
	std::string deviceId = "";
	
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/gamebox/GBActivity", "getDeviceId", "()Ljava/lang/String;")) {
	    jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
	    deviceId = StringUtils::getStringUTFCharsJNI(t.env, str);
	    
	    t.env->DeleteLocalRef(t.classID);
	}
	
	return deviceId;
}

void GBUtils::vibration(float duration)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/gamebox/GBActivity", "vibration", "(F)V")) {
	    t.env->CallStaticVoidMethod(t.classID, t.methodID, duration);
	    t.env->DeleteLocalRef(t.classID);
	}		
}

void GBUtils::purchase(const string& payInfo)
{
#ifdef USE_ANYSDK
	GBAnySDK::getInstance()->pay(payInfo);
#else
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/gamebox/GBActivity", "purchase", "(Ljava/lang/String;)V")) {
			jstring str = t.env->NewStringUTF(payInfo.c_str());
	    t.env->CallStaticVoidMethod(t.classID, t.methodID, str);
	    t.env->DeleteLocalRef(str);
	    t.env->DeleteLocalRef(t.classID);
	}	
#endif	
}

std::string GBUtils::getOpenUDID()
{
	std::string udid = "";
	
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/wanpishu/gamebox/GBActivity", "getOpenUDID", "()Ljava/lang/String;")) {			
	    jstring ret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
	    udid = StringUtils::getStringUTFCharsJNI(t.env, ret);	    
	    t.env->DeleteLocalRef(t.classID);
	}
	
	return udid;	
}
