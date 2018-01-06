/*封装EASYCTP交易部分对外的接口
*/
#include"tdsm.h"
#include"easyctp.h"
#include"ctputils.h"

#include<utils/logging.h>
#include<utils/strutil.h>
#include<utils/fileutil.h>
#include<fmt/format.h>
static TdSm g_tdSm;

static int timeout=1000;
static int default_timeout=2000;


static bool checkStatus()
{
    int step =100;
    std::chrono::milliseconds span(step);
    for(int i=0;i<timeout;i+=step)
    {
        std::this_thread::sleep_for(span);
        if(g_tdSm.sync_status==2)
        {
            return true;
        }
    }
    return false;
}
//TD API
 void __cdecl td_setBrokerInfo(const char* brokerID,const char* tdFront)
 {
     LOG(INFO)<<__FUNCTION__<<",BrokerID:"<<brokerID<<",TdFront:"<<tdFront;
     g_tdSm.brokerID = brokerID;
     g_tdSm.tdFront = tdFront;
 }
 void __cdecl td_setUserInfo(const char* userID,const char* password)
 {
     LOG(INFO)<<__FUNCTION__<<",UserID:"<<userID<<",Password:******";
     g_tdSm.userID = userID;
     g_tdSm.password = password;
 }
 void __cdecl td_setConfig(const char* flowPath,int apiTimeout,int queryFreq,int tradeFreq)
 {
    LOG(INFO)<<__FUNCTION__;
    g_tdSm.tdFlowPath = StrUtil::printf("%s/",flowPath);
    bpm_createDir(flowPath);
    timeout = apiTimeout > default_timeout ? apiTimeout:default_timeout;
    g_tdSm.queryFreq = queryFreq;
    g_tdSm.orderFreq = tradeFreq;
 }
 int __cdecl td_start()
 {
     if(g_tdSm.started)
     {
         return STATUS_RUNNING;
     }
     if(g_tdSm.sync_status>0)
     {
         return STATUS_BUSY;
     }
     g_tdSm.start();
     if(checkStatus())
     {
         g_tdSm.sync_status=0;
         return STATUS_OK;
     }
     else
     {
         return STATUS_TIMEOUT;
     }
 }
 const char* __cdecl td_getTradingDay()
 {
    return  g_tdSm.getTradingDay();
 }
//td 相关的查询
 int __cdecl td_queryContracs()
 {
     return STATUS_OK;
 }
 int __cdecl td_queryAccount()
 {
     return STATUS_OK;
 }
 int __cdecl td_queryPosition()
 {
     return STATUS_OK;
 }
 int __cdecl td_queryOrder()
 {
     return STATUS_OK;
 }
 int __cdecl td_queryTrade()
 {
     return STATUS_OK;
 }

 int __cdecl td_queryMarketData()
 {
     if(!g_tdSm.started)
     {
         return STATUS_STOPPED;
     }
     if(g_tdSm.sync_status>0)
     {
         return STATUS_BUSY;
     }
     g_tdSm.queryMarketData();

     return STATUS_OK;
 }
//td 交易相关
 int __cdecl td_placeOrder()
 {
     return STATUS_OK;
 }
 int __cdecl td_cancelOrder()
 {
     return STATUS_OK;
 }

 int __cdecl td_freeMemory(void* ptr)
 {
     return STATUS_OK;
 }
 int __cdecl td_stop()
 {
     if(!g_tdSm.started)
     {
         return STATUS_STOPPED;
     }
     g_tdSm.stop();
     return STATUS_OK;
 }
