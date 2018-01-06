
#include"tdsm.h"
#include"ctputils.h"
#include<utils/logging.h>
TdSm::TdSm()
{
    tpLastChecker = std::chrono::system_clock::now();
    orderDuration = std::chrono::milliseconds(1000/orderFreq);
    queryDuration = std::chrono::milliseconds(1000/queryFreq);
}
void  TdSm::start()
{
    //check status
    CHECK(!started);
    CHECK(sync_status==0);
    //init memeber
    sync_status=1;
    started = true;
    reqID =0;

    //init ctp td api
    tdApi = CThostFtdcTraderApi::CreateFtdcTraderApi((char*)tdFlowPath.c_str());
    tdApi->RegisterSpi(this);
    tdApi->RegisterFront((char*)tdFront.c_str());
    //只接收重新登录后的公有流和私有流信息
    tdApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    tdApi->SubscribePublicTopic(THOST_TERT_QUICK);
    tdApi->Init();
}
void  TdSm::stop()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    tdApi->RegisterSpi(nullptr);
    tdApi->Release();
    tdApi=nullptr;

    started=false;
    reqID=0;
    sync_status=0;
}
const char*  TdSm::getTradingDay()
{
    CHECK(started);
    return tdApi->GetTradingDay();
}
//查询相关
void TdSm::queryInstrument()
{}
void TdSm::queryAccount()
{}
void  TdSm::queryPosition()
{}
void  TdSm::queryOrder()
{}
void  TdSm::queryTrade()
{}

void TdSm::queryMarketData()
{
    CHECK(started);
    CHECK(sync_status==0);
    sync_status=1;

    CThostFtdcQryDepthMarketDataField req;
    memset(&req,0,sizeof(req));
    flowControl();
    int id =++reqID;
    int ret = tdApi->ReqQryDepthMarketData(&req,id);
    if(ret ==0)
    {
        LOG(INFO)<<__FUNCTION__<<" success";
    }
    else
    {
        LOG(INFO)<<__FUNCTION__<<" fail:"<<ret;
    }
}
//TODO:queryMargin,queryCommission...

//交易相关
void  TdSm::placeOrder()
{}
void  TdSm::cancelOrder()
{}


////私有函数
///
void TdSm::login()
{
    CThostFtdcReqUserLoginField req;
    memset(&req,0,sizeof(req));
    strncpy(req.BrokerID,brokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID,userID.c_str(),sizeof(req.UserID)-1);
    strncpy(req.Password,password.c_str(),sizeof(req.Password)-1);

    flowControl(false);
    int id = ++reqID;
    int ret = tdApi->ReqUserLogin(&req,id);
    if(ret==0)
    {
        LOG(INFO)<<__FUNCTION__<<" success";
    }
    else
    {
        LOG(INFO)<<__FUNCTION__<<" fail:"<<ret;
    }
}

void TdSm::settlementConfirm()
{
     CThostFtdcSettlementInfoConfirmField req;
     memset(&req,0,sizeof(req));
     strncpy(req.BrokerID,brokerID.c_str(),sizeof(req.BrokerID)-1);
     strncpy(req.InvestorID,userID.c_str(),sizeof(req.InvestorID)-1);

     flowControl(false);
     int id =++reqID;
     int ret = tdApi->ReqSettlementInfoConfirm(&req,id);
     if(ret==0)
     {
         LOG(INFO)<<__FUNCTION__<<" success";
     }
     else
     {
         LOG(INFO)<<__FUNCTION__<<" fail:"<<ret;
     }
}

void TdSm::flowControl(bool isQuery)
{
    std::lock_guard<std::mutex> lock(flowMutex);

    auto tpNow = std::chrono::system_clock::now();
    auto diffDuration = std::chrono::duration_cast<std::chrono::milliseconds>(tpNow-tpLastChecker);
    if(diffDuration>(isQuery ? queryDuration:orderDuration))
    {
        tpLastChecker = tpNow;
    }
    auto sleepDuration = (isQuery? queryDuration:orderDuration) - diffDuration;
    std::this_thread::sleep_for(sleepDuration);
    tpLastChecker = std::chrono::system_clock::now();
}
///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void TdSm::OnFrontConnected()
{
    LOG(INFO)<<__FUNCTION__;
    login();
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void TdSm::OnFrontDisconnected(int nReason){}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void TdSm::OnHeartBeatWarning(int nTimeLapse){}

///客户端认证响应
void TdSm::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}


