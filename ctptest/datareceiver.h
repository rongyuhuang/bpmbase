#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include<string>
#include<map>
#include<vector>
#include<atomic>
#include<thread>
#include<memory>
#include<easyctp/easyctp.h>

#include"utils/logging.h"
#include"utils/timeutil.h"
#include"kbarutils/tickstorage.h"
#include"config.h"
#include"simplesyncqueue.h"
class DataReceiver{
public:
    DataReceiver();
    DataReceiver(const Config& cfg);
    ~DataReceiver();
    void start();

    void stop();

    void onTick(TickData* tick);

    void tickCollect();

    bool getLastestTick(const char* symbol,TickData& result);

public:
    std::map<std::string,SimpleSyncQueue<TickData>> tickMap;
    std::map<std::string,std::vector<TickData*>*> tickCache;
    std::map<std::string,TickStorage*> tickStorage;

private:
    Config config;
    std::string tradingDay;
    std::map<std::string,std::vector<std::string>> prodInstMap; //品种-所有对应合约
    std::map<std::string,std::string> mainContractMap;//品种-主力合约(持仓量最大)
    std::vector<std::string> subscribePool;
    std::mutex tick_mutex;
    std::atomic_bool quit;
    std::unique_ptr<std::thread> qryTickThread;
private:
    void prepareMd();
    void prepareTd();
    void initSubscribePool();
    void saveTicks();
    void saveTickToFile(const char* symbol,std::vector<TickData*>& ticks);
    void loadTicks();
    void loadTickFromFile(const std::string fname,TickStorage* result);
    void calcIndex(const std::string& productID,const TickData* mainTick);
};
#endif // DATARECEIVER_H
