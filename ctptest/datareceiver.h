#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include<string>
#include<map>
#include<atomic>
#include<easyctp/easyctp.h>

#include"utils/logging.h"
#include"utils/timeutil.h"

#include"config.h"
#include"simplesyncqueue.h"
class DataReceiver{
public:
    DataReceiver();
    DataReceiver(const Config& cfg):
        quit(false),config(cfg)
    {
        initMd();
    }
    ~DataReceiver()
    {
        quit=true;
        stop();
    }
    void start()
    {
        md_start();
        for(auto x:config.symbols)
        {
            md_subscribe(x.c_str());
        }
    }
    void stop()
    {
        for(auto x:config.symbols)
        {
            md_unsubscribe(x.c_str());
        }
        md_stop();
        //        for(auto x:tickMap)
        //        {
        //            delete x.second;
        //        }
        tickMap.clear();
    }

    void onTick(TickData* tick)
    {
        std::lock_guard<std::mutex> lock(tick_mutex);
        auto iter = tickMap.find(std::string(tick->symbol));
        if(iter !=tickMap.end())
        {
            LOG(INFO)<<__FUNCTION__<<iter->first;
            //(*iter)[std::string(tick->symbol)].Put(*tick);
        }
    }

    void tickCollect()
    {
        LOG(INFO)<<__FUNCTION__<<" begin...";

        int ret = STATUS_OK;
        RtnData data;
        while(!quit)
        {
            ret = md_queryRtnData(&data);
            if(ret== STATUS_OK)
            {
                CHECK(data.rtnDataType==RTNDATA_TICK);
                TickData* tick = (TickData*)data.rtnDataPtr;
                onTick(tick);
                ret = md_freeMemory(tick);
                CHECK(ret==STATUS_OK);
                //handle tick
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
            {break;
            }
        }


        LOG(INFO)<<__FUNCTION__<<" end.";
    }
public:
    std::map<std::string,SimpleSyncQueue<TickData>> tickMap;

private:
    Config config;
    std::mutex tick_mutex;
    std::atomic_bool quit;
private:
    void initMd()
    {
        md_setBrokerInfo(config.brokerID.c_str(),config.mdFront.c_str());
        md_setUserInfo(config.userID.c_str(),config.password.c_str());
        md_setConfig("mdFlow",2000);

    }
};
#endif // DATARECEIVER_H