///登录请求响应
void TdSm::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)
    {
        if(CtpUtils::isErrorRsp(pRspInfo,nRequestID))
        {

        }
        else
        {
            settlementConfirm();
        }
    }
}

///登出请求响应
void TdSm::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///用户口令更新请求响应
void TdSm::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///资金账户口令更新请求响应
void TdSm::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///报单录入请求响应
void TdSm::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///预埋单录入请求响应
void TdSm::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///预埋撤单录入请求响应
void TdSm::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///报单操作请求响应
void TdSm::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///查询最大报单数量响应
void TdSm::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///投资者结算结果确认响应
void TdSm::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast && !CtpUtils::isErrorRsp(pRspInfo,nRequestID))
    {
        LOG(INFO)<<__FUNCTION__;
        sync_status=2;
    }
}

///删除预埋单响应
void TdSm::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///删除预埋撤单响应
void TdSm::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///执行宣告录入请求响应
void TdSm::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///执行宣告操作请求响应
void TdSm::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///询价录入请求响应
void TdSm::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///报价录入请求响应
void TdSm::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///报价操作请求响应
void TdSm::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///批量报单操作请求响应
void TdSm::OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///期权自对冲录入请求响应
void TdSm::OnRspOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///期权自对冲操作请求响应
void TdSm::OnRspOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField *pInputOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///申请组合录入请求响应
void TdSm::OnRspCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询报单响应
void TdSm::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询成交响应
void TdSm::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询投资者持仓响应
void TdSm::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询资金账户响应
void TdSm::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询投资者响应
void TdSm::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询交易编码响应
void TdSm::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询合约保证金率响应
void TdSm::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询合约手续费率响应
void TdSm::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询交易所响应
void TdSm::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询产品响应
void TdSm::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询合约响应
void TdSm::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询行情响应
void TdSm::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(CtpUtils::isErrorRsp(pRspInfo,nRequestID))
    {
        LOG(ERROR)<<__FUNCTION__<<","<<pRspInfo->ErrorID;
    }
    else
    {
        LOG(INFO)<<__FUNCTION__<<","<<pDepthMarketData->InstrumentID<<","<<pDepthMarketData->LastPrice;
    }
}

///请求查询投资者结算结果响应
void TdSm::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询转帐银行响应
void TdSm::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询投资者持仓明细响应
void TdSm::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询客户通知响应
void TdSm::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询结算信息确认响应
void TdSm::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询投资者持仓明细响应
void TdSm::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///查询保证金监管系统经纪公司资金账户密钥响应
void TdSm::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询仓单折抵信息响应
void TdSm::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询投资者品种/跨品种保证金响应
void TdSm::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询交易所保证金率响应
void TdSm::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询交易所调整保证金率响应
void TdSm::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询汇率响应
void TdSm::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询二级代理操作员银期权限响应
void TdSm::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询产品报价汇率
void TdSm::OnRspQryProductExchRate(CThostFtdcProductExchRateField *pProductExchRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询产品组
void TdSm::OnRspQryProductGroup(CThostFtdcProductGroupField *pProductGroup, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询做市商合约手续费率响应
void TdSm::OnRspQryMMInstrumentCommissionRate(CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询做市商期权合约手续费响应
void TdSm::OnRspQryMMOptionInstrCommRate(CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询报单手续费响应
void TdSm::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询资金账户响应
void TdSm::OnRspQrySecAgentTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询二级代理商资金校验模式响应
void TdSm::OnRspQrySecAgentCheckMode(CThostFtdcSecAgentCheckModeField *pSecAgentCheckMode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询期权交易成本响应
void TdSm::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询期权合约手续费响应
void TdSm::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询执行宣告响应
void TdSm::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询询价响应
void TdSm::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询报价响应
void TdSm::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询期权自对冲响应
void TdSm::OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询投资单元响应
void TdSm::OnRspQryInvestUnit(CThostFtdcInvestUnitField *pInvestUnit, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询组合合约安全系数响应
void TdSm::OnRspQryCombInstrumentGuard(CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询申请组合响应
void TdSm::OnRspQryCombAction(CThostFtdcCombActionField *pCombAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询转帐流水响应
void TdSm::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询银期签约关系响应
void TdSm::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///错误应答
void TdSm::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///报单通知
void TdSm::OnRtnOrder(CThostFtdcOrderField *pOrder) {}

///成交通知
void TdSm::OnRtnTrade(CThostFtdcTradeField *pTrade) {}

///报单录入错误回报
void TdSm::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {}

///报单操作错误回报
void TdSm::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {}

///合约交易状态通知
void TdSm::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) {}

///交易所公告通知
void TdSm::OnRtnBulletin(CThostFtdcBulletinField *pBulletin) {}

///交易通知
void TdSm::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) {}

///提示条件单校验错误
void TdSm::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) {}

///执行宣告通知
void TdSm::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) {}

///执行宣告录入错误回报
void TdSm::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) {}

///执行宣告操作错误回报
void TdSm::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo) {}

///询价录入错误回报
void TdSm::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo) {}

///报价通知
void TdSm::OnRtnQuote(CThostFtdcQuoteField *pQuote) {}

///报价录入错误回报
void TdSm::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) {}

