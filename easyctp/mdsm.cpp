#include <iostream>
#include<algorithm>
#include"MdSm.h"
#include"ctpstruct.h"
#include"utils/logging.h"
#include"utils/timeutil.h"
#include"ctputils.h"

MdSm::MdSm()
{
    tpLastCheck = std::chrono::system_clock::now();
    tradeDuration = std::chrono::milliseconds(1000/tradeFreq);
    qryDuration = std::chrono::milliseconds(1000/queryFreq);
}

void MdSm::start()
{
    CHECK(!started);
    CHECK(sync_status==0);
    sync_status = 1;

    started = true;
    reqID =0;
    need_notify=false;
    quit_flag=false;

    mdApi = CThostFtdcMdApi::CreateFtdcMdApi(mdFlowPath.c_str());
    mdApi->RegisterSpi(this);
    mdApi->RegisterFront((char*)mdFront.c_str());
    mdApi->Init();
}

void MdSm::stop()
{
    CHECK(started);
    {
        std::lock_guard<std::mutex> lock(ticks_mutex);
        quit_flag = true;
        ticks_cv.notify_all();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));
    mdApi->RegisterSpi(nullptr);
    mdApi->Release();
    mdApi = nullptr;

    sync_status=0;
    started = false;
    reqID =0;
    quit_flag = false;
    need_notify = false;
    for(auto x:ticks)
    {
        free(x);
    }
    ticks.clear();;
}

void MdSm::subscribe(const char *instrument)
{
    CHECK(started);
    CHECK(sync_status==0);
    sync_status =1;

    char* symbols[1] ={(char*)instrument};
    flowControl(false);
    int result =mdApi->SubscribeMarketData(symbols,1);
    if(result==0)
    {
        LOG(INFO)<<__FUNCTION__<<",Instrument:"<<instrument<<",success";
    }
    else
    {
        LOG(INFO)<<__FUNCTION__<<",Instrument:"<<instrument<<",error:code="<<result;
    }

}

void MdSm::unsubscribe(const char *instrument)
{
    CHECK(started);
    CHECK(sync_status==0);
    sync_status =1;

    char* symbols[1] ={(char*)instrument};
    flowControl(false);
    int result =mdApi->UnSubscribeMarketData(symbols,1);
    if(result==0)
    {
        //remove instrument from subPool
        LOG(INFO)<<__FUNCTION__<<",Instrument:"<<instrument<<",success";
    }
    else
    {
        LOG(INFO)<<__FUNCTION__<<",Instrument:"<<instrument<<",error:code="<<result;
    }
}

const char* MdSm::getTradingDay()
{
    CHECK(started);
    return mdApi->GetTradingDay();
}
///private function

void MdSm::login()
{
    CThostFtdcReqUserLoginField req;
    memset(&req,0,sizeof(req));

    int reqId = ++reqID;
    strncpy(req.BrokerID,brokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID,userID.c_str(),sizeof(req.UserID)-1);
    strncpy(req.Password,password.c_str(),sizeof(req.Password)-1);

    flowControl(false);
    int result = mdApi->ReqUserLogin(&req,reqId);
    if(result==0)
    {
        LOG(INFO)<<__FUNCTION__<<",success";
    }
    else
    {
        LOG(INFO)<<__FUNCTION__<<",error:code="<<result;
    }

}

void MdSm::flowControl(bool isQuery)
{
    std::lock_guard<std::mutex> lock(flowMutex);

    auto tpNow = std::chrono::system_clock::now();
    auto consumedDuration = std::chrono::duration_cast<std::chrono::milliseconds>(tpNow - tpLastCheck);
    if(consumedDuration > (isQuery ? qryDuration:tradeDuration))
    {
        tpLastCheck = tpNow;
        return ;
    }
    auto sleepDuration = (isQuery? qryDuration:tradeDuration) - consumedDuration;
    std::this_thread::sleep_for(sleepDuration);
    tpLastCheck = std::chrono::system_clock::now();
}
///CTP MD的回调函数
///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void MdSm:: OnFrontConnected()
{
    LOG(INFO)<<__FUNCTION__;
    login();
    //断线重连后，需要重新订阅合约池
    if(subPool.size()>0)
    {
        for(auto x:subPool)
        {
            subscribe(x.c_str());
        }
    }
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void MdSm:: OnFrontDisconnected(int nReason)
{
    LOG(INFO)<<__FUNCTION__<<",Reason:"<<nReason;
}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void MdSm:: OnHeartBeatWarning(int nTimeLapse){}


///登录请求响应
void MdSm:: OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast && !CtpUtils::isErrorRsp(pRspInfo,nRequestID))
    {
        sync_status=2;
        LOG(INFO)<<__FUNCTION__;
    }
}

