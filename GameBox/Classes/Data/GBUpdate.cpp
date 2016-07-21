#include "GBUpdate.h"
#include "GBCsv.h"
#include "GBUtils.h"

#include "MD5/md5.h"

#include "CCLuaStack.h"
#include "CCLuaEngine.h"

#define MIN_DOWNLOAD_SIZE						(10 * 1024)

static GBUpdate * s_pUpdate = NULL;

static GBVersion splitVersion(const string& version)
{
	ssize_t pos1 = version.find_first_of('.');
	ssize_t pos2 = version.find_last_of('.');

	if (pos1 == std::string::npos || pos2 == std::string::npos)
		return { 1, 0, 0 };
	
	GBVersion v;
	v.version1 = atoi(version.substr(0, pos1).c_str());
	v.version2 = atoi(version.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
	v.version3 = atoi(version.substr(pos2 + 1).c_str());

	return v;
}

static bool isVersionValid(const string& version, const string& base)
{
	GBVersion v = splitVersion(version);
	GBVersion b = splitVersion(base);

	if (v.version1 == b.version1)
	{
		if (v.version2 == b.version2)
			return v.version3 > b.version3;
		else
			return v.version2 > b.version2;
	}

	return false;
}

GBUpdate::GBUpdate()
{
	_isUpdateEnabled = false;
	_isAutoUpdate = false;
	_remoteVersion = "";
	_localVersion = "";
	_localExternalVersion = "";
	
	_downloader.reset(new network::Downloader());
	_downloader->onFileTaskSuccess = [this](const DownloadTask& task) { onDownloadSuccess(task); };
	_downloader->onTaskError = [this](const DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr) {onDownloadError(task, errorCode, errorCodeInternal, errorStr); };
	
	DIRECTOR()->getEventDispatcher()->addCustomEventListener(GBUPDATE_EVENT_UPDATE_START, [this](EventCustom * pEvent){
		for (map<string, GBUpdateInfo>::iterator it = _downloadList.begin(); it != _downloadList.end(); it++)
		{
			if (it->second.location.empty())
			{
				GBVersion version = splitVersion(_remoteVersion);
				_downloader->createDownloadFileTask(StringUtils::format("%s/%d.%d/%s", _updateUrl.c_str(), version.version1, version.version2, it->first.c_str()), GBUtils::getExternalPath(it->first), it->first);
				return;
			}
		}
		DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_FINISH, (void *)0);
	});

	DIRECTOR()->getEventDispatcher()->addCustomEventListener(GBUPDATE_EVENT_UPDATED_ONE, [this](EventCustom * pEvent){
		for (map<string, GBUpdateInfo>::iterator it = _downloadList.begin(); it != _downloadList.end(); it++)
		{
			if (it->second.location.empty())
			{
				GBVersion version = splitVersion(_remoteVersion);
				_downloader->createDownloadFileTask(StringUtils::format("%s/%d.%d/%s", _updateUrl.c_str(), version.version1, version.version2, it->first.c_str()), GBUtils::getExternalPath(it->first), it->first);
				return;
			}
		}
		DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_FINISH, (void *)1);
	});

	DIRECTOR()->getEventDispatcher()->addCustomEventListener(GBUPDATE_EVENT_UPDATE_FINISH, [this](EventCustom * pEvent){
		long isDownloadFinish = (long)pEvent->getUserData();
		if(isDownloadFinish) parseMD5();
	});
}

GBUpdate * GBUpdate::getInstance()
{
	if (!s_pUpdate)
		s_pUpdate = new GBUpdate();

	return s_pUpdate;
}

void GBUpdate::setIsUpdateEnabled(bool isEnabled, const std::string& updateUrl)
{
	_isUpdateEnabled = isEnabled;
	_updateUrl = updateUrl;

	parseMD5();

	if (_isUpdateEnabled)
		requestRemoteVersion();
	else
		DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_FINISH, (void *)0);
}

string GBUpdate::getLocalVersion()
{
	if (_localExternalVersion.empty() && _localVersion.empty())
		return "1.0.0";

	return isVersionValid(_localExternalVersion, _localVersion) ? _localExternalVersion : _localVersion;
}

