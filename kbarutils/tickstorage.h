#ifndef TICKTSTORAGE_H
#define TICKTSTORAGE_H

#include "easyctp/easyctp.h"

#include <vector>

//
// 在这里不要计算各个周期的bar，只提供最细粒度的如1分钟的就行了
// 计算各周期的bar，应该由信号系统自己去做，每个算法的要求都不一样
// 信号系统是独立并行线程集合，不是事件驱动，这里算了也没用
//

struct TickVector {
    TickData** data; // ticks
    int len; // ticks

    // 当日统计数据
    double openPrice = 0.0; // 今日开盘价
    double highPrice = 0.0; // 今日最高价
    double lowPrice = 0.0; // 今日最低价
    double preClosePrice = 0.0; // 昨日收盘价
    double upperLimit = 0.0; // 涨停价
    double lowerLimit = 0.0; // 跌停价

    TickVector()
        : len(0)
        , data(0)
    {
    }
    TickVector(TickData** _data, int _len)
        : len(_len)
        , data(_data)
    {
    }
    TickData* operator[](int i) const { return data[i]; }
};

class TickStorage {
public:
    TickStorage& operator=(const TickStorage&) = delete;
    TickStorage(const TickStorage&) = delete;
    TickStorage(int cap);
    ~TickStorage();

    TickData* appendTick(TickData* data);
    void getSnapshot(TickVector& result);
    bool getLatestTick(TickData& result); // 是否有数据
    TickData* getLatestTick();

    double openPrice();
    double highPrice();
    double lowPrice();
    double preClosePrice();
    double upperLimit();
    double lowerLimit();

private:
    std::vector<TickData*> ticks_; // ticks
    double openPrice_ = 0; // 今日开盘价
    double highPrice_ = 0; // 今日最高价
    double lowPrice_ = 0; // 今日最低价
    double preClosePrice_ = 0; // 昨日收盘价
    double upperLimit_ = 0; // 涨停价
    double lowerLimit_ = 0; // 跌停价

private:
    int cap_ = 86400;
    TickData* latestTick_ = nullptr;
};

#endif // TICKTSTORAGE_H
