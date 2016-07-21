//==========================================================================================
// GamePayment.h
// Created by Dolphin Lee.
// http://blog.csdn.net/ldpjay/article/details/46459509
//==========================================================================================
#ifndef __GAME_PAYMENT_H__
#define __GAME_PAYMENT_H__

#include "IOSiAP_Bridge.h"

#include "GBDefine.h"
//==========================================================================================

class GamePayment
{
public:
    static GamePayment *getInstance();
    
    // 请求商品信息
    void req_iap(std::vector<std::string>& identifiers, iapProductCallback callback);
    
    // 购买请求
    void pay_iap(std::string& identifier, iapPaymentCallback callback);
    
    // 恢复购买
    void restore_iap(iapRestoreCallback restoreCallback, iapRestoreFinishCallback finishCallback);
    
    CC_SYNTHESIZE(std::string, _verifyUrl, VerifyUrl);
    
protected:
    GamePayment();
    
private:
    IOSiAP_Bridge           _iap;
};

#endif

