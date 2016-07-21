#include "GBSqlite.h"
#include "LocalStorage.h"

void GBSqlite::setSecret(const std::string& secret)
{
	localStorageSetSecret(secret);
}

void GBSqlite::initDB(const std::string& dbName)
{
	localStorageInit(FILE()->getWritablePath() + dbName);
}

void GBSqlite::freeDB()
{
	localStorageFree();
}

bool GBSqlite::getBoolForKey(const char* key, bool defaultValue)
{
	std::string outItem;
	if (localStorageGetItem(key, &outItem))
	{
		if (outItem == "true")
			return true;
		else if (outItem == "false")
			return false;
	}
	
	return defaultValue;
}

int GBSqlite::getIntegerForKey(const char* key, int defaultValue)
{
	std::string outItem;
	if (localStorageGetItem(key, &outItem))
	{
		if (!outItem.empty())
			return atoi(outItem.c_str());
	}

	return defaultValue;
}

float GBSqlite::getFloatForKey(const char* key, float defaultValue)
{
	std::string outItem;
	if (localStorageGetItem(key, &outItem))
	{
		if (!outItem.empty())
			return atof(outItem.c_str());
	}

	return defaultValue;
}

std::string GBSqlite::getStringForKey(const char* key, const std::string & defaultValue)
{
	std::string outItem;
	if (localStorageGetItem(key, &outItem))
		return outItem;

	return defaultValue;
}

void GBSqlite::setBoolForKey(const char* key, bool value)
{
	localStorageSetItem(key, value ? "true" : "false");
}

void GBSqlite::setIntegerForKey(const char* key, int value)
{
	localStorageSetItem(key, StringUtils::format("%d", value));
}

void GBSqlite::setFloatForKey(const char* key, float value)
{
	localStorageSetItem(key, StringUtils::format("%f", value));
}

void GBSqlite::setStringForKey(const char* key, const std::string & value)
{
	localStorageSetItem(key, value);
}