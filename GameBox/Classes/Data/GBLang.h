#ifndef _GBLANG_H_
#define _GBLANG_H_

#include "GBDefine.h"

class GBLang
{
public:
	enum
	{
		CHINESE,
	};

	static GBLang *							getInstance();

	void									loadStrings(const std::string& filePath);
	void									unloadStrings();

	const std::string&						getString(int strIndex, int langIndex = CHINESE);

protected:
	vector<vector<string>>					_strings;
};

#endif // _GBLANG_H_