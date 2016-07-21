#ifndef _GBSHADEREFFECT_H_
#define _GBSHADEREFFECT_H_

#include "GBDefine.h"

#undef CLASSNAME
#define CLASSNAME GBShaderEffect

class GBShaderEffect : public Ref
{
public:
	CREATE_WITH_P1(P1(const string&));

	~GBShaderEffect();

	CC_SYNTHESIZE_READONLY(cocos2d::GLProgramState *, _glprogramState, GLProgramState);
protected:
	GBShaderEffect();

	string				_fragFilename;

#if CC_ENABLE_CACHE_TEXTURE_DATA
	EventListener *		_backgroundListener;
#endif
};

#endif