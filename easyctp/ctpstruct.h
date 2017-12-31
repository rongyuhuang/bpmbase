/*CTP和easyctp中转的结构体，及easyctp的相关结构体
 *
*/
#ifndef CTPSTRUCT_H
#define CTPSTRUCT_H


///常量定义

#define STR_LEN_BIG 64   //长字符串
#define STR_LEN_SMALL 16 //短字符串



////结构体定义


struct TickData{
    char symbol[STR_LEN_SMALL];
    long long actionDatetime;
    int updateMS;
    int res1;
    double lastPrice;
    int volume;
    int totalVolume;
    int openInterest;
    double bid1Price;
    int bid1Volume;
    double ask1Price;
    int ask1Volume;
    //日K信息
    double openPrice;
    double highPrice;
    double lowPrice;
    double closePrice;
    double settlePrice;
    double preclosePrice;
    double presettlePrice;
    double averagePrice;
    double upperLimitPrice;
    double lowerLimitPrice;

};

struct KBarData{

};


////easyctp 相关定义
///

//状态
#define STATUS_OK 0
#define STATUS_QUIT -1
#define STATUS_TIMEOUT -2
#define STATUS_INVALIDPARAMS -3
#define STATUS_RUNNING -4
#define STATUS_BUSY -5 //系统在处理上一个请求
#define STATUS_STOPPED -6
#define STATUS_EMPTY -7 //队列为空

//推送数据类型
#define RTNDATA_TICK 1  //深度行情
#define RTNDATA_QUOTE 2 //询价
#define RTNDATA_ORDER 3 //委托回报
#define RTNDATA_TRADE 4 //成交回报
#define RTNDATA_BANK  5 //出入金通知
#define RTNDATA_STATUS 6 //交易状态推送
//还有期权和做市方面的的RTNDATA类型

struct RtnData{
    int rtnDataType; //回报数据类型
    void* rtnDataPtr; //回报数据指针
};
#endif // CTPSTRUCT_H
