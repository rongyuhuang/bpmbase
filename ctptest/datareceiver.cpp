#include"datareceiver.h"
#include<fstream>
#include<list>

DataReceiver::DataReceiver(const Config& cfg):
   config(cfg)
{
    initMd();
}
DataReceiver::~DataReceiver()
{
    LOG(INFO)<<__FUNCTION__;
    tickCache.clear();
    tickMap.clear();
}

void DataReceiver::start()
{
    std::lock_guard<std::mutex> lock(tick_mutex);
    quit=false;
    md_start();
    qryTickThread.reset(new std::thread(std::bind(&DataReceiver::tickCollect,this)));


    for(auto& x:config.symbols)
    {
        tickCache[x] = std::vector<TickData>();
        int ret = md_subscribe(x.c_str());
        CHECK(ret ==STATUS_OK);
        LOG(INFO)<<__FUNCTION__<<","<<x;
    }
}

void DataReceiver::stop()
{
    std::lock_guard<std::mutex> lock(tick_mutex);
    LOG(INFO)<<__FUNCTION__;
    quit=true;

    saveTicks();

    for(auto x:config.symbols)
    {
        md_unsubscribe(x.c_str());
    }
    int ret =md_stop();
    CHECK(ret ==STATUS_OK);
    qryTickThread->join();
}

void DataReceiver::onTick(TickData *tick)
{
    std::lock_guard<std::mutex> lock(tick_mutex);
    if(quit)
    {
        return;
    }
    auto& x = tickCache[std::string(tick->symbol)];
    x.push_back(*tick);

//    auto iter = tickMap.find(std::string(tick->symbol));
//    if(iter !=tickMap.end())
//    {
//        LOG(INFO)<<__FUNCTION__<<iter->first;
//        //(*iter)[std::string(tick->symbol)].Put(*tick);
//    }
}

void DataReceiver::tickCollect()
{
    LOG(INFO)<<__FUNCTION__<<" begin...";

    int ret = STATUS_OK;
    RtnData data;
    int loopTime =0;
    while(!quit)
    {
        //LOG(INFO)<<__FUNCTION__<<",loop times="<<++loopTime<<",quit="<<quit;
        ret = md_queryRtnData(&data);
        if(ret== STATUS_OK)
        {
            if(data.rtnDataType!= RTNDATA_TICK)
            {
                LOG(INFO)<<"RTNDATA Type:"<<data.rtnDataType;
            }
            else
            {
                CHECK(data.rtnDataType==RTNDATA_TICK);
                TickData* tick = (TickData*)data.rtnDataPtr;
                onTick(tick);
                ret = md_freeMemory(tick);
                CHECK(ret==STATUS_OK);
                //handle tick
            }
        }
        else if(ret == STATUS_QUIT)
        {
            LOG(INFO)<<__FUNCTION__<<",quit";
        }
        else if(ret ==STATUS_TIMEOUT)
        {
            break;
        }
        else
        {
            break;
        }
    }

    LOG(INFO)<<__FUNCTION__<<" end.";
}

void DataReceiver::initMd()
{
    md_setBrokerInfo(config.mdBrokerID.c_str(),config.mdFront.c_str());
    md_setUserInfo(config.mdUserID.c_str(),config.mdPassword.c_str());
    md_setConfig("mdFlow",10000);
}
void DataReceiver::saveTicks()
{
    LOG(INFO)<<__FUNCTION__<<"...";
    auto begin = tickCache.cbegin();
    while(begin !=tickCache.cend())
    {
        auto sym = begin->first.c_str();
        auto vec = begin->second;
        std::string path= StrUtil::printf("c:/temp/datafeed/tick/%s_%s.txt", sym,md_getTradingDay());
        std::ofstream file;
        file.open(path,std::ios::binary|std::ios::app);
        if(file.is_open()==false)
        {
            continue;
        }
        for(auto & v:vec)
        {
            file.write((char*)&v,sizeof(TickData));
        }
        vec.clear();
        file.close();
        ++begin;
    }
}

void DataReceiver::loadTicks()
{

}
