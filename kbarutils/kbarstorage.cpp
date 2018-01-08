#include "kbarstorage.h"
#include "utils/logging.h"
#include"easyctp/ctputils.h"
#include "kbartable.h"
#include <algorithm>

KBarStorage::KBarStorage(int cap)
{
    cap_ = std::max(24 * 60, cap); // 86400

    kbars_.reserve(cap_);
}

KBarStorage::~KBarStorage()
{
    for (auto& x : kbars_) {
        free(x);
    }
    kbars_.clear();
    LOG(INFO) << __FUNCTION__;
}

KBarData* KBarStorage::appendKBar(KBarData* data)
{
    if (kbars_.size() >= (size_t)cap_) {
        LOG(INFO) << __FUNCTION__ << " overflow";
        return nullptr;
    }
//    if(KBarTable::isValidKBarData(data)==false)
//    {
//        LOG(INFO)<<__FUNCTION__<<EasyctpUtil::KBarToString(*data);
//    }
    CHECK(KBarTable::isValidKBarData(data));

    KBarData* newKBar = (KBarData*)malloc(sizeof(KBarData));
    memcpy(newKBar, data, sizeof(KBarData));
    kbars_.push_back(newKBar);

    latestKBar_ = newKBar;
    return newKBar;
}

void KBarStorage::getSnapshot(KBarVector& result)
{
    result.data = kbars_.data();
    result.len = kbars_.size();
}

bool KBarStorage::getLatestKBar(KBarData& result)
{
    if (latestKBar_ != nullptr) {
        memcpy(&result, latestKBar_, sizeof(KBarData));
        return true;
    }
    return false;
}

KBarData* KBarStorage::getLatestKBar()
{
    return latestKBar_;
}
