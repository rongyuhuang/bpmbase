#include "tickstorage.h"
#include "utils/logging.h"
#include "kbartable.h"
#include <algorithm>

TickStorage::TickStorage(int cap)
{
    cap_ = std::max(24 * 60 * 60, cap); // 86400

    ticks_.reserve(cap_);
}

TickStorage::~TickStorage()
{
    for (auto& x : ticks_) {
        free(x);
    }
    ticks_.clear();
    LOG(INFO) << __FUNCTION__;
}

TickData* TickStorage::appendTick(TickData* data)
{
    if (ticks_.size() >= (size_t)cap_) {
        LOG(INFO) << __FUNCTION__ << " overflow";
        return nullptr;
    }
    CHECK(KBarTable::isValidTickData(data, false));

    TickData* newTick = (TickData*)malloc(sizeof(TickData));
    memcpy(newTick, data, sizeof(TickData));
    ticks_.push_back(newTick);

    openPrice_ = data->openPrice;
    highPrice_ = data->highPrice;
    lowPrice_ = data->lowPrice;
    preClosePrice_ = data->preClosePrice;
    upperLimit_ = data->upperLimit;
    lowerLimit_ = data->lowerLimit;

    latestTick_ = newTick;
    return newTick;
}

void TickStorage::getSnapshot(TickVector& result)
{
    result.data = ticks_.data();
    result.len = ticks_.size();

    result.openPrice = openPrice();
    result.highPrice = highPrice();
    result.lowPrice = lowPrice();
    result.preClosePrice = preClosePrice();
    result.upperLimit = upperLimit();
    result.lowerLimit = lowerLimit();
}

bool TickStorage::getLatestTick(TickData& result)
{
    if (latestTick_ != nullptr) {
        memcpy(&result, latestTick_, sizeof(TickData));
        return true;
    }
    return false;
}

TickData* TickStorage::getLatestTick()
{
    return latestTick_;
}

double TickStorage::openPrice()
{
    return openPrice_;
}

double TickStorage::highPrice()
{
    return highPrice_;
}

double TickStorage::lowPrice()
{
    return lowPrice_;
}

double TickStorage::preClosePrice()
{
    return preClosePrice_;
}

double TickStorage::upperLimit()
{
    return upperLimit_;
}

double TickStorage::lowerLimit()
{
    return lowerLimit_;
}