int GBUpdate::getDownloadTotalSize()
{
	int totalSize = 0;
	for (map<string, GBUpdateInfo>::iterator it = _downloadList.begin(); it != _downloadList.end(); it++)
		totalSize += it->second.size;

	return totalSize;
}

int GBUpdate::getDownloadedSize()
{
	int downloadedSize = 0;
	for (map<string, GBUpdateInfo>::iterator it = _downloadList.begin(); it != _downloadList.end(); it++)
	{
		if (!it->second.location.empty())
			downloadedSize += it->second.size;
	}		

	return downloadedSize;
}

GBUpdateInfo * GBUpdate::getUpdateInfo(const string& key)
{
	if (_updateInfos.find(key) == _updateInfos.end())
		return NULL;

	return &_updateInfos[key];
}

void GBUpdate::requestRemoteVersion()
{
	GBVersion version = splitVersion(getLocalVersion());

	auto request = new (std::nothrow) HttpRequest();
	request->setUrl(StringUtils::format("%s/%d.%d/md5", _updateUrl.c_str(), version.version1, version.version2).c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(GBUpdate::onRequestVersionCompleted, this));
	HttpClient::getInstance()->sendImmediate(request);
	request->release();
}

void GBUpdate::onRequestVersionCompleted(HttpClient *sender, HttpResponse *response)
{
	do 
	{
		if (!response)
			break;
		if (!response->isSucceed())
			break;
		vector<char> * pBuf = response->getResponseData();
		if (pBuf->size() > 0)
		{
			vector<vector<string>> v = GBCsv::parseCsv((unsigned char *)pBuf->data(), pBuf->size());
			_remoteVersion = v[0][0];
		}

		if (!_remoteVersion.empty())
		{
			string localVersion = getLocalVersion();
			if (_remoteVersion == localVersion)
			{
				if (getDownloadTotalSize() > MIN_DOWNLOAD_SIZE && !_isAutoUpdate)
				{
					DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_READY);
				}
				else
				{
					DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_START);
				}				
				return;
			}
			else if (isVersionValid(_remoteVersion, getLocalVersion()))
			{
				GBVersion version = splitVersion(_remoteVersion);
				string md5Url = StringUtils::format("%s/%d.%d/md5", _updateUrl.c_str(), version.version1, version.version2);				
				_downloader->createDownloadFileTask(md5Url, GBUtils::getExternalPath("md5"), "md5");
				return;
			}
		}
	} while (false);
	DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_FINISH, (void *)0);
}

void GBUpdate::onDownloadSuccess(const DownloadTask& task)
{
	string url = task.requestURL;
	string path = task.storagePath;
	string custom = task.identifier;
	if (custom == "md5")
	{		
		_downloadList.clear();
		vector<vector<string>> v = GBCsv::parseCsv(GBUtils::getExternalPath(custom));
		for (unsigned int i = 1; i < v.size(); i++)
		{
			string key = v[i][0];
			string md5 = v[i][1];
			unsigned int size = atoi(v[i][2].c_str());
			if (_updateInfos.find(key) == _updateInfos.end() || _updateInfos[key].md5 != md5)
			{
				GBUpdateInfo info = { key, md5, _remoteVersion, "", size };
				_downloadList[key] = info;				
			}
		}
		if (getDownloadTotalSize() > MIN_DOWNLOAD_SIZE && !_isAutoUpdate)
		{
			DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_READY);
		}
		else
		{
			DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_START);
		}		
	}
	else
	{
		if (_downloadList.find(custom) != _downloadList.end())
		{
			Data data = FILE()->getDataFromFile(path);
			if (data.getBytes())
			{
				auto md5 = new MD5(data.getBytes(), data.getSize());
				if (md5->toStr() == _downloadList[custom].md5)
				{
					_downloadList[custom].location = GBUtils::getExternalPath(custom);
					DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATED_ONE, &_downloadList[custom]);
				}
				else
					_downloader->createDownloadFileTask(url, path, custom);
			}
			else
				_downloader->createDownloadFileTask(url, path, custom);
			data.clear();
		}
	}
}

