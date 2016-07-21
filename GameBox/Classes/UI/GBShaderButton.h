#ifndef  _GB_SHADER_BUTTON_H_
#define  _GB_SHADER_BUTTON_H_

#include "GBButton.h"
#include "GBShaderEffect.h"
#include "GBDefine.h"

#undef CLASSNAME
#define CLASSNAME GBShaderButton

class GBShaderButton : public GBButton
{
public:
	CREATE_WITH_P2(P1(const std::string&), P2(const std::string&));

protected:
	virtual void			onPressStateChangedToPressed() override;
	virtual void			onPressStateChangedToDisabled() override;
	virtual void			onPressStateChangedToNormal() override;

	GBShaderEffect *		_pShaderEffect;

	std::string				_pressShader;
};

#endif // _GB_SHADER_BUTTON_H_

