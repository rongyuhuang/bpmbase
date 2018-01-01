/*封装EASYCTP行情部分对外的接口
*/
#include"mdsm.h"
#include"easyctp.h"
#include"utils/fileutil.h"
#include"utils/strutil.h"
#include"utils/logging.h"
#include<thread>
#include<chrono>

static MdSm g_mdSm;

int timeout=10000;
int default_timeout = 10000;


bool checkStatus()
{
    int step =100;
    std::chrono::milliseconds span(step);
    for(int i=0;i<timeout;i+=step)
    {
        std::this_thread::sleep_for(span);
        if(g_mdSm.sync_status==2)
        {
            return true;
        }
    }
    return false;
}
///easyctp的MD API

void __cdecl md_setBrokerInfo(const char* brokerID,const char* mdFront)
{
    LOG(INFO)<<__FUNCTION__<<",BrokerID:"<<brokerID<<",Front:"<<mdFront;
    g_mdSm.brokerID = brokerID;
    g_mdSm.mdFront = mdFront;
}

void __cdecl md_setUserInfo(const char* userID,const char* password)
{
    LOG(INFO)<<__FUNCTION__<<",UserID:"<<userID<<",Password:******";
    g_mdSm.userID= userID;
    g_mdSm.password=password;
}

void __cdecl md_setConfig(const char* flowPath,int apiTimeout)
{
    LOG(INFO)<<__FUNCTION__<<",FlowPath:"<<flowPath<<",ApiTimeout:"<<apiTimeout;
    //CTP API在设置流文件夹时，目录需要后跟"/"
    g_mdSm.mdFlowPath = StrUtil::printf("%s/",flowPath);
    bpm_createDir(flowPath);

    timeout = (apiTimeout>default_timeout)?apiTimeout:default_timeout;
    g_mdSm.apiTimeout = timeout;
}

int __cdecl md_start()
{
    if(g_mdSm.started)
    {
        return STATUS_RUNNING;
    }
    if(g_mdSm.sync_status>0)
    {
        return STATUS_BUSY;
    }
    g_mdSm.start();
    if(checkStatus())
    {
        g_mdSm.sync_status=0;
        return STATUS_OK;
    }
    return STATUS_TIMEOUT;

}
const char* __cdecl md_getTradingDay()
{
    if(!g_mdSm.started)
    {
        return nullptr;
    }
    return g_mdSm.getTradingDay();
}
int __cdecl md_subscribe(const char* instrument)
{
    if(!g_mdSm.started)
    {
        return STATUS_STOPPED;
    }
    if(g_mdSm.sync_status>0)
    {
        return STATUS_BUSY;
    }
    g_mdSm.subscribe(instrument);
    if(checkStatus())
    {
        g_mdSm.sync_status=0;
        return STATUS_OK;
    }
    return STATUS_TIMEOUT;
}
int __cdecl md_unsubscribe(const char* instrument)
{
    if(!g_mdSm.started)
    {
        return STATUS_STOPPED;
    }
    g_mdSm.unsubscribe(instrument);
    if(checkStatus())
    {
        return STATUS_OK;
    }
    return STATUS_TIMEOUT;
}
int __cdecl md_queryRtnData(RtnData* rtnData)
{
    if(!g_mdSm.started)
    {
        return STATUS_STOPPED;
    }
    std::unique_lock<std::mutex> lock(g_mdSm.ticks_mutex);
    while(g_mdSm.ticks.empty())
    {
        g_mdSm.need_notify = true;
        std::chrono::milliseconds span(timeout);
        auto status = g_mdSm.ticks_cv.wait_for(lock,span);
        g_mdSm.need_notify=false;
        if(g_mdSm.quit_flag)
        {
            return STATUS_QUIT;
        }
        if(status == std::cv_status::timeout)
        {
            return STATUS_TIMEOUT;
        }
    }
    //转换数据
    void* data = nullptr;
    data = g_mdSm.ticks.front();
    g_mdSm.ticks.pop_back();
    rtnData->rtnDataType = *(int*)data;
    rtnData->rtnDataPtr = (void*)((char*)data+4);//从data的首地址向后移四位
    return STATUS_OK;
}
int __cdecl md_freeMemory(void* ptr)
{
    void* buff = (void*)((char*)ptr-4);
    free(buff);
    return STATUS_OK;
}
int __cdecl md_stop()
{
    if(!g_mdSm.started)
    {
        return STATUS_STOPPED;
    }
    g_mdSm.stop();
    if(checkStatus())
    {
        return STATUS_OK;
    }
    return STATUS_TIMEOUT;
}
