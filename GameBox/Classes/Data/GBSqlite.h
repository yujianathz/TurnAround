#ifndef _GBSQLITE_H_
#define _GBSQLITE_H_

#include "GBDefine.h"

class GBSqlite
{
public:
	static void				setSecret(const std::string& secret);
	static void				initDB(const std::string& dbName);
	static void				freeDB();

	static bool				getBoolForKey(const char* key, bool defaultValue);
	static int				getIntegerForKey(const char* key, int defaultValue);
	static float			getFloatForKey(const char* key, float defaultValue);	
	static std::string		getStringForKey(const char* key, const std::string & defaultValue);

	static void				setBoolForKey(const char* key, bool value);
	static void				setIntegerForKey(const char* key, int value);
	static void				setFloatForKey(const char* key, float value);
	static void				setStringForKey(const char* key, const std::string & value);
};

#endif // _GBSQLITE_H_