#include "GBShaderEffect.h"

static map<string, string>		s_gFragSources;

GBShaderEffect::GBShaderEffect()
	:_glprogramState(NULL)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
	_backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
		[this](EventCustom*)
	{
		auto glProgram = _glprogramState->getGLProgram();
		glProgram->reset();
		glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, s_gFragSources[_fragFilename].c_str());
		glProgram->link();
		glProgram->updateUniforms();

		CHECK_GL_ERROR_DEBUG();
	}
	);
	DIRECTOR()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}

GBShaderEffect::~GBShaderEffect()
{
	CC_SAFE_RELEASE_NULL(_glprogramState);
#if CC_ENABLE_CACHE_TEXTURE_DATA
	DIRECTOR()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

bool GBShaderEffect::init(const string& fragmentFilename)
{
	if (s_gFragSources.find(fragmentFilename) == s_gFragSources.end())
	{
		auto fragSource = FILE()->getStringFromFile(fragmentFilename);
		s_gFragSources[fragmentFilename] = fragSource;
	}
	
	auto glprogram = GLProgramCache::getInstance()->getGLProgram(fragmentFilename);
	if (!glprogram)
	{
		glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, s_gFragSources[fragmentFilename].c_str());
		GLProgramCache::getInstance()->addGLProgram(glprogram, fragmentFilename);
	}

	_fragFilename = fragmentFilename;

	_glprogramState = GLProgramState::getOrCreateWithGLProgram(glprogram);
	_glprogramState->retain();

	return _glprogramState != NULL;
}