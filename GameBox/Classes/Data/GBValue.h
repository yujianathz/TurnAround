#ifndef  _GB_VALUE_H_
#define  _GB_VALUE_H_

#include "GBDefine.h"

class  GBValue
{
public:
	GBValue();

	GBValue					operator=(bool v);
	GBValue					operator=(int v);
	GBValue					operator=(float v);
	GBValue					operator=(unsigned int v);

	GBValue					operator+=(int v);
	GBValue					operator+=(float v);
	GBValue					operator+=(unsigned int v);

	bool							toBool();
	int							toInt();
	float							toFloat();
	unsigned int				toUInt();

protected:
	static std::string		code(const std::string& str);

	std::string		_value;
};

#endif // _GB_VALUE_H_

