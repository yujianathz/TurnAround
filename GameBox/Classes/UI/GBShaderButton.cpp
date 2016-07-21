#include "GBShaderButton.h"
#include "GBShaderEffect.h"

bool GBShaderButton::init(const std::string& framename, const std::string& pressShader)
{
	_pressShader = pressShader;
	_pShaderEffect = NULL;

	if (!GBButton::init(framename))
		return false;

	return true;
}

void GBShaderButton::onPressStateChangedToPressed()
{
	stopAllActions();
	runAction(EaseSineIn::create(ScaleTo::create(0.2f, 0.95f)));
	
	_pShaderEffect = GBShaderEffect::create(_pressShader);
	_pShaderEffect->retain();

	_buttonNormalRenderer->setGLProgramState(_pShaderEffect->getGLProgramState());
}

void GBShaderButton::onPressStateChangedToDisabled()
{
	if (_pShaderEffect)
	{
		_pShaderEffect->release();
		_pShaderEffect = NULL;
	}

	GBButton::onPressStateChangedToDisabled();
}

void GBShaderButton::onPressStateChangedToNormal()
{
	if (_pShaderEffect)
	{
		_pShaderEffect->release();
		_pShaderEffect = NULL;
	}

	GBButton::onPressStateChangedToNormal();
}