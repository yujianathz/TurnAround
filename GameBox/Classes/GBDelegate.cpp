/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2015 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GBDelegate.h"
#include "GBSqlite.h"
#include "AudioEngine.h"

using namespace cocos2d;
using namespace cocos2d::experimental;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <jni.h>

void cocos_android_app_init(JNIEnv* env) __attribute__((weak));
	
extern "C"
{
	JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
	{
		JniHelper::setJavaVM(vm);
		cocos_android_app_init(JniHelper::getEnv());

		return JNI_VERSION_1_4;
	}
}

#endif

GBDelegate::GBDelegate()
{
}

GBDelegate::~GBDelegate()
{
	GBSqlite::freeDB();

	AudioEngine::stopAll();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void GBDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool GBDelegate::applicationDidFinishLaunching()
{
    // As an example, load config file
    // FIXME:: This should be loaded before the Director is initialized,
    // FIXME:: but at this point, the director is already initialized
	GBSqlite::setSecret("wanpishu");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	FileUtils::getInstance()->addSearchPath("../../Resources", true);
#endif	

#if COCOS2D_DEBUG > 0
	DIRECTOR()->setDisplayStats(true);
#endif
	DIRECTOR()->setAnimationInterval(1.0f / 60);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void GBDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
	AudioEngine::pauseAll();
}

// this function will be called when the app is active again
void GBDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
	AudioEngine::resumeAll();
}
