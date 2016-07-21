//==========================================================================================
// IOSiAP_Bridge.h
// Created by Dolphin Lee.
// http://blog.csdn.net/ldpjay/article/details/46459509
//==========================================================================================
#ifndef __IOSIAP_BRIDGE_H__
#define __IOSIAP_BRIDGE_H__

#include "IOSiAP.h"

//==========================================================================================

typedef std::function<void(IOSProduct *product, int code)> iapProductCallback; // 获取商品回调
typedef std::function<void(bool succeed, std::string &identifier)> iapPaymentCallback; // 支付结果回调

typedef std::function<void(std::string &identifier)> iapRestoreCallback; // 恢复购买回调（每个商品回调一次）
typedef std::function<void(bool succeed)> iapRestoreFinishCallback; // 恢复购买完成回调

#define IAP_REQUEST_SUCCESS 1000  // 获取成功代码

//==========================================================================================

class IOSiAP_Bridge : public IOSiAPDelegate
{
public:
    // ［请求］获取商品信息
    void requestProducts(std::vector<std::string>& identifiers, iapProductCallback callback);
   
    // ［请求］付款请求
    void requestPayment(std::string& identifier, iapPaymentCallback callback);

    // ［请求］恢复购买
    void requestRestore(iapRestoreCallback restoreCallback, iapRestoreFinishCallback finishCallback);

public:
    IOSiAP_Bridge();
    ~IOSiAP_Bridge();
    
    // ［回调］请求商品信息回调
    virtual void onRequestProductsFinish(std::string& identifier);
    virtual void onRequestProductsError(int code);
    // ［回调］付款结果回调（恢复流程也走这里）
    virtual void onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event);
    // ［回调］恢复购买完成回调
    virtual void onRestoreFinished(bool succeed);
    
private:
    IOSiAP             *iap;             // IOSiAp实例
    
    iapProductCallback _productCallback; // 外部回调（商品信息）
    iapPaymentCallback _paymentCallback; // 外部回调（购买结果）
    iapRestoreCallback _restoreCallback; // 外部回调（恢复购买）；
    iapRestoreFinishCallback _restoreFinishCallback; // 外部回调（恢复完成）
};

//==========================================================================================

#endif

