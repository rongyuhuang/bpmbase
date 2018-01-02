#include"ctputils.h"
#include"utils/timeutil.h"
#include"utils/strutil.h"
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
}