void GBUpdate::onDownloadError(const DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)
{
	DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_FINISH, (void *)1);
}

void GBUpdate::parseMD5()
{
	_updateInfos.clear();

	vector<vector<string>> strs = GBCsv::parseCsv("md5");
	if (strs.size() > 0)
	{
		_localVersion = strs[0][0];		
		for (unsigned int i = 1; i < strs.size(); i++)
		{
			GBUpdateInfo info = { strs[i][0], strs[i][1], _localVersion, strs[i][0], (unsigned int)atoi(strs[i][2].c_str()) };				
			Data data = FILE()->getDataFromFile(info.location);
			if (data.getBytes())
			{
				auto md5 = new MD5(data.getBytes(), data.getSize());				
				if (md5->toStr() == info.md5)
				{
					_updateInfos[strs[i][0]] = info;					
				}
			}
		}
	}

	if (_isUpdateEnabled)
	{
		strs = GBCsv::parseCsv(GBUtils::getExternalPath("md5"));
		if (strs.size() > 0)
		{
			_localExternalVersion = strs[0][0];			
			if (isVersionValid(_localExternalVersion, _localVersion))
			{
				for (unsigned int i = 1; i < strs.size(); i++)
				{					
					GBUpdateInfo externalInfo = { strs[i][0], strs[i][1], _localExternalVersion, GBUtils::getExternalPath(strs[i][0]), (unsigned int)atoi(strs[i][2].c_str()) };
					if (_updateInfos.find(externalInfo.key) == _updateInfos.end() || _updateInfos[externalInfo.key].md5 != externalInfo.md5)
					{			
						Data data = FILE()->getDataFromFile(externalInfo.location);
						if (data.getBytes())
						{
							auto md5 = new MD5(data.getBytes(), data.getSize());
							if (md5->toStr() == externalInfo.md5)
							{
								_updateInfos[externalInfo.key] = externalInfo;								
							}
							else
							{
								externalInfo.location = "";
								_downloadList[externalInfo.key] = externalInfo;
							}
						}
						else
						{
							externalInfo.location = "";
							_downloadList[externalInfo.key] = externalInfo;
						}
						data.clear();
					}
				}
			}
		}
	}
}
string GBUpdate::getRealPath(const string& path)
{
	if (_isUpdateEnabled)
	{
		string newPath = path;
		size_t pos = path.find_first_of("\\");
		while (pos != std::string::npos)
		{
			newPath.replace(pos, 1, "/");
			pos = newPath.find_first_of("\\");
		}
		newPath = StringUtils::format("res/%s", newPath.c_str());

		GBUpdateInfo * pInfo = GBUpdate::getInstance()->getUpdateInfo(newPath);
		if (pInfo)
			return pInfo->location;
	}

	return path;
}

extern "C" {
	int gbupdate_lua_loader(lua_State * L)
	{
		static const std::string FILE_EXT = ".lua";
		std::string filename(luaL_checkstring(L, 1));

		Data chunk;
		size_t pos = filename.rfind(FILE_EXT);
		if (pos == filename.length() - FILE_EXT.length())
		{
			filename = filename.substr(0, pos);
		}

		pos = filename.find_first_of(".");
		while (pos != std::string::npos)
		{
			filename.replace(pos, 1, "/");
			pos = filename.find_first_of(".");
		}

		do
		{
			GBUpdateInfo * pInfo = GBUpdate::getInstance()->getUpdateInfo(StringUtils::format("src/%s%s", filename.c_str(), FILE_EXT.c_str()));
			if (!pInfo)
				break;

			if (FILE()->isFileExist(pInfo->location))
			{
				chunk = FILE()->getDataFromFile(pInfo->location);
				break;
			}
		} while (false);

		if (chunk.getBytes())
		{
			LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
			stack->luaLoadBuffer(L, (char*)chunk.getBytes(), (int)chunk.getSize(), filename.c_str());
			chunk.clear();
		}
		else
		{
			CCLOG("can not get file data of %s", filename.c_str());
			return 0;
		}

		return 1;
	}
}