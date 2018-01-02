#include <iostream>
#include<thread>
#include<chrono>
#include"easyctp/easyctp.h"

#include"utils/logging.h"
#include"utils/fileutil.h"
#include"utils/crashdump.h"
#include<fmt/format.h>
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

#include"datareceiver.h"

int main(int argc, char *argv[])
{
    bpm_CrashDumpInitialize();
    FLAGS_log_dir="D:/lovelyGoogle"; //指定程序日志目录
    bpm_createDir(FLAGS_log_dir.c_str());
    FLAGS_alsologtostderr = true; //设置日志消息除了日志文件是否输出到标准输出
    google::SetStderrLogging(google::GLOG_INFO); //设置日志输出最低级别
    google::InitGoogleLogging(argv[0]);

    LOG(INFO)<< "Start to test EASYCTP!";
    //std::cout<<fmt::format("{}@{}","easyctp","2018-01-01")<<std::endl;
    bool result =false;
    std::cout<<"result="<<result<<",!result="<<!result<<std::endl;
//    prepare_md();
//    test_md();

    Config cfg("sim03");
    DataReceiver dr(cfg);
//    std::thread collectThread([&dr]{dr.tickCollect();});
//    collectThread.detach();
    dr.start();

    getchar();

    google::ShutdownGoogleLogging();
    return 0;
}
