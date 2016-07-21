#ifndef _GBANYSDK_H_
#define _GBANYSDK_H_

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "AgentManager.h"
#include "platform/android/jni/JniHelper.h"

using namespace anysdk::framework;
#endif

using namespace std;

#define ANYSDK_EVENT_INIT_SUCCESS						"init_success"
#define ANYSDK_EVENT_INIT_FAIL							"init_fail"
#define ANYSDK_EVENT_LOGIN_SUCCESS					"login_success"
#define ANYSDK_EVENT_LOGIN_FAIL							"login_fail"
#define ANYSDK_EVENT_LOGOUT_SUCCESS					"logout_success"
#define ANYSDK_EVENT_LOGOUT_FAIL						"logout_fail"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
class GBAnySDK : public UserActionListener, PayResultListener, AdsListener
#else
class GBAnySDK
#endif
{
public:
	static GBAnySDK * getInstance();
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	static void				setJavaVM(JavaVM* vm);
#endif
	
	void							initSDK(const string& appKey, const string& appSecret, const string& privateKey, const string& oauthLoginServer);
	void							login();
	void							logout();
	void							showToolBar();
	void							hideToolBar();
	void							exit();
	bool							isLogined();
	void							pay(const std::string& productId);
	void							preloadAds(AdsType adType);
	void							showAds(AdsType adType);
	void							hideAds(AdsType adType);
	void							startSession();
	void							stopSession();
	
	virtual void 			onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg);
	virtual void 			onPayResult(PayResultCode ret, const char* msg, TProductInfo info);
	virtual void			onAdsResult(AdsResultCode code, const char* msg);
	
	CC_SYNTHESIZE_READONLY(bool, _isInited, IsInited);
	
protected:
	GBAnySDK();
	
	void 							initSuccessCallback();
	void 							initFailCallback();
	void 							loginSuccessCallback();
	void 							loginFailCallback();
	void 							logoutSuccessCallback();
	void 							logoutFailCallback();
	
	int								getAdsType(int type);
};

#endif // _GBANYSDK_H_