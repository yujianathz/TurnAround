#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "GBDefine.h"
#include "GBDelegate.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private GBDelegate
{
public:
	AppDelegate();
	virtual ~AppDelegate();

	virtual bool applicationDidFinishLaunching();

	virtual void applicationDidEnterBackground();
	virtual void applicationWillEnterForeground();

	static void executeScript(bool isUpdate);

protected:
	void initShareSDKConfig();
};

#endif // _APP_DELEGATE_H_

