#include "GBButton.h"
#include "AudioEngine.h"

#if CC_ENABLE_SCRIPT_BINDING
#include "CCLuaEngine.h"
#endif

using namespace cocos2d::experimental;

bool GBButton::init(const std::string& framename)
{
	if (!Button::init(framename, "", "", TextureResType::PLIST))
		return false;

	setCascadeColorEnabled(true);

	return true;
}

void GBButton::onPressStateChangedToNormal()
{
	stopActionByTag(ACTION_TAG_PRESS);
	auto action = EaseSineOut::create(ScaleTo::create(0.2f, 1.0f));
	action->setTag(ACTION_TAG_PRESS);
	runAction(action);
	setColor(Color3B::WHITE);

	_buttonNormalRenderer->setState(Scale9Sprite::State::NORMAL);
}

void GBButton::onPressStateChangedToPressed()
{
	stopActionByTag(ACTION_TAG_PRESS);
	auto action = EaseSineIn::create(ScaleTo::create(0.2f, 0.95f));
	action->setTag(ACTION_TAG_PRESS);
	runAction(action);
	setColor(Color3B::GRAY);
	
	_buttonNormalRenderer->setState(Scale9Sprite::State::NORMAL);
}

void GBButton::onPressStateChangedToDisabled()
{
	_buttonNormalRenderer->setState(Scale9Sprite::State::GRAY);
}

void GBButton::releaseUpEvent()
{
	DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBBUTTON_RELEASE_UP, this);

/*#if CC_ENABLE_SCRIPT_BINDING
	int handler = ScriptHandlerMgr::getInstance()->getObjectHandler(this, ScriptHandlerMgr::HandlerType::NODE);
	if (0 != handler)
	{
		LuaStack * pStack = LuaEngine::getInstance()->getLuaStack();
		pStack->pushString("releaseUpEvent");
		pStack->executeFunctionByHandler(handler, 1);
		pStack->clean();
	}
#endif*/
	Button::releaseUpEvent();
}