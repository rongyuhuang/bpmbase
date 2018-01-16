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
#include"datareceiver.h"

#include"utils/cppjson.h"

void testCppJson()
{
    CppJson json;
    json.StartArray();
    for(int i=0;i<3;++i)
    {
        json.StartObject();
        json.WriteJson("ID",i);
        json.WriteJson("Name","Pter");
        json.EndObject();
    }
    json.EndArray();
    std::cout<<json.GetString()<<std::endl;
}
void test_md()
{
  LOG(INFO) << __FUNCTION__;
  md_start();

  md_subscribe("rb1805");
  md_subscribe("ZC805");
  {
      RtnData rtndata;
      int tryTimes=50;
      do{
          --tryTimes;
          auto ret = md_queryRtnData(&rtndata);
          if(ret ==STATUS_OK)
          {
              CHECK(rtndata.rtnDataType == RTNDATA_TICK);
              TickData* tick = (TickData*)rtndata.rtnDataPtr;
              std::cout<<tick->symbol<<","<<tick->lastPrice<<std::endl;
              return ;
          }
          else
          {
              if(ret ==STATUS_EMPTY)
              {
                  std::cout<<"tick queue is empty,sleep 100ms"<<std::endl;
                  std::this_thread::sleep_for(std::chrono::milliseconds(100));
              }
          }
      }while(tryTimes>0);
  }
}

void prepare_md() {
  LOG(INFO) << __FUNCTION__;

  md_setBrokerInfo("9999","tcp://180.168.146.187:10031");
  md_setUserInfo("038262","qaz75369");
  md_setConfig("mdflow",100);
}

void test_td()
{
    td_start();
    TickData* mdSnap=nullptr;
    int count=0;
    int ret =td_queryMarketData(&mdSnap,&count);
    LOG(INFO)<<fmt::format("td_queryMarketData()={},count={}",ret,count);
    std::map<std::string,std::vector<TickData>> prodOiMap;

    for(int i = 0;i<count;++i)
    {
        char prod[3];
        LOG(INFO)<<fmt::format("{} Volume={} , OI={}", mdSnap[i].symbol, mdSnap[i].totalVolume, mdSnap[i].openInterest);
        CtpUtils::instrument2product(mdSnap[i].symbol,prod);
        std::string productID = std::string(prod);
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
        LOG(INFO)<<fmt::format("{} has {} contracts",begin->first,begin->second.size());
        int sumOi = 0;
        auto iter = begin->second.cbegin();
        while(iter != begin->second.cend())
        {
            sumOi+= (*iter).openInterest;
            ++iter;
        }
        iter = begin->second.cbegin();
        while(iter != begin->second.cend())
        {
            double weight = (*iter).openInterest == 0 ? 0:(*iter).openInterest/(double)sumOi;
            prodWeight[std::string((*iter).symbol)]= weight;
            std::cout<<(*iter).symbol<<" weight="<<prodWeight[std::string((*iter).symbol)]<<std::endl;
            ++iter;
        }
        ++begin;
    }

    td_stop();
}
void prepare_td()
{
    td_setBrokerInfo("9999","tcp://180.168.146.187:10030");
    td_setUserInfo("038262","qaz75369");
    td_setConfig("tdFlow",200000,1,10);
}
#include"datareceiver.h"

int main(int argc, char *argv[])
{
//    auto tpNow = std::chrono::system_clock::now();
//    auto ctNow = std::chrono::system_clock::to_time_t(tpNow);
//    auto strNow = std::put_time(std::localtime(&ctNow),"%Y%m%d %X");
//    std::cout<<strNow<<std::endl;

    bpm_CrashDumpInitialize();
    FLAGS_log_dir="D:/lovelyGoogle"; //指定程序日志目录
    bpm_createDir(FLAGS_log_dir.c_str());
    FLAGS_alsologtostderr = true; //设置日志消息除了日志文件是否输出到标准输出
    google::SetStderrLogging(google::GLOG_INFO); //设置日志输出最低级别
    google::InitGoogleLogging(argv[0]);
    testCppJson();
    char cNow[20];
    bpm_getLocalTime(cNow);
    LOG(INFO)<<fmt::format( "Start to test {} @ {}!","EASYCTP",cNow);

//    prepare_md();
//    test_md();

//    prepare_td();
//    test_td();
    Config cfg("sim03");
    DataReceiver dr(cfg);
    dr.start();
    getchar();
    dr.stop();
    google::ShutdownGoogleLogging();
    getchar();
    return 0;
}
