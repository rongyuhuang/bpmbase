/*对CTP的行情做包装
*/
#ifndef MDSM_H
#define MDSM_H

#include<string>

#include"ThostFtdcMdApi.h"

class MdSm:public CThostFtdcMdSpi{

private:
 CThostFtdcMdApi* mdapi_ = nullptr;

private:
 void OnFrontConnected() override;
 void OnFrontDisconnected(int nReason) override;
 void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
                     CThostFtdcRspInfoField* pRspInfo,
                     int nRequestID,
                     bool bIsLast) override;
 void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout,
                      CThostFtdcRspInfoField* pRspInfo,
                      int nRequestID,
                      bool bIsLast) override;
 void OnRspError(CThostFtdcRspInfoField* pRspInfo,
                 int nRequestID,
                 bool bIsLast) override;
 void OnRspSubMarketData(
     CThostFtdcSpecificInstrumentField* pSpecificInstrument,
     CThostFtdcRspInfoField* pRspInfo,
     int nRequestID,
     bool bIsLast) override;
 void OnRspUnSubMarketData(
     CThostFtdcSpecificInstrumentField* pSpecificInstrument,
     CThostFtdcRspInfoField* pRspInfo,
     int nRequestID,
     bool bIsLast) override;
 void OnRtnDepthMarketData(
     CThostFtdcDepthMarketDataField* pDepthMarketData) override;
};
#endif // MDSM_H
