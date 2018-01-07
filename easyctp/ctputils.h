/*CTP工具方法
 *包括Tick转换、CTP SDK中数据与easyctp的数据转换等
*/
#include <iostream>
#include<string>

#include"ThostFtdcUserApiStruct.h"
#include"ctpstruct.h"
namespace CtpUtils {


const std::string Tick2Str(const TickData& tick);

void Str2Tick(const std::string tickLine);

bool isErrorRsp(CThostFtdcRspInfoField* rspInfo,int reqID);

void instrument2product(const char* instrument,char* product);
}
