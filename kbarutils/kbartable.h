#ifndef KBARTABLE_H
#define KBARTABLE_H

#include "easyctp/easyctp.h"
#include "kbarstorage.h"
#include "mempiece.h"

#include <vector>

struct KBarTableSnapshot {
    DoubleVector actionDatetime; // 时间,time_t
    DoubleVector volume; // 本bar的成交量
    // DoubleVector totalVolume;     // 总成交量
    DoubleVector openInterest; // 持仓量
    DoubleVector openPrice; // 本bar的开盘价
    DoubleVector closePrice; // 本bar的收盘价
    DoubleVector highPrice; // 本bar的最高价
    DoubleVector lowPrice; // 本bar的最低价
};

//
// 主周期：可以直接appendBar，如果是从子周期合并而来，可以用appendSubBar
// 子周期：tick计算出一个子周期bar，然后appendSubBar;也可以直接调用
//
class KBarTable {
public:
    KBarTable& operator=(const KBarTable&) = delete;
    KBarTable(const KBarTable&) = delete;
    KBarTable() = delete;
    KBarTable(int period, int cap, int subPeriod);
    ~KBarTable();

    void getSnapshot(KBarTableSnapshot& result);
    bool getLatestKBar(KBarData& result, bool& done); //是否有数据,是否完成
    void appendKBar(KBarData* data);
    bool appendSubKBar(KBarData* data, KBarData* newBar = nullptr);
    bool appendTick(TickData* data, KBarData* newBar = nullptr);
    void finish(); //尾巴处理

    // result内部已经指定了主周期和子周期
    static void resample(const char* symbol,
        const KBarVector& src,
        KBarTable& result);
    static void resample(const char* symbol,
        const KBarTableSnapshot& src,
        KBarTable& result);
    static bool isValidKBarData(KBarData* data);
    static bool isValidTickData(TickData* currTick, bool checkDatetime);

    DoubleVector actionDatetime();
    DoubleVector volume();
    // DoubleVector totalVolume();
    DoubleVector openInterest();
    DoubleVector openPrice();
    DoubleVector highPrice();
    DoubleVector lowPrice();
    DoubleVector closePrice();

private:
    bool newSubBar(long long tickDatetime, long long& newSubBarDatetime);

private:
    std::vector<double> actionDatetime_; // 时间,time_t
    std::vector<double> volume_; // 本bar的成交量
    // std::vector<double> totalVolume_;     // 总成交量
    std::vector<double> openInterest_; // 持仓量
    std::vector<double> openPrice_; // 本bar的开盘价
    std::vector<double> closePrice_; // 本bar的收盘价
    std::vector<double> highPrice_; // 本bar的最高价
    std::vector<double> lowPrice_; // 本bar的最低价

private:
    int period_ = 60; // 主周期
    int subPeriod_ = 60; // 子周期
    int cap_ = 1440;
    KBarData latestKBar_;
    int countOfSubKBar_ = 0; // 已经累加了多少个子周期bar
    KBarData latestSubKBar_;
};

#endif // KBARTABLE_H
