#include"datareceiver.h"
#include<fstream>
#include<list>
#include <iostream>
#include<cmath>

#include"easyctp/ctputils.h"
#include"utils/alias.h"
#include"kbarutils/kbartable.h"
DataReceiver::DataReceiver(const Config& cfg):
   config(cfg)
{
    prepareMd();
    prepareTd();
}
DataReceiver::~DataReceiver()
{
    LOG(INFO)<<__FUNCTION__;
    for(auto& x:tickStorage)
    {
        delete x.second;
    }
    tickStorage.clear();

    for(auto& x:tickCache)
    {
        delete x.second;
    }
    tickCache.clear();
    tickMap.clear();
}

void DataReceiver::start()
{
    std::lock_guard<std::mutex> lock(tick_mutex);
    initSubscribePool();
    loadTicks();
    quit=false;
    md_start();
    qryTickThread.reset(new std::thread(std::bind(&DataReceiver::tickCollect,this)));


    for(auto& x:subscribePool)
    {
        tickCache[x] = new std::vector<TickData*>;
        int ret = md_subscribe(x.c_str());
        CHECK(ret ==STATUS_OK);
        //LOG(INFO)<<__FUNCTION__<<","<<x;
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
    auto it = tickStorage.find(tick->symbol);
    if(it != tickStorage.end())
    {
        TickStorage* storage = it->second;
        //1.有效性检查
        TickData* lastestTick = storage->getLatestTick();
        utils::Alias(&lastestTick);
        if(!CtpUtils::isValidTickData(tick,true))
        {
            if(lastestTick !=nullptr)
            {
                //CHECK(0);
            }
            //return;
        }
        //2.计算当前tick的volume
        if(lastestTick == nullptr)
        {
            tick->volume=tick->totalVolume; //如果是开盘第一个tick，volume=totalvolume
        }
        else
        {
            tick->volume = tick->totalVolume - lastestTick->totalVolume;
            if(tick->volume<0)
            {
                if(true)
                {
                    //实盘
                    TickData uglyTick;
                    memset(&uglyTick,0,sizeof(TickData));
                    storage->getLatestTick(uglyTick);
                    utils::Alias(&uglyTick);
                    CHECK(tick->volume>=0);
                }
                else
                {
                    //模拟盘
                    tick->volume=0;
                }
            }
        }
        //3.根据交易所信息，修正 turnover和averageprice， ZCE

        //4.缓存
        TickData* newTick = storage->appendTick(tick);
        auto& x= tickCache[newTick->symbol];
        if(x->size()==120) //大约1min保存一次
        {
            saveTickToFile(tick->symbol,*x);
        }
        x->push_back(tick);

        //5.当为主力合约时，更新指数
        char prod[3];
        memset(prod,0,sizeof(prod));
        CtpUtils::instrument2product(newTick->symbol,prod);
        auto productID = std::string(prod);
        auto inst = mainContractMap[productID];
        if(inst==std::string(newTick->symbol))
        {
            LOG(INFO)<<"receive "<<productID<<"\'s main contract:"<<inst<<",update its index price ";
            calcIndex(productID,newTick);
        }
    }
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

bool DataReceiver:: getLastestTick(const char* symbol,TickData& result)
{
    std::lock_guard<std::mutex> lock(tick_mutex);
    if(quit)
    {
        return false;
    }
    auto it = tickStorage.find(symbol);
    if(it != tickStorage.end())
    {
        TickStorage* ticks = it->second;
        return ticks->getLatestTick(result);
    }
    return false;
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
    tradingDay =std::string(td_getTradingDay());

    TickData* mdSnap=nullptr;
    int count=0;
    int ret =td_queryMarketData(&mdSnap,&count);
    LOG(INFO)<<"td_queryMarketData()="<<ret<<",count="<<count;
    std::map<std::string,std::vector<TickData>> prodOiMap;

    for(int i = 0;i<count;++i)
    {
        char prod[3];
        memset(prod,0,sizeof(prod));
        //LOG(INFO)<<fmt::format("{} Volume={} , OI={}", mdSnap[i].symbol, mdSnap[i].totalVolume, mdSnap[i].openInterest);
        CtpUtils::instrument2product(mdSnap[i].symbol,prod);
        std::string productID = std::string(prod);
        if(std::find(config.symbols.cbegin(),config.symbols.cend(),productID)
                ==config.symbols.cend())
        {
            //LOG(INFO)<<mdSnap[i].symbol<<" does not in subscribe products";
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
    }
    auto begin = prodOiMap.cbegin();
    while(begin!=prodOiMap.cend())
    {
        std::string indexSym =begin->first+"0000";
        tickCache[indexSym] = new std::vector<TickData*>;

        prodInstMap[begin->first] = std::vector<std::string>();
        //LOG(INFO)<<fmt::format("{} has {} contracts",begin->first,begin->second.size());
        int sumOi = 0;
        int maxOi=0;
        auto iter = begin->second.cbegin();
        while(iter != begin->second.cend())
        {
            if(maxOi< (*iter).openInterest)
            {
                mainContractMap[begin->first]=(*iter).symbol; //取品种对应的主力合约
            }
            subscribePool.push_back(std::string((*iter).symbol));
            prodInstMap[begin->first].push_back(std::string((*iter).symbol));
            sumOi+= (*iter).openInterest;
            ++iter;
        }
        ++begin;
    }
//    std::cout<<"output product->instrument map"<<std::endl;
//    auto prodInstIter = prodInstMap.cbegin();
//    while(prodInstIter != prodInstMap.cend())
//    {
//        std::cout<<prodInstIter->first<<":\t";
//        auto instIter = (prodInstIter->second).cbegin();
//        while(instIter != (prodInstIter->second).cend())
//        {
//            std::cout<<*instIter<<",";
//            instIter++;
//        }
//        std::cout<<std::endl;
//        prodInstIter++;
//    }
    td_stop();
}
void DataReceiver::saveTicks()
{
    LOG(INFO)<<__FUNCTION__<<"...";
    auto begin = tickCache.cbegin();
    while(begin !=tickCache.cend())
    {
        auto sym = begin->first.c_str();
        auto vec = *begin->second;
        saveTickToFile(sym,vec);
        ++begin;
    }
}

void DataReceiver::saveTickToFile(const char* symbol,std::vector<TickData*>& ticks)
{
    std::string path= StrUtil::printf("c:/temp/datafeed/tick/%s_%s.txt", symbol,tradingDay.c_str());
    std::ofstream file;
    file.open(path,std::ios::binary|std::ios::app);
    if(file.is_open())
    {
        for(auto & v:ticks)
        {
            file.write((char*)&v,sizeof(TickData));
        }
    }
    ticks.clear();
    file.close();
}

void DataReceiver::loadTicks()
{
    auto iter = prodInstMap.begin();
    while(iter != prodInstMap.end())
    {
        auto productID = iter->first;
        auto symbols = iter->second;
        for(auto & sym:symbols)
        {
            TickStorage* storage= new TickStorage(86400);
            std::string fname = StrUtil::printf("c:/temp/datafeed/tick/%s_%s.txt",
                                               sym.c_str(),tradingDay.c_str());
            loadTickFromFile(fname,storage);
            tickStorage[sym] = storage;
        }
        //指数的
        std::string indexSym = StrUtil::printf("%s0000",productID.c_str());
        TickStorage* storage= new TickStorage(86400);
        std::string fname = StrUtil::printf("c:/temp/datafeed/tick/%s_%s.txt",
                                           indexSym.c_str(),tradingDay.c_str());
        loadTickFromFile(fname,storage);
        tickStorage[indexSym] = storage;

        iter++;
    }
}
void DataReceiver::loadTickFromFile(const std::string fname,TickStorage* result)
{
    std::ifstream file;
    file.open(fname,std::ios::binary);
    TickData tick;
    int ncount=0;
    while(file.is_open()&& !file.eof()&& file.read((char*)&tick,sizeof(TickData)))
    {
        result->appendTick(&tick);
        ncount++;
    }
    file.close();
    LOG(INFO)<<__FUNCTION__<<",file="<<fname<<",count="<<ncount;
}

void DataReceiver::calcIndex(const std::string &productID, const TickData *mainTick)
{
    auto symbols = prodInstMap[productID];
    auto indexSym = productID+"0000";// StrUtil::printf("%s0000",productID);

    //计算指数价格
    double totalOI=0,totalOI_PX=0;
    for(auto& sym :symbols)
    {
        auto iter = tickStorage.find(sym);
        CHECK(iter !=tickStorage.end());
        TickStorage* storage =iter->second;
        TickData* lastestTick = storage->getLatestTick();
        if(lastestTick !=0 && CtpUtils::isValidTickData(lastestTick,true))
        {
            LOG(INFO)<<__FUNCTION__<<",symbol="<<sym<<",oi="<<lastestTick->openInterest<<",lastPrice="<<lastestTick->lastPrice;
            totalOI +=lastestTick->openInterest;
            totalOI_PX += lastestTick->openInterest * lastestTick->lastPrice;
        }
    }
    auto indexPx = totalOI_PX==0 ? 0: totalOI_PX/totalOI;
    //indexPx = std::round(indexPx);
    LOG(INFO)<<__FUNCTION__<<",current index:"<<indexSym<<",oi="<<totalOI<<",price="<<indexPx<<",std::round(indexPrice)="<<std::round(indexPx);
    //以主力合约为基础，更新指数tick
    TickData indexTick=*mainTick;

//    memset(indexTick,0,sizeof(TickData));
//    memcpy(indexTick,mainTick,sizeof(indexTick));
    indexTick.lastPrice = indexPx;
    indexTick.openInterest =totalOI;
    auto& x = tickCache[indexSym];
     x->push_back(&indexTick);
    auto indexIter = tickStorage.find(indexSym);
    if(indexIter !=tickStorage.end())
    {
        TickStorage* indexStorage = indexIter->second;
        indexStorage->appendTick(&indexTick);
    }
    else
    {
        TickStorage* storage = new TickStorage(86400);
        storage->appendTick(&indexTick);
        tickStorage[indexSym] = storage;
    }
}
