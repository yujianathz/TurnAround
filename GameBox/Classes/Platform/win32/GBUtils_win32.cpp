#include "GBUtils.h"
#include "GBDefine.h"

#include "win32-specific\icon\include\iconv.h"
#include <io.h>
#include <iostream>

static std::string getCPUId()
{
	int s1, s2;
	__asm
	{
		mov eax, 01h
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, eax
	}
	std::string cpuId1 = StringUtils::format("%08x%08x", s1, s2);

	__asm
	{
		mov eax, 03h
		xor ecx, ecx
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, ecx
	}
	std::string cpuId2 = StringUtils::format("%08x%08x", s1, s2);

	return cpuId1 + cpuId2;
}

std::vector<std::string> GBUtils::readDirectory(const std::string& relativePath, const std::string keyWord)
{
	std::vector<std::string> v;

	std::vector<std::string> searchPaths = FileUtils::getInstance()->getSearchPaths();
	for (unsigned int i = 0; i < searchPaths.size(); ++ i)
	{
		std::string absolutePath = searchPaths[i] + relativePath;
		if (FILE()->isDirectoryExist(absolutePath))
		{
			_finddata_t fileDir;
			if (absolutePath.find_last_of("\\") != absolutePath.size() - 1 && absolutePath.find_last_of("/") != absolutePath.size() - 1)
				absolutePath.append("/");
			absolutePath.append(keyWord.c_str());
			long lfDir;
			if((lfDir = _findfirst(absolutePath.c_str(),&fileDir))==-1l)
				CCLOG("No file is found\n");
			else{
				CCLOG("file list:\n");
				do{
					v.push_back(std::string(fileDir.name));
					CCLOG("%s\n",fileDir.name);
				}while( _findnext( lfDir, &fileDir ) == 0 );
			}
			_findclose(lfDir);
			break;
		}
	}

	return v;
}

std::string GBUtils::coding(const std::string &str,const char* toCode,const char* formCode)
{
	std::string temp = str;

	iconv_t iconvH;  
	iconvH = iconv_open(formCode,toCode);  
	if(iconvH == 0)  
	{  
		return temp;  
	}  

	const char* strChar = temp.c_str();
	const char** pin = &strChar;  

	size_t strLength = temp.length();
	char* outbuf = (char*)malloc(strLength*4);  
	char* pBuff = outbuf;  
	memset(outbuf,0,strLength*4);  
	size_t outLength = strLength*4;  
	if(-1 == iconv(iconvH,pin,&strLength,&outbuf,&outLength))  
	{  
		iconv_close(iconvH);  
		return temp;  
	}  

	temp = pBuff;
	iconv_close(iconvH);  

	return temp;
}

void GBUtils::openURL(std::string &url)
{
	ShellExecuteA(NULL,"open", url.c_str(), NULL, NULL, SW_SHOW);
}

std::string GBUtils::getChannel()
{
	return "wanpishu";
}

std::string GBUtils::getDeviceId()
{
	return "windows" + getCPUId();
}

void GBUtils::vibration(float duration)
{
	CCLOG("vibration");
}

void GBUtils::purchase(const string& payInfo)
{
	DIRECTOR()->getEventDispatcher()->dispatchCustomEvent("purchase_success", (void *)&payInfo);
}

std::string GBUtils::getOpenUDID()
{
	return GBUtils::getDeviceId();
}