//==========================================================================================
// IOSiAP_Bridge.cpp
// Created by Dolphin Lee.
//==========================================================================================

#include "IOSiAP_Bridge.h"

//==========================================================================================
// 构造函数
//==========================================================================================

IOSiAP_Bridge::IOSiAP_Bridge()
{
    _productCallback = nullptr;
    _paymentCallback = nullptr;
    _restoreCallback = nullptr;
    _restoreFinishCallback = nullptr;
    
    // 创建IOSIAP
    iap = new IOSiAP();
    iap->delegate = this;
}

IOSiAP_Bridge::~IOSiAP_Bridge()
{
    delete iap;
}

//==========================================================================================
// 回调函数（IOSIAP返回结果）
//==========================================================================================

// ［回调］获取商品请求成功返回
void IOSiAP_Bridge::onRequestProductsFinish(std::string& identifier)
{
    // 必须在onRequestProductsFinish后才能去请求IAP产品数据
    auto product = iap->iOSProductByIdentifier(identifier);
    
    // 回调出去
    if (_productCallback)
        _productCallback(product, IAP_REQUEST_SUCCESS);
}

// ［回调］获取商品请求失败返回
void IOSiAP_Bridge::onRequestProductsError(int code)
{
    // 回调出去
    if (_productCallback)
        _productCallback(nullptr, code);
}

// ［回调］支付结果返回
void IOSiAP_Bridge::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event)
{
    if (event == IOSIAP_PAYMENT_PURCHASING)
    {
        // 不需要做任何处理
    }
    else if (event == IOSIAP_PAYMENT_PURCHAED)
    {
        // 付款成功
        if (_paymentCallback)
        {
            _paymentCallback(true, identifier);
        }
    }
    else if (event == IOSIAP_PAYMENT_FAILED)
    {
        // 付款失败
        if (_paymentCallback)
        {
            _paymentCallback(false, identifier);
        }
    }
    else if (event == IOSIAP_PAYMENT_RESTORED)
    {
        // 恢复购买
        if (_restoreCallback)
        {
            _restoreCallback(identifier);
        }
    }
}

// ［回调］恢复购买完成回调
void IOSiAP_Bridge::onRestoreFinished(bool succeed)
{
    // 恢复购买完成
    if (_restoreFinishCallback)
    {
        _restoreFinishCallback(succeed);
    }
}

//==========================================================================================
// 外部调用
//==========================================================================================

// 获取商品信息
void IOSiAP_Bridge::requestProducts(std::vector<std::string>& identifiers, iapProductCallback callback)
{
    _productCallback = callback;
    iap->requestProducts(identifiers);
}

// 付款请求
void IOSiAP_Bridge::requestPayment(std::string& identifier, iapPaymentCallback callback)
{
    _paymentCallback = callback;
    auto product = iap->iOSProductByIdentifier(identifier);
    if (product)
        iap->paymentWithProduct(product, 1);
}

// ［请求］恢复购买
void IOSiAP_Bridge::requestRestore(iapRestoreCallback restoreCallback, iapRestoreFinishCallback finishCallback)
{
    _restoreCallback = restoreCallback;
    _restoreFinishCallback = finishCallback;
    
    // 恢复请求
    iap->restorePayment();
}

//==========================================================================================
//
//==========================================================================================

