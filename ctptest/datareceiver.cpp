#include"datareceiver.h"
#include<fstream>
#include<list>
#include <iostream>
#include<thread>
#include<chrono>
#include<iomanip>
#include<ctime>
#include<map>
#include<string>
#include<vector>
#include<functional>
#include<algorithm>

#include"easyctp/easyctp.h"
#include"easyctp/ctputils.h"
#include"utils/logging.h"
#include"utils/fileutil.h"
#include"utils/crashdump.h"
#include"utils/timeutil.h"
#include<fmt/format.h>

DataReceiver::DataReceiver(const Config& cfg):
   config(cfg)
{
    prepareMd();
    prepareTd();
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
    initSubscribePool();
    quit=false;
    md_start();
    qryTickThread.reset(new std::thread(std::bind(&DataReceiver::tickCollect,this)));


    for(auto x:subscribePool)
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

    for(auto x:subscribePool)
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
}

void DataReceiver::tickCollect()
{
    LOG(INFO)<<__FUNCTION__<<" begin...";

    int ret = STATUS_OK;
    RtnData data;
    while(!quit)
    {
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

void DataReceiver::prepareMd()
{
    md_setBrokerInfo(config.mdBrokerID.c_str(),config.mdFront.c_str());
    md_setUserInfo(config.mdUserID.c_str(),config.mdPassword.c_str());
    md_setConfig("mdFlow",10000);
}
void DataReceiver::prepareTd()
{
    td_setBrokerInfo(config.tdBrokerID.c_str(),config.tdFront.c_str());
    td_setUserInfo(config.tdUserID.c_str(),config.tdPassword.c_str());
    td_setConfig("tdFlow",20000,1,6);
}

void DataReceiver::initSubscribePool()
{
    td_start();
    TickData* mdSnap=nullptr;
    int count=0;
    int ret =td_queryMarketData(&mdSnap,&count);
    //LOG(INFO)<<fmt::format("td_queryMarketData()={},count={}",ret,count);
    std::map<std::string,std::vector<TickData>> prodOiMap;

    for(int i = 0;i<count;++i)
    {
        char prod[3];
        //LOG(INFO)<<fmt::format("{} Volume={} , OI={}", mdSnap[i].symbol, mdSnap[i].totalVolume, mdSnap[i].openInterest);
        CtpUtils::instrument2product(mdSnap[i].symbol,prod);
        std::string productID = std::string(prod);
        if(std::find(config.symbols.cbegin(),config.symbols.cend(),productID)
                ==config.symbols.cend())
        {
            continue;
        }
        auto iter = prodOiMap.find(productID);
        if(iter==prodOiMap.end())
        {
           prodOiMap[productID] = std::vector<TickData>{mdSnap[i]};
        }
        else
        {
            prodOiMap[productID].push_back(mdSnap[i]);
        }
        memset(prod,0,sizeof(prod)/sizeof(char));
    }
    std::map<std::string,double> prodWeight;
    auto begin = prodOiMap.cbegin();
    while(begin!=prodOiMap.cend())
    {
        //LOG(INFO)<<fmt::format("{} has {} contracts",begin->first,begin->second.size());
        int sumOi = 0;
        auto iter = begin->second.cbegin();
        while(iter != begin->second.cend())
        {
            subscribePool.push_back(std::string((*iter).symbol));
            sumOi+= (*iter).openInterest;
            ++iter;
        }
        iter = begin->second.cbegin();
        while(iter != begin->second.cend())
        {
            double weight = (*iter).openInterest == 0 ? 0:(*iter).openInterest/(double)sumOi;
            prodWeight[std::string((*iter).symbol)]= weight;
            instrumentWeight[std::string((*iter).symbol)] =weight;
            std::cout<<(*iter).symbol<<" weight="<<prodWeight[std::string((*iter).symbol)]<<std::endl;
            ++iter;
        }
        ++begin;
    }

    td_stop();
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
