#include"ctputils.h"
#include"utils/timeutil.h"
#include"utils/strutil.h"

#include <algorithm>
#include <cmath>
#include<cctype>

namespace CtpUtils {

const std::string Tick2Str(const TickData& tick)
{

    std::string tickLine=StrUtil::printf("%s");
    return nullptr;
}

void Str2Tick(const std::string tickLine)
{

}

bool isErrorRsp(CThostFtdcRspInfoField *rspInfo, int reqID)
{
    if(rspInfo && rspInfo->ErrorID !=0)
    {
        return true;
    }
    return false;
}

void instrument2product(const char* instrument,char* product)
{
    int idx=0;
    while(*instrument)
    {
        if(std::isalpha(*instrument))
        {
            product[idx] = *instrument;
            ++instrument;
            idx++;
        }
        else
        {
            return;
        }
    }
}
bool isValidTickData(TickData* currTick, bool checkDatetime)
{
    if (checkDatetime) {
        if (std::abs(std::time(nullptr) - currTick->actionDatetime) > 3 * 60) {
            return false;
        }
    }

    // 价格检查
    if (currTick->lastPrice< 0 ||currTick->lowerLimit< 0
        ||currTick->upperLimit< 0 ||currTick->askPrice< 0
        ||currTick->bidPrice< 0 ||currTick->openPrice< 0
        ||currTick->highPrice< 0 ||currTick->lowPrice< 0
        ||currTick->preClosePrice< 0 ||currTick->averagePrice< 0
        ||currTick->preSettlementPrice< 0 )
        //||currTick->settlementPrice)||currTick->closePrice) //交易时段中，这两个字段为MAX_DOUBLE
    {
        return false;
    }

    // 成交量 持仓检查
    if (currTick->askVolume < 0 || currTick->bidVolume < 0
        || currTick->totalVolume < 0 || currTick->openInterest < 0.0) {
        return false;
    }

    return true;
}
}
