/*easyctp接口的定义
 *
*/
#ifndef EASYCTP_H
#define EASYCTP_H

#include"ctpstruct.h"

#define EASYCTP_API

#if !defined(_WIN32) && !defined(__cdecl)
#define __cdecl
#endif

#ifdef __cplusplus
extern "C"{
#endif

//MD API
EASYCTP_API void __cdecl md_setBrokerInfo(const char* brokerID,const char* mdFront);
EASYCTP_API void __cdecl md_setUserInfo(const char* userID,const char* password);
EASYCTP_API void __cdecl md_setConfig(const char* flowPath,int apiTimeout);
EASYCTP_API int __cdecl md_start();
EASYCTP_API const char* __cdecl md_getTradingDay();
EASYCTP_API int __cdecl md_subscribe(const char* instrument);
EASYCTP_API int __cdecl md_unsubscribe(const char* instrument);
EASYCTP_API int __cdecl md_queryRtnData(RtnData* rtnData);
EASYCTP_API int __cdecl md_freeMemory(void* ptr);
EASYCTP_API int __cdecl md_stop();


//TD API
EASYCTP_API void __cdecl td_setBrokerInfo(const char* brokerID,const char* tdFront);
EASYCTP_API void __cdecl td_setUserInfo(const char* userID,const char* password);
EASYCTP_API void __cdecl td_setConfig(const char* flowPath,int apiTimeout,int queryFreq,int tradeFreq);
EASYCTP_API int __cdecl td_start();
EASYCTP_API const char* __cdecl td_getTradingDay();
//td 相关的查询
EASYCTP_API int __cdecl td_queryContracs();
EASYCTP_API int __cdecl td_queryAccount();
EASYCTP_API int __cdecl td_queryPosition();
EASYCTP_API int __cdecl td_queryOrder();
EASYCTP_API int __cdecl td_queryTrade();
EASYCTP_API int __cdecl td_queryMarketData();
//td 交易相关
EASYCTP_API int __cdecl td_placeOrder();
EASYCTP_API int __cdecl td_cancelOrder();

EASYCTP_API int __cdecl td_freeMemory(void* ptr);
EASYCTP_API int __cdecl td_stop();


#ifdef __cplusplus
}
#endif

#endif // EASYCTP_H
