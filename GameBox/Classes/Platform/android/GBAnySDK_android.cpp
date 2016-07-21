#include "GBAnySDK.h"
#include "GBDefine.h"

#include "PluginJniHelper.h"

#include "json/document.h"

GBAnySDK * s_pAnySDK = NULL;

string s_appKey = "";
string s_appSecret = "";
string s_privateKey = "";
string s_oauthLoginServer = "";

GBAnySDK * GBAnySDK::getInstance()
{
		if (!s_pAnySDK)
			s_pAnySDK = new GBAnySDK();
			
		return s_pAnySDK;
}

void GBAnySDK::setJavaVM(JavaVM* vm)
{
		PluginJniHelper::setJavaVM(vm);
}

GBAnySDK::GBAnySDK()
{
		DIRECTOR()->getEventDispatcher()->addCustomEventListener(ANYSDK_EVENT_INIT_SUCCESS, [this](EventCustom * pEvent)
		{
				initSuccessCallback();	
		});

		DIRECTOR()->getEventDispatcher()->addCustomEventListener(ANYSDK_EVENT_INIT_FAIL, [this](EventCustom * pEvent)
		{
				initFailCallback();	
		});

		DIRECTOR()->getEventDispatcher()->addCustomEventListener(ANYSDK_EVENT_LOGIN_SUCCESS, [this](EventCustom * pEvent)
		{
				loginSuccessCallback();	
		});

		DIRECTOR()->getEventDispatcher()->addCustomEventListener(ANYSDK_EVENT_LOGIN_FAIL, [this](EventCustom * pEvent)
		{
				loginFailCallback();	
		});

		DIRECTOR()->getEventDispatcher()->addCustomEventListener(ANYSDK_EVENT_LOGOUT_SUCCESS, [this](EventCustom * pEvent)
		{
				logoutSuccessCallback();	
		});

		DIRECTOR()->getEventDispatcher()->addCustomEventListener(ANYSDK_EVENT_LOGOUT_FAIL, [this](EventCustom * pEvent)
		{
				logoutFailCallback();	
		});
		
		_isInited = false;
}

void GBAnySDK::initSuccessCallback()
{
		_isInited = true;	
}

void GBAnySDK::initFailCallback()
{
		initSDK(s_appKey, s_appSecret, s_privateKey, s_oauthLoginServer);
}

void GBAnySDK::loginSuccessCallback()
{
			
}

void GBAnySDK::loginFailCallback()
{
		login();			
}

void GBAnySDK::logoutSuccessCallback()
{
	
}

void GBAnySDK::logoutFailCallback()
{
		logout();	
}

void GBAnySDK::initSDK(const string& appKey, const string& appSecret, const string& privateKey, const string& oauthLoginServer)
{
		s_appKey = appKey;
		s_appSecret = appSecret;
		s_privateKey = privateKey;
		s_oauthLoginServer = oauthLoginServer;		
	
		AgentManager::getInstance()->init(appKey, appSecret, privateKey, oauthLoginServer);
		AgentManager::getInstance()->loadAllPlugins();
			
		ProtocolUser* pUser = AgentManager::getInstance()->getUserPlugin();
		if(pUser);
				pUser->setActionListener(this);	
		
		std::map<std::string , ProtocolIAP*>* pluginsIAPMap= AgentManager::getInstance()->getIAPPlugin();
		if (pluginsIAPMap)
		{
				std::map<std::string , ProtocolIAP*>::iterator iter;					
				for(iter = pluginsIAPMap->begin(); iter != pluginsIAPMap->end(); iter++)
				    (iter->second)->setResultListener(this);				
		}
		
		ProtocolAds* pAds= AgentManager::getInstance()->getAdsPlugin();
		if(pAds)
				pAds->setAdsListener(this);
}

void GBAnySDK::login()
{
		ProtocolUser* pUser = AgentManager::getInstance()->getUserPlugin();
    if(!pUser) return;
    pUser->login();
}

void GBAnySDK::logout()
{
		ProtocolUser* pUser = AgentManager::getInstance()->getUserPlugin();
    if(!pUser || !pUser->isFunctionSupported("logout")) return;
    	pUser->callFuncWithParam("logout",NULL);	
}

void GBAnySDK::showToolBar()
{
    ProtocolUser* pUser = AgentManager::getInstance()->getUserPlugin();
    if(!pUser || !pUser->isFunctionSupported("showToolBar")) return;
    PluginParam paramInfo(3);
    pUser->callFuncWithParam("showToolBar",&paramInfo,NULL);	
}

void GBAnySDK::hideToolBar()
{
    ProtocolUser* pUser = AgentManager::getInstance()->getUserPlugin();
    if(!pUser || !pUser->isFunctionSupported("hideToolBar")) return;
    pUser->callFuncWithParam("hideToolBar",NULL);			
}

void GBAnySDK::startSession()
{
		if(AgentManager::getInstance()->getAnalyticsPlugin())
		{
				AgentManager::getInstance()->getAnalyticsPlugin()->startSession();
		}
}