///报价操作错误回报
void TdSm::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) {}

///询价通知
void TdSm::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {}

///保证金监控中心用户令牌
void TdSm::OnRtnCFMMCTradingAccountToken(CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) {}

///批量报单操作错误回报
void TdSm::OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField *pBatchOrderAction, CThostFtdcRspInfoField *pRspInfo) {}

///期权自对冲通知
void TdSm::OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField *pOptionSelfClose) {}

///期权自对冲录入错误回报
void TdSm::OnErrRtnOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField *pInputOptionSelfClose, CThostFtdcRspInfoField *pRspInfo) {}

///期权自对冲操作错误回报
void TdSm::OnErrRtnOptionSelfCloseAction(CThostFtdcOptionSelfCloseActionField *pOptionSelfCloseAction, CThostFtdcRspInfoField *pRspInfo) {}

///申请组合通知
void TdSm::OnRtnCombAction(CThostFtdcCombActionField *pCombAction) {}

///申请组合录入错误回报
void TdSm::OnErrRtnCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, CThostFtdcRspInfoField *pRspInfo) {}

///请求查询签约银行响应
void TdSm::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询预埋单响应
void TdSm::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询预埋撤单响应
void TdSm::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询交易通知响应
void TdSm::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询经纪公司交易参数响应
void TdSm::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询经纪公司交易算法响应
void TdSm::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询监控中心用户令牌
void TdSm::OnRspQueryCFMMCTradingAccountToken(CThostFtdcQueryCFMMCTradingAccountTokenField *pQueryCFMMCTradingAccountToken, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///银行发起银行资金转期货通知
void TdSm::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) {}

///银行发起期货资金转银行通知
void TdSm::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) {}

///银行发起冲正银行转期货通知
void TdSm::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) {}

///银行发起冲正期货转银行通知
void TdSm::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) {}

///期货发起银行资金转期货通知
void TdSm::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) {}

///期货发起期货资金转银行通知
void TdSm::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) {}

///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
void TdSm::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {}

///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
void TdSm::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {}

///期货发起查询银行余额通知
void TdSm::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) {}

///期货发起银行资金转期货错误回报
void TdSm::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {}

///期货发起期货资金转银行错误回报
void TdSm::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {}

///系统运行时期货端手工发起冲正银行转期货错误回报
void TdSm::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {}

///系统运行时期货端手工发起冲正期货转银行错误回报
void TdSm::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {}

///期货发起查询银行余额错误回报
void TdSm::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) {}

///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
void TdSm::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) {}

///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
void TdSm::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) {}

///期货发起银行资金转期货应答
void TdSm::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///期货发起期货资金转银行应答
void TdSm::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///期货发起查询银行余额应答
void TdSm::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///银行发起银期开户通知
void TdSm::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) {}

///银行发起银期销户通知
void TdSm::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) {}

///银行发起变更银行账号通知
void TdSm::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) {}
