#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <myboost/boost/asio.hpp>
#include<thread>
#include"message.h"
#include"connector.h"
#include"rwhandler.h"
#include"server.h"

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost;

void testServer()
{
    io_service ios;
    Server server(ios,9900);
    server.Accept();
    ios.run();
}

int  testClient()
{
    io_service ios;
    boost::asio::io_service::work work(ios);

    //boost::asio::detail::thread thd([&ios]{ios.run();});
    //std::thread thd([&ios]{ios.run();});

    Connector conn(ios,"127.0.0.1",9900);
    conn.Start();

    std::string str;
    if(!conn.Connected())
    {
        std::cin>>str;
        return -1;
    }

    const int len=512;
    char line[len]="";
    while(std::cin>>str)
    {
        char header[HEAD_LEN]={};
        int totalLen = str.length()+1+HEAD_LEN;
        std::sprintf(header,"%4d",totalLen);
        memcpy(line,header,HEAD_LEN);
        memcpy(line+HEAD_LEN,str.c_str(),str.length()+1);
        conn.Send(line,totalLen);
    }
    return 0;
}
void testCh16()
{
    std::cout<<"Start to test chapter 16"<<std::endl;

    int port =10000;
    try
    {
        //testServer();
        testClient();
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }

}
