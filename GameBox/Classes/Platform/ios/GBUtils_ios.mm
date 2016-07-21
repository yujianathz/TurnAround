#include "GBUtils.h"
#include "GBDefine.h"

#include "OpenUDID.h"
#include "GamePayment.h"

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <StoreKit/StoreKit.h>

using namespace cocos2d;

std::vector<std::string> GBUtils::readDirectory(const std::string& relativePath, const std::string keyWord)
{
	std::vector<std::string> v;

	return v;
}

std::string GBUtils::coding(const std::string &str,const char* toCode,const char* formCode)
{
	std::string temp = str;
	return temp;  
}

void GBUtils::openURL(std::string &url)
{

}

std::string GBUtils::getChannel()
{
    return "ios";
}

std::string GBUtils::getDeviceId()
{
    NSString* openUDID = [OpenUDID value];
    const char * pStr = [openUDID UTF8String];
    
    return std::string(pStr);
}

void GBUtils::vibration(float duration)
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

void GBUtils::purchase(const string &payInfo)
{
    DIRECTOR()->getEventDispatcher()->dispatchCustomEvent("purchase_success", (void *)&payInfo);
}

std::string GBUtils::getOpenUDID()
{
    return getDeviceId();
}
