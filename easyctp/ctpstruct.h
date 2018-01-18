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


//持仓方向
#define POS_DIRECTION_NET '1'
#define POS_DIRECTION_LONG '2'
#define POS_DIRECTION_SHORT '3'

//买卖方向
#define DIRECTION_BUY '0'
#define DIRECTION_SELL '1'

//投机套保标识
#define HEDGE_SPECULATION '1'
#define HEDGE_ARBITRAGE '2'
#define HEDGE_HEDGE '3'

//成交类型

///组合持仓拆分为单一持仓,初始化不应包含该类型的持仓
#define TRDT_SplitCombination '#'
///普通成交
#define TRDT_Common '0'
///期权执行
#define TRDT_OptionsExecution '1'
///OTC成交
#define TRDT_OTC '2'
///期转现衍生成交
#define TRDT_EFPDerived '3'
///组合衍生成交
#define TRDT_CombinationDerived '4'

////结构体定义
struct AccountInfo{
    /*
     * CThostFtdcTradingAccountField
    */
    char brokerID[STR_LEN_SMALL];
    char accountID[STR_LEN_SMALL];
    double preMortgage;//-上次质押金额
    double preCredit;//- 上次信用额度
    double preDeposit;// + 上次存款额
    double preBalance;//+ 上次结算准备金
    double preMargin;//+ 上次占用的保证金
    double interestBase;// 利息基数
    double deposit;//入金金额
    double withdraw; //出金金额
    double frozenMargin; //冻结保证金
    double frozenCash; //冻结资金
    double frozenCommission;//冻结手续费
    double currMargin; //占用保证金
    double commission; //手续费
    double closeProfit; //平仓盈亏
    double positionProfit; //持仓盈亏
    double balance;//期货结算准备金
    double availabe; //可用资金
    double withdrawQuota;//可取资金

};

struct PositionDetailInfo{
    char brokerID[STR_LEN_SMALL];
    char investorID[STR_LEN_SMALL];
    char instrumentID[STR_LEN_SMALL];
    int hedgeFlag;//投机套保标识? char or int？
    int direction; //买卖
    char openDate[STR_LEN_SMALL];
    char tradeID[STR_LEN_SMALL]; //openDate+tradeID 唯一标志
    double openPrice;
    int volume;//当前持仓量
    int tradeType; //成交类型 int ? char
    double closeProfitByDate;//逐日平仓盈亏
    double closeProfitByTrade;//逐笔平仓盈亏
    double positionProfitByDate;//逐日持仓盈亏
    double positionProfitByTrade;//逐笔平仓盈亏 ？有没有必要维护？
    double margin;
    double presettlementPrice;
    int closeVolume;//平仓量
};

struct TradeInfo{
    char brokerID[STR_LEN_SMALL];
    char investorID[STR_LEN_SMALL];
    char instrumentID[STR_LEN_SMALL];
    char orderRef[STR_LEN_SMALL];//报单引用
    int hedgeFlag;//投机套保标识? char or int？
    int offsetFlag;//开平标识
    int direction; //买卖
    double price;
    int volume;
    char tradeDate[STR_LEN_SMALL];//成交日期,格式为
    char tradeTime[STR_LEN_SMALL];//成交时间，格式为
    char tradeID[STR_LEN_SMALL]; //openDate+tradeID 唯一标志
    int tradeType;
    char exchangeID[STR_LEN_SMALL];
    char orderSysID[STR_LEN_SMALL]; //报单编号(交易所返回)
};

struct OrderInfo
{
    char brokerID[STR_LEN_SMALL];
    char investorID[STR_LEN_SMALL];
    char instrumentID[STR_LEN_SMALL];
    char orderRef[STR_LEN_SMALL];//报单引用
    int hedgeFlag;//投机套保标识? char or int？
    int offsetFlag;//开平标识
    int direction; //买卖
    double limitPrice;
    int volume;
    int orderStatus;
    long long insertDatetime;
    int tradedVolume;//已成交数量
    int remainVolume; //未成交数量
    double frozenMargin;
    double frozenCommission;

};

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
