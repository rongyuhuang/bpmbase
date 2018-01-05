#include"datareceiver.h"
#include<fstream>
#include<list>
void DataReceiver::start()
{
    md_start();
    qryTickThread.reset(new std::thread(std::bind(&DataReceiver::tickCollect,this)));


    for(auto x:config.symbols)
    {
        md_subscribe(x.c_str());
    }
}

void DataReceiver::stop()

{
    LOG(INFO)<<"DataReceiver descontruct...";
    for(auto x:config.symbols)
    {
        md_unsubscribe(x.c_str());
    }
    md_stop();
    qryTickThread->join();
    //        for(auto x:tickMap)
    //        {
    //            delete x.second;
    //        }
    tickMap.clear();
}

void DataReceiver::onTick(TickData *tick)
{
    std::lock_guard<std::mutex> lock(tick_mutex);
    auto iter = tickMap.find(std::string(tick->symbol));
    if(iter !=tickMap.end())
    {
        LOG(INFO)<<__FUNCTION__<<iter->first;
        //(*iter)[std::string(tick->symbol)].Put(*tick);
    }
}

void DataReceiver::tickCollect()
{
    LOG(INFO)<<__FUNCTION__<<" begin...";

    int ret = STATUS_OK;
    RtnData data;
    int loopTime =0;
    while(!quit)
    {
        LOG(INFO)<<__FUNCTION__<<",loop times="<<++loopTime<<",quit="<<quit;
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
    md_setBrokerInfo(config.brokerID.c_str(),config.mdFront.c_str());
    md_setUserInfo(config.userID.c_str(),config.password.c_str());
    md_setConfig("mdFlow",2000);
}
void DataReceiver::saveTicks()
{
    auto item = tickMap.cbegin();
    while(item!=tickMap.cend())
    {
        auto sym = item->first.c_str();
        auto tickQueue = (item->second);
        std::list<TickData> ticks;
        tickQueue.Take(ticks);
//        (item->second).Take(ticks);
//        std::string path = StrUtil::printf("c:/temp/datafeed/tick/%s_%s.txt", sym,
//                                           md_getTradingDay());
//        std::ofstream file;
//        file.open(path,std::ios::binary|std::ios::app);
//        if(file.is_open()==false)
//        {
//            continue;
//        }
//        for(auto tick :ticks)
//        {
//            file.write((char*)&tick,sizeof(TickData));
//        }
//        ticks.clear();
//        file.close();
    }
}
