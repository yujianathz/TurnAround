#include "GBValue.h"

const static char SECRET[] = { 'W', 'P', 'S', 'G', 'A', 'M', 'E' };

GBValue::GBValue()
{
	_value = "";
}

GBValue GBValue::operator=(bool v)
{
	_value = GBValue::code(StringUtils::format("%s", v ? "true" : "false"));

	return *this;
}

GBValue GBValue::operator=(int v)
{
	_value = GBValue::code(StringUtils::format("%d", v));

	return *this;
}

GBValue GBValue::operator=(float v)
{
	_value = GBValue::code(StringUtils::format("%f", v));

	return *this;
}

GBValue GBValue::operator=(unsigned int v)
{
	_value = GBValue::code(StringUtils::format("%u", v));

	return *this;
}

GBValue GBValue::operator+=(int v)
{
	_value = GBValue::code(StringUtils::format("%d", toInt() + v));

	return *this;
}

GBValue GBValue::operator+=(float v)
{
	_value = GBValue::code(StringUtils::format("%f", toFloat() + v));

	return *this;
}

bool GBValue::toBool()
{
	return GBValue::code(_value) == "true";
}

int GBValue::toInt()
{
	return atoi(GBValue::code(_value).c_str());
}

float GBValue::toFloat()
{
	return atof(GBValue::code(_value).c_str());
}

unsigned int GBValue::toUInt()
{
	return (unsigned int)atoi(GBValue::code(_value).c_str());
}

std::string GBValue::code(const std::string& str)
{
	std::string temp = str;
	for (unsigned int i = 0; i < temp.length(); i++)
	{
		int ch = temp[i]^SECRET[i % 7];
		temp[i] = ch;
	}

	return temp;
}