void GBAnySDK::stopSession()
{
		if(AgentManager::getInstance()->getAnalyticsPlugin())
		{
				AgentManager::getInstance()->getAnalyticsPlugin()->stopSession();
		}
}

void GBAnySDK::exit()
{
    ProtocolUser* pUser = AgentManager::getInstance()->getUserPlugin();
    if(!pUser || !pUser->isFunctionSupported("exit")) return;
    pUser->callFuncWithParam("exit",NULL);	
}

bool GBAnySDK::isLogined()
{
		ProtocolUser* pUser = AgentManager::getInstance()->getUserPlugin();
		if(!pUser) return false;
		
		return pUser->isLogined();
}

void GBAnySDK::pay(const std::string& json)
{
		std::map<std::string , ProtocolIAP*>* pluginsIAPMap= AgentManager::getInstance()->getIAPPlugin();    
    if(pluginsIAPMap)
    {
    		rapidjson::Document d;
    		d.Parse<0>(json.c_str());
    		
    		string productId = d["productId"].GetString();
    		string productName = d["productName"].GetString();
    		int productPrice = d["price"].GetInt();
    	
    		TProductInfo productInfo;
        productInfo["Product_Price"] = StringUtils::format("%d", productPrice);
        productInfo["Product_Id"] = productId;
        productInfo["Product_Name"] = productName;
        productInfo["Server_Id"] = "1";
        productInfo["Product_Count"] = "1";
        productInfo["Role_Id"] = "1";
        productInfo["Role_Name"] = "1";
        productInfo["Role_Grade"] = "1";
        productInfo["Role_Balance"] = "1";
        productInfo["EXT"] = json;
        
        if(pluginsIAPMap->size() > 0)
        {
        		std::map<std::string , ProtocolIAP*>::iterator it = pluginsIAPMap->begin();
        		(it->second)->payForProduct(productInfo);	
        }                
    }			
}

void GBAnySDK::preloadAds(AdsType adType)
{
    ProtocolAds* pAds = AgentManager::getInstance()->getAdsPlugin();
    if( pAds && pAds->isAdTypeSupported(adType))
    {
        pAds->preloadAds(adType, 1);      
        pAds->preloadAds(adType, 2);
    }
}

void GBAnySDK::showAds(AdsType adType)
{
    ProtocolAds* pAds = AgentManager::getInstance()->getAdsPlugin();
    if( pAds && pAds->isAdTypeSupported(adType))
    {
        pAds->showAds(adType, CCRANDOM_0_1() > 0.5f ? 1 : 2);
    }	
}

void GBAnySDK::hideAds(AdsType adType)
{
    ProtocolAds* pAds = AgentManager::getInstance()->getAdsPlugin();
    if( pAds && pAds->isAdTypeSupported(adType))
    {
        pAds->hideAds(adType, 1);      
        pAds->hideAds(adType, 2);
    }	
}

void GBAnySDK::onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg)
{
		switch(code)
		{
		case kInitSuccess:
				DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(ANYSDK_EVENT_INIT_SUCCESS);
		    break;
		case kInitFail:
				DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(ANYSDK_EVENT_INIT_FAIL);
		    break;		    
		case kLoginSuccess:
		    DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(ANYSDK_EVENT_LOGIN_SUCCESS);
		    break;
		case kLoginNetworkError:		
		case kLoginFail:		
				DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(ANYSDK_EVENT_LOGIN_FAIL);
		    break;		    
		case kLogoutSuccess:
				DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(ANYSDK_EVENT_LOGOUT_SUCCESS);
		    break;
		case kLogoutFail:
				DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(ANYSDK_EVENT_LOGOUT_FAIL);
		    break;		    
		}
}

void GBAnySDK::onPayResult(PayResultCode ret, const char* msg, TProductInfo info)
{
		string * pInfo = &info["EXT"];
		switch(ret)
		{
		case kPaySuccess:				  
				 DIRECTOR()->getEventDispatcher()->dispatchCustomEvent("purchase_success", pInfo); 
		     break;
		case kPayCancel://支付取消回调
				 DIRECTOR()->getEventDispatcher()->dispatchCustomEvent("purchase_cancel", pInfo);
				 break;		     
		case kPayNetworkError://支付网络出错回调		
		case kPayProductionInforIncomplete://支付信息填写不完整回调
		case kPayFail://支付失败回调
				 DIRECTOR()->getEventDispatcher()->dispatchCustomEvent("purchase_fail", pInfo);
				 break;
		/**
		* 新增加:正在进行中回调
		* 支付过程中若SDK没有回调结果，就认为支付正在进行中
		* 游戏开发商可让玩家去判断是否需要等待，若不等待则进行下一次的支付
		*/
		case kPayNowPaying:
				 break;
		}			
}

void GBAnySDK::onAdsResult(AdsResultCode code, const char* msg)
{

}