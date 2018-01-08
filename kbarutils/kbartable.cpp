#include "kbartable.h"
#include "utils/logging.h"

#include "utils/timeutil.h"
#include <algorithm>
#include <cmath>

KBarTable::KBarTable(int period, int cap, int subPeriod)
{
    CHECK(period > 0 && subPeriod > 0 && period % subPeriod == 0);
    CHECK(subPeriod <= 300 || (period == 86400 && subPeriod == 86400));

    period_ = period;
    subPeriod_ = subPeriod;
    cap_ = std::max((int)ceil((double)86400 / period), cap);
    actionDatetime_.reserve(cap_);
    volume_.reserve(cap_);
    // totalVolume_.reserve(cap_);
    openInterest_.reserve(cap_);
    openPrice_.reserve(cap_);
    highPrice_.reserve(cap_);
    lowPrice_.reserve(cap_);
    closePrice_.reserve(cap_);

    memset(&latestKBar_, 0, sizeof(KBarData));
    memset(&latestSubKBar_, 0, sizeof(KBarData));
}

KBarTable::~KBarTable()
{
    LOG(INFO) << __FUNCTION__;
}

void KBarTable::appendKBar(KBarData* data)
{
    CHECK(data->symbol[0] != 0);
    if (actionDatetime_.size() >= (size_t)cap_) {
        LOG(INFO) << __FUNCTION__ << " overflow";
        return;
    }
    CHECK(isValidKBarData(data));

    actionDatetime_.push_back(data->actionDatetime);
    volume_.push_back(data->volume);
    // totalVolume_.push_back(data->totalVolume);
    openInterest_.push_back(data->openInterest);

    openPrice_.push_back(data->openPrice);
    highPrice_.push_back(data->highPrice);
    lowPrice_.push_back(data->lowPrice);
    closePrice_.push_back(data->closePrice);

    memcpy(&latestKBar_, data, sizeof(KBarData));
    countOfSubKBar_ = 0;
}

bool KBarTable::appendSubKBar(KBarData* data, KBarData* newBar)
{
    CHECK(data->symbol[0] != 0);
    CHECK(isValidKBarData(data));
    bool isNew = false;

    //相同周期直接插入
    if (subPeriod_ == period_) {
        appendKBar(data);
        isNew = true;
        if (newBar) {
            memcpy(newBar, data, sizeof(KBarData));
        }
        return isNew;
    }

    // 跨日的直接当之前的完成并开启新bar
    if (!bpm_sametradingday(latestKBar_.actionDatetime, data->actionDatetime)) {
        // 新bar，插入老的,把第一个空的排除
        if (countOfSubKBar_ > 0) {
            CHECK(latestKBar_.symbol[0] != 0);
            appendKBar(&latestKBar_);
            isNew = true;
            if (newBar) {
                memcpy(newBar, &latestKBar_, sizeof(KBarData));
            }
        }
        memset(&latestKBar_, 0, sizeof(KBarData));
        strncpy(latestKBar_.symbol, data->symbol, sizeof(latestKBar_.symbol) - 1);
        latestKBar_.actionDatetime = floor((double)data->actionDatetime / 60) * 60;
        latestKBar_.openPrice = data->openPrice;
        latestKBar_.highPrice = data->highPrice;
        latestKBar_.lowPrice = data->lowPrice;
        latestKBar_.closePrice = data->closePrice;
        latestKBar_.volume = data->volume;
        // latestKBar_.totalVolume = data->totalVolume;
        latestKBar_.openInterest = data->openInterest;
        countOfSubKBar_ = 1;

        return isNew;
    }

    // 累加
    if (countOfSubKBar_ == 0) {
        memset(&latestKBar_, 0, sizeof(KBarData));
        strncpy(latestKBar_.symbol, data->symbol, sizeof(latestKBar_.symbol) - 1);
        latestKBar_.actionDatetime = floor((double)data->actionDatetime / 60) * 60;
        latestKBar_.openPrice = data->openPrice;
        latestKBar_.highPrice = data->highPrice;
        latestKBar_.lowPrice = data->lowPrice;
        latestKBar_.closePrice = data->closePrice;
        latestKBar_.volume = data->volume;
        // latestKBar_.totalVolume = data->totalVolume;
        latestKBar_.openInterest = data->openInterest;
        countOfSubKBar_++;
    }
    else {
        CHECK(latestKBar_.symbol[0] != 0);
        latestKBar_.highPrice = std::max(latestKBar_.highPrice, data->highPrice);
        latestKBar_.lowPrice = std::min(latestKBar_.lowPrice, data->lowPrice);
        latestKBar_.closePrice = data->closePrice;
        latestKBar_.volume += data->volume;
        // latestKBar_.totalVolume = data->totalVolume;
        latestKBar_.openInterest = data->openInterest;
        countOfSubKBar_++;
    }
    if (countOfSubKBar_ == period_ / subPeriod_) {
        CHECK(latestKBar_.symbol[0] != 0);
        appendKBar(&latestKBar_);
        isNew = true;
        if (newBar) {
            memcpy(newBar, &latestKBar_, sizeof(KBarData));
        }
        countOfSubKBar_ = 0;
    }
    return isNew;
}

