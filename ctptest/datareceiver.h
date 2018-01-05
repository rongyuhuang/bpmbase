#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include<string>
#include<map>
#include<atomic>
#include<thread>
#include<memory>
#include<easyctp/easyctp.h>

#include"utils/logging.h"
#include"utils/timeutil.h"

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

public:
    std::map<std::string,SimpleSyncQueue<TickData>> tickMap;
    std::map<std::string,std::vector<TickData>> tickCache;

private:
    Config config;
    std::mutex tick_mutex;
    std::atomic_bool quit;
    std::unique_ptr<std::thread> qryTickThread;
private:
    void initMd();
    void saveTicks();
    void loadTicks();
};
#endif // DATARECEIVER_H
