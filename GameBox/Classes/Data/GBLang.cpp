#include "GBLang.h"
#include "GBCsv.h"

static GBLang * s_pGBLang = NULL;

GBLang * GBLang::getInstance()
{
	if (!s_pGBLang)
		s_pGBLang = new GBLang();

	return s_pGBLang;
}

void GBLang::loadStrings(const std::string& filePath)
{
	_strings = GBCsv::parseCsv(filePath);
}

void GBLang::unloadStrings()
{
	for (unsigned int i = 0; i < _strings.size(); i++)
		_strings[i].clear();
	_strings.clear();
}

const std::string& GBLang::getString(int strIndex, int langIndex)
{
	return _strings[strIndex][langIndex];
}