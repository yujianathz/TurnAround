#include "GamePayment.h"
#include "HttpClient.h"

using namespace cocos2d::network;

//==========================================================================================
// 初始化
//==========================================================================================

GamePayment *GamePayment::getInstance()
{
	static GamePayment _instance;
	return &_instance;
}

GamePayment::GamePayment()
{
    _verifyUrl = "https://sandbox.itunes.apple.com/verifyReceipt";
}

// 初始化（获取商品信息）
void GamePayment::req_iap(std::vector<std::string>& identifiers, iapProductCallback callback)
{
    _iap.requestProducts(identifiers, callback);
}

// 付款
void GamePayment::pay_iap(std::string& identifier, iapPaymentCallback callback)
{
    _iap.requestPayment(identifier, callback);
}

// 恢复购买
void GamePayment::restore_iap(iapRestoreCallback restoreCallback, iapRestoreFinishCallback finishCallback)
{
    _iap.requestRestore(restoreCallback, finishCallback);
}