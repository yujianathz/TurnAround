#include "AppDelegate.h"
#include "AppUpdateScene.h"

#include "SimpleAudioEngine.h"
#include "base/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include "lua_module_register.h"

AppDelegate::AppDelegate()
{
	srand((unsigned int)time(NULL));
}

AppDelegate::~AppDelegate()
{

}

bool AppDelegate::applicationDidFinishLaunching()
{
	if (!GBDelegate::applicationDidFinishLaunching())
		return false;

	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		glview = GLViewImpl::createWithRect("TurnAround", Rect(0, 0, 480, 800), 1.0f);
#else
		glview = GLViewImpl::create("TurnAround");
#endif
		director->setOpenGLView(glview);
	}

	Size screenSize = glview->getFrameSize();
	if (screenSize.width > screenSize.height)
	{
		glview->setFrameSize(screenSize.height, screenSize.width);
		screenSize = glview->getFrameSize();
	}
	glview->setDesignResolutionSize(720, screenSize.width * 720 / screenSize.height, ResolutionPolicy::FIXED_WIDTH);

	GBSqlite::initDB("turnaround.db");
    
#ifdef _DEBUG
	FILE()->addSearchPath("../../../src", true);
	FILE()->addSearchPath("../../../res", true);
	AppDelegate::executeScript(false);
#else
	DIRECTOR()->runWithScene(AppUpdateScene::create());
#endif
	
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
	GBDelegate::applicationDidEnterBackground();

	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void AppDelegate::applicationWillEnterForeground()
{
	GBDelegate::applicationWillEnterForeground();

	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}

void AppDelegate::executeScript(bool isUpdate)
{
	// register lua engine
	LuaEngine* engine = LuaEngine::getInstance();
	ScriptEngineManager::getInstance()->setScriptEngine(engine);
	lua_State* L = engine->getLuaStack()->getLuaState();

	lua_module_register(L);

	if (isUpdate)
	{
		const string key = "tianyue-wanpishu";
		const string signment = "wanpishu";
		engine->getLuaStack()->setXXTEAKeyAndSign(key.c_str(), (int)key.length(), signment.c_str(), (int)signment.length());		
		engine->addLuaLoader(gbupdate_lua_loader);
	}	
	//The call was commented because it will lead to ZeroBrane Studio can't find correct context when debugging    
	engine->executeString("require 'main'");
}