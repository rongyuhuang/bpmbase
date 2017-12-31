/*对CTP的行情做包装
*/
#ifndef MDSM_H
#define MDSM_H

#include<string>
#include<vector>
#include<atomic>
#include<mutex>
#include<condition_variable>

#include"ThostFtdcMdApi.h"

class MdSm:public CThostFtdcMdSpi{

public:
    explicit MdSm();

    void start();
    void stop();
    const char* getTradingDay();
    void subscribe(const char* instrument);
    void unsubscribe(const char* instrument);
public:
    //CTP部分的
    std::string brokerID,mdFront,userID,password,mdFlowPath;
    int apiTimeout,queryFreq=1,tradeFreq=5;

    //多线程控制
    std::atomic_bool started; //是否已经启动
    std::atomic_int sync_status; //同步状态

    //tick相关
    std::vector<void*> ticks;
    std::condition_variable ticks_cv;
    std::mutex ticks_mutex;
    std::atomic_bool need_notify;
    std::atomic_bool quit_flag;

private:
    CThostFtdcMdApi* mdApi= nullptr;

    std::atomic_int reqID=0;
    std::mutex flowMutex;
    std::chrono::system_clock::time_point tpLastCheck;
    std::chrono::milliseconds tradeDuration;
    std::chrono::milliseconds qryDuration;
    std::vector<std::string> subPool;//合约订阅池
private:
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnFrontConnected();

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    ///@param nReason 错误原因
    ///        0x1001 网络读失败
    ///        0x1002 网络写失败
    ///        0x2001 接收心跳超时
    ///        0x2002 发送心跳失败
    ///        0x2003 收到错误报文
    virtual void OnFrontDisconnected(int nReason);

    ///心跳超时警告。当长时间未收到报文时，该方法被调用。
    ///@param nTimeLapse 距离上次接收报文的时间
    virtual void OnHeartBeatWarning(int nTimeLapse);


    ///登录请求响应
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    ///登出请求响应
    virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    ///错误应答
    virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    ///订阅行情应答
    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    ///取消订阅行情应答
    virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    ///订阅询价应答
    virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    ///取消订阅询价应答
    virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    ///深度行情通知
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) ;

    ///询价通知
    virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;

private:
    void login();
    void flowControl(bool isQuery=true);
};
#endif // MDSM_H
