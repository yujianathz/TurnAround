#ifndef  _APP_UTILS_H_
#define  _APP_UTILS_H_

#include "GBDefine.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

class  AppUtils
{
public:	
	static void openShare(const std::string& shareText, const std::string& shareImage, const std::string& shareTitle, const std::string& shareUrl);
	static void playVideo();
	static bool isVideoPlayable();
	static void showAd1();
	static void showAd2();
};

TOLUA_API int register_apputils_module(lua_State* L);

#endif // _APP_UTILS_H_

