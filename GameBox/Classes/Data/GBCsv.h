#ifndef _GBCSV_H_
#define _GBCSV_H_

#include "GBDefine.h"

class GBCsv
{
public:
	static vector<vector<string>>			parseCsv(const string& csvPath);
	static vector<vector<string>>			parseCsv(unsigned char * pData, ssize_t size);

protected:
	static void								lineSplit(vector<string>& rows, string& content, const char& rowSeperator);
	static void								fieldSplit(vector<string>& fields, string line);

	static int								getFieldWithQuoted(const string& line, string& field, int i);
	static int								getFieldNoQuoted(const string& line, string& field, int index);
};

#endif // _GBLANG_H_