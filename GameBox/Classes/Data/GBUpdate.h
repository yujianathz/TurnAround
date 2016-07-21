#ifndef _GBUPDATE_H_
#define _GBUPDATE_H_

#include "GBDefine.h"

#include "HttpClient.h"
#include "CCDownloader.h"

#define GBUPDATE_EVENT_UPDATE_READY				"gbupdate update ready"
#define GBUPDATE_EVENT_UPDATE_START				"gbupdate update start"
#define GBUPDATE_EVENT_UPDATE_FINISH				"gbupdate update finish"
#define GBUPDATE_EVENT_UPDATED_ONE				"gbupdate updated one"

using namespace cocos2d::network;

struct GBUpdateInfo
{
	string key;
	string md5;	
	string version;
	string location;
	unsigned int size;
};

struct GBVersion 
{
	int version1;
	int version2;
	int version3;
};

class GBUpdate
{
public:
	static GBUpdate *						getInstance();

	void									setIsUpdateEnabled(bool isEnabled, const std::string& updateUrl = "");

	string									getLocalVersion();
	int										getDownloadTotalSize();
	int										getDownloadedSize();
	GBUpdateInfo *							getUpdateInfo(const string& key);

	string									getRealPath(const string& path);
	
	CC_SYNTHESIZE_READONLY(string, _remoteVersion, RemoteVersion);
	CC_SYNTHESIZE_READONLY(bool, _isUpdateEnabled, IsUpdateEnabled);
	CC_SYNTHESIZE(bool, _isAutoUpdate, IsAutoUpdate);

protected:
	GBUpdate();

	void									requestRemoteVersion();
	void									onRequestVersionCompleted(HttpClient *sender, HttpResponse *response);
	void									onDownloadSuccess(const DownloadTask& task);
	void									onDownloadError(const DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr);
	void									parseMD5();
	
	map<string, GBUpdateInfo>				_downloadList;
	map<string, GBUpdateInfo>				_updateInfos;

	string									_localVersion;
	string									_localExternalVersion;
	string									_updateUrl;

	std::unique_ptr<network::Downloader>	_downloader;
};

extern "C" {	
	int gbupdate_lua_loader(lua_State * L);
}

#endif // _GBUPDATE_H_