///登出请求响应
void MdSm:: OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///错误应答
void MdSm:: OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast && CtpUtils::isErrorRsp(pRspInfo,nRequestID))
    {
        LOG(INFO)<<__FUNCTION__<<",ErrorID="<<pRspInfo->ErrorID<<",ErrorMsg="<<pRspInfo->ErrorMsg;
    }
}

///订阅行情应答
void MdSm:: OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast && !CtpUtils::isErrorRsp(pRspInfo,nRequestID))
    {
        auto iter= std::find(subPool.begin(),subPool.end(),std::string(pSpecificInstrument->InstrumentID));
        if(iter ==subPool.end())
        {
            subPool.push_back(std::string(pSpecificInstrument->InstrumentID));
        }
        sync_status=2;
    }
}

///取消订阅行情应答
void MdSm:: OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

    if(bIsLast && !CtpUtils::isErrorRsp(pRspInfo,nRequestID))
    {
        auto iter= std::find(subPool.begin(),subPool.end(),std::string(pSpecificInstrument->InstrumentID));
        if(iter !=subPool.end())
        {
            subPool.erase(iter);
        }
    }
}

///订阅询价应答
void MdSm:: OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///取消订阅询价应答
void MdSm:: OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///深度行情通知
void MdSm:: OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    //包装tick
    void* buff = malloc(sizeof(TickData)+4);
    memset(buff,0,sizeof(TickData)+4);
    *(int*)buff = RTNDATA_TICK;
    TickData* tick = (TickData*)((char*)buff+4);

    strncpy(tick->symbol,pDepthMarketData->InstrumentID,sizeof(tick->symbol)-1);
    char buff_time[32];
    sprintf(buff_time,"%s %s",pDepthMarketData->ActionDay,pDepthMarketData->UpdateTime);
    char fmt[]="%Y%m%d %H%M%S";
    tick->actionDatetime =bpm_str2ctime(buff_time,fmt);
    tick->updateMS = pDepthMarketData->UpdateMillisec;
    tick->lastPrice = pDepthMarketData->LastPrice;
    tick->totalVolume = pDepthMarketData->Volume;
    //TODO:volume
    tick->openInterest = pDepthMarketData->OpenInterest;
    tick->ask1Price = pDepthMarketData->AskPrice1;
    tick->ask1Volume = pDepthMarketData->AskVolume1;
    tick->bid1Price = pDepthMarketData->BidPrice1;
    tick->bid1Volume = pDepthMarketData->BidVolume1;

    tick->openPrice = pDepthMarketData->OpenPrice;
    tick->highPrice = pDepthMarketData->HighestPrice;
    tick->lowPrice = pDepthMarketData->LowestPrice;
    tick->closePrice = pDepthMarketData->ClosePrice;
    tick->settlePrice = pDepthMarketData->SettlementPrice;

    tick->preclosePrice = pDepthMarketData->PreClosePrice;
    tick->presettlePrice = pDepthMarketData->PreSettlementPrice;
    tick->averagePrice = pDepthMarketData->AveragePrice;
    tick->upperLimitPrice = pDepthMarketData->UpperLimitPrice;
    tick->lowerLimitPrice = pDepthMarketData->LowerLimitPrice;

    //rudui
    std::lock_guard<std::mutex> lock(ticks_mutex);
    ticks.push_back(buff);
    if(need_notify)
    {
        ticks_cv.notify_one();
    }
}

///询价通知
void MdSm:: OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {}

