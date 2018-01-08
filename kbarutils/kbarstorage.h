#ifndef KBARSTORAGE_H
#define KBARSTORAGE_H

#include "easyctp/easyctp.h"
#include <vector>

struct KBarVector {
    KBarData** data; // kbars
    int len; // kbars
    KBarVector()
        : len(0)
        , data(0)
    {
    }
    KBarVector(KBarData** _data, int _len)
        : len(_len)
        , data(_data)
    {
    }
    KBarData* operator[](int i) const { return data[i]; }
};

class KBarStorage {
public:
    KBarStorage& operator=(const KBarStorage&) = delete;
    KBarStorage(const KBarStorage&) = delete;
    KBarStorage(int cap);
    ~KBarStorage();

    KBarData* appendKBar(KBarData* data);
    void getSnapshot(KBarVector& result);
    bool getLatestKBar(KBarData& result); // 是否有数据
    KBarData* getLatestKBar();

private:
    int cap_ = 1440;
    std::vector<KBarData*> kbars_;
    KBarData* latestKBar_ = nullptr;
};

#endif // KBARSTORAGE_H
