#include "GBCsv.h"

vector<vector<string>> GBCsv::parseCsv(const string& csvPath)
{
	Data data = FILE()->getDataFromFile(csvPath);
	vector<vector<string>> v = parseCsv(data.getBytes(), data.getSize());
	data.clear();

	return v;
}

vector<vector<string>> GBCsv::parseCsv(unsigned char * pData, ssize_t size)
{
	vector<vector<string>> v;

	if (pData && size > 0)
	{
		string tmpStr = (char *)pData;
		string fileContent = tmpStr.substr(0, size);

		vector<string> lines;
		lineSplit(lines, fileContent, '\n');
		for (unsigned int i = 0; i < lines.size(); ++i)
		{
			vector<string> fields;
			fieldSplit(fields, lines[i]);
			v.push_back(fields);
		}
	}

	return v;
}

void GBCsv::lineSplit(vector<string>& rows, string& content, const char& rowSeperator)
{
	string::size_type lastIndex = content.find_first_not_of(rowSeperator, 0);
	string::size_type curIndex = content.find_first_of(rowSeperator, lastIndex);

	while (string::npos != curIndex || string::npos != lastIndex)
	{
		rows.push_back(content.substr(lastIndex, curIndex - lastIndex));
		lastIndex = content.find_first_not_of(rowSeperator, curIndex);
		curIndex = content.find_first_of(rowSeperator, lastIndex);
	}
}

void GBCsv::fieldSplit(vector<string>& fields, string line)
{
	if (line[line.length() - 1] == '\r')
		line = line.substr(0, line.length() - 1);

	string field;
	unsigned int i = 0, j = 0;
	while (j < line.length())
	{
		if (line[i] == '"')
			j = getFieldWithQuoted(line, field, i);
		else
			j = getFieldNoQuoted(line, field, i);

		fields.push_back(field);
		i = j + 1;
	}
}

int GBCsv::getFieldWithQuoted(const string& line, string& field, int i)
{
	unsigned int j = 0;
	field = string();
	if (line[i] != '"')
	{
		CCLOGERROR("start char is not quote when call %s", __FUNCTION__);
		return -1;
	}

	for (j = i + 1; j < line.length() - 1; ++j)
	{
		if (line[j] != '"')
			field += line[j];
		else
			return j;
	}

	if (j == line.length())
		CCLOGERROR("resolve the line error:no pair quote, line:%s, filed:%s, start index:%d", line.c_str(), field.c_str(), i);

	return j;
}

int GBCsv::getFieldNoQuoted(const string& line, string& field, int index)
{
	unsigned int j = line.find_first_of(',', index);
	if (j > line.length())
		j = line.length();

	field = string(line, index, j - index);

	return j;
}