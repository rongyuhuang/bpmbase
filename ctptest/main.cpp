#include <iostream>
#include"easyctp/easyctp.h"

#include"utils/logging.h"
void test_md()
{
  LOG(INFO) << __FUNCTION__;
  md_start();

  md_subscribe("rb1805");
}

void prepare_md() {
  LOG(INFO) << __FUNCTION__;

  md_setBrokerInfo("9999","tcp://180.168.146.187:10031");
  md_setUserInfo("038262","qaz75369");
  md_setConfig("c:/temp/mdapi/",100);
}

int main(int argc, char *argv[])
{
    std::cout << "Start to test EASYCTP!" << std::endl;
    prepare_md();
    test_md();
    getchar();
    return 0;
}
