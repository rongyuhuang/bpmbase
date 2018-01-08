/*CTP和easyctp中转的结构体，及easyctp的相关结构体
 *
*/
#ifndef CTPSTRUCT_H
#define CTPSTRUCT_H


///常量定义

#define STR_LEN_BIG 64   //长字符串
#define STR_LEN_SMALL 16 //短字符串

//品种类别
#define PRODUCT_CLASS_FUTURES '1'
#define PRODUCT_CLASS_OPTIONS '2' //期货期权
#define PRODUCT_CLASS_COMBINATION '3' //组合
#define PRODUCT_CLASS_SPOT '4' //即期
#define PRODUCT_CLASS_EFP '5' //期转现
#define PRODUCT_CLASS_SPOTOPTION '6' //现货期权

//组合类别
#define COMBINE_BUL '1' //垂直价差BUL
#define COMBINE_BER '2' //垂直价差BER
#define COMBINE_STD '3' //跨式组合
#define COMBINE_STG '4' //宽跨式组合
#define COMBINE_PRT '5' //备兑组合
#define COMBINE_CLD '6' //时间价差组合

////结构体定义

struct InstrumentInfo{
    char symbol[STR_LEN_SMALL];
    char name[STR_LEN_SMALL];
    char product[STR_LEN_SMALL];
    char exchange[STR_LEN_SMALL];
    char productClass;
    int volumeMultiple; //合约乘数
    double priceTick; //最小变动价位
    int maxLimitOrder; //限价单最大下单量
    int minLimitOrder; //
    int maxMarketOrder;
    int minMarketOrder;
    char combineType; //组合类型

};
struct TickData{
    char symbol[STR_LEN_SMALL];
    long long actionDatetime;
    int updateMS;
    int res1;
    double lastPrice;
    int volume;
    int totalVolume;
    int openInterest;
    double bidPrice;
    int bidVolume;
    double askPrice;
    int askVolume;
    //日K信息
    double openPrice;
    double highPrice;
    double lowPrice;
    double closePrice;
    double settlementPrice;
    double preClosePrice;
    double preSettlementPrice;
    double averagePrice;
    double upperLimit;
    double lowerLimit;

};

struct KBarData{
    char symbol[STR_LEN_SMALL];
    long long actionDatetime;
    int volume;
    double openInterest;
    double openPrice;
    double highPrice;
    double lowPrice;
    double closePrice;
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
