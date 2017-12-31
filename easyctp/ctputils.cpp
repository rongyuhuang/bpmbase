#include"ctputils.h"

namespace CtpUtils {

const std::string Tick2Str()
{
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