bool KBarTable::appendTick(TickData* data, KBarData* newBar)
{
    CHECK(data->symbol[0] != 0);
    CHECK(isValidTickData(data, false));
    long long newSubBarDateTime = 0;
    bool isNew = false;

    // 不是新bar
    if (!newSubBar(data->actionDatetime, newSubBarDateTime)) {
        latestSubKBar_.volume += data->volume;
        // latestSubKBar_.totalVolume = data->totalVolume;
        latestSubKBar_.openInterest = data->openInterest;

        latestSubKBar_.highPrice = std::max(data->lastPrice, latestSubKBar_.highPrice);
        latestSubKBar_.lowPrice = std::min(data->lastPrice, latestSubKBar_.lowPrice);
        latestSubKBar_.closePrice = data->lastPrice;
    }
    else {
        // 新bar，插入老的,把第一个空的排除
        if (latestSubKBar_.symbol[0] != 0) {
            isNew = appendSubKBar(&latestSubKBar_, newBar);
        }

        // 新开
        memset(&latestSubKBar_, 0, sizeof(KBarData));
        strncpy(latestSubKBar_.symbol, data->symbol, sizeof(latestSubKBar_.symbol));
        latestSubKBar_.actionDatetime = newSubBarDateTime;
        latestSubKBar_.volume = data->volume;
        // latestSubKBar_.totalVolume = data->totalVolume;
        latestSubKBar_.openInterest = data->openInterest;
        latestSubKBar_.openPrice = data->lastPrice;
        latestSubKBar_.highPrice = data->lastPrice;
        latestSubKBar_.lowPrice = data->lastPrice;
        latestSubKBar_.closePrice = data->lastPrice;
    }

    return isNew;
}

void KBarTable::finish()
{
    if (latestSubKBar_.symbol[0] != 0) {
        appendSubKBar(&latestSubKBar_);
    }
    memset(&latestSubKBar_, 0, sizeof(KBarData));
    if (latestKBar_.symbol[0] != 0 && countOfSubKBar_ != 0) {
        appendKBar(&latestKBar_);
    }
    memset(&latestKBar_, 0, sizeof(KBarData));
    countOfSubKBar_ = 0;
}

void KBarTable::getSnapshot(KBarTableSnapshot& result)
{
    result.actionDatetime = actionDatetime();
    result.volume = volume();
    // result.totalVolume_ = totalVolume();
    result.openInterest = openInterest();

    result.openPrice = openPrice();
    result.highPrice = highPrice();
    result.lowPrice = lowPrice();
    result.closePrice = closePrice();
}

bool KBarTable::getLatestKBar(KBarData& result, bool& done)
{
    memcpy(&result, &latestKBar_, sizeof(KBarData));
    done = (countOfSubKBar_ == 0);
    return (latestKBar_.symbol[0] != 0);
}

// static
void KBarTable::resample(const char* symbol,
    const KBarTableSnapshot& src,
    KBarTable& result)
{
    // 加入子bar
    KBarData bar;
    memset(&bar, 0, sizeof(KBarData));
    for (int i = 0; i < src.actionDatetime.len; i++) {
        memset(&bar, 0, sizeof(KBarData));
        strncpy(bar.symbol, symbol, sizeof(bar.symbol) - 1);
        bar.actionDatetime = floor((double)src.actionDatetime[i] / 60) * 60;
        bar.openPrice = src.openPrice[i];
        bar.highPrice = src.highPrice[i];
        bar.lowPrice = src.lowPrice[i];
        bar.closePrice = src.closePrice[i];
        bar.volume = src.volume[i];
        // bar.totalVolume = src.totalVolume[i];
        bar.openInterest = src.openInterest[i];
        result.appendSubKBar(&bar);
    }
    // 尾巴处理
    result.finish();
}

// static
void KBarTable::resample(const char* symbol,
    const KBarVector& src,
    KBarTable& result)
{
    // 加入子bar
    for (int i = 0; i < src.len; i++) {
        result.appendSubKBar(src.data[i]);
    }
    // 尾巴处理
    result.finish();
}

bool KBarTable::isValidKBarData(KBarData* data)
{
    if (!std::isnormal(data->closePrice) || !std::isnormal(data->highPrice)
        || !std::isnormal(data->openPrice) || !std::isnormal(data->lowPrice)) {
        return false;
    }

    if (data->volume < 0 || data->openInterest < 0 || data->closePrice <= 0
        || data->openPrice <= 0 || data->highPrice <= 0 || data->lowPrice <= 0) {
        return false;
    }

    return true;
}

bool KBarTable::isValidTickData(TickData* currTick, bool checkDatetime)
{
    if (checkDatetime) {
        if (std::abs(std::time(nullptr) - currTick->actionDatetime) > 3 * 60) {
            return false;
        }
    }

    // 价格检查
    if (!std::isnormal(currTick->lastPrice) || !std::isnormal(currTick->lowerLimit)
        || !std::isnormal(currTick->upperLimit) || !std::isnormal(currTick->askPrice)
        || !std::isnormal(currTick->bidPrice) || !std::isnormal(currTick->openPrice)
        || !std::isnormal(currTick->highPrice) || !std::isnormal(currTick->lowPrice)
        || !std::isnormal(currTick->preClosePrice) || !std::isnormal(currTick->averagePrice)
        || !std::isnormal(currTick->preSettlementPrice) || !std::isnormal(currTick->settlementPrice)) {
        return false;
    }

    // 成交量 持仓检查
    if (currTick->askVolume < 0 || currTick->bidVolume < 0
        || currTick->totalVolume < 0 || currTick->openInterest < 0.0) {
        return false;
    }

    return true;
}

DoubleVector KBarTable::actionDatetime()
{
    return DoubleVector(actionDatetime_.data(), actionDatetime_.size());
}

DoubleVector KBarTable::volume()
{
    return DoubleVector(volume_.data(), volume_.size());
}

// DoubleVector KBarTable::totalVolume() {
//  return DoubleVector(totalVolume_.data(), totalVolume_.size());
//}

DoubleVector KBarTable::openInterest()
{
    return DoubleVector(openInterest_.data(), openInterest_.size());
}

DoubleVector KBarTable::openPrice()
{
    return DoubleVector(openPrice_.data(), openPrice_.size());
}

DoubleVector KBarTable::highPrice()
{
    return DoubleVector(highPrice_.data(), highPrice_.size());
}

DoubleVector KBarTable::lowPrice()
{
    return DoubleVector(lowPrice_.data(), lowPrice_.size());
}

DoubleVector KBarTable::closePrice()
{
    return DoubleVector(closePrice_.data(), closePrice_.size());
}

//////

bool KBarTable::newSubBar(long long tickDatetime,
    long long& newSubBarDatetime)
{
    CHECK(subPeriod_ <= 300 || (period_ == 86400 && subPeriod_ == 86400));
    if (floor((double)tickDatetime / subPeriod_) == floor(latestSubKBar_.actionDatetime / subPeriod_)) {
        return false;
    }
    else {
        newSubBarDatetime = floor((double)tickDatetime / subPeriod_) * subPeriod_;
        return true;
    }
}
