#ifndef CONNECTOR_H
#define CONNECTOR_H

#include<string>
#include<thread>
#include<chrono>

#include<boost/asio.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost;

#include"rwhandler.h"

class Connector{
public:
    Connector(io_service& ios,const std::string strIP,short port):
        m_ios(ios),m_sock(ios),
    m_serverAddr(tcp::endpoint(address::from_string(strIP),port)),
    m_connected(false),m_chkThread(nullptr)
    {
        createEventHandler(ios);
    }

    ~Connector()
    {}

    bool Start()
    {
        m_eventHandler->GetSocket().async_connect(m_serverAddr,[this](const boost::system::error_code& ec)
        {
            if(ec)
            {
                handleConnectError(ec);
                return;
            }
            std::cout<<"connect success"<<std::endl;
            m_connected =true;
            m_eventHandler->HandleRead(); //连接成功后发起一个异步读
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));
        return m_connected;
    }

    void Send(char* data,int len)
    {
        if(!m_connected)
        {
            return;
        }
        m_eventHandler->HandleWrite(data,len);
    }

    bool Connected()const
    {
        return m_connected;
    }
private:
    void createEventHandler(io_service& ios)
    {
        m_eventHandler = std::make_shared<RWHandler>(ios);
        m_eventHandler->SetCallbackError([this](int connID){std::cout<<connID<<std::endl;});
    }

    void checkConnect()
    {
        if(m_chkThread!=nullptr)
        {
            return;
        }
        m_chkThread = std::make_shared<std::thread>([this]{
            while(true)
            {
                if(m_connected)
                {
                    Start();
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
    }

    void handleConnectError(const boost::system::error_code& ec)
    {
        m_connected = false;
        std::cout<<ec.message()<<std::endl;
        m_eventHandler->CloseSocket();
        checkConnect();
    }

    void handleRWError(int connID)
    {
        m_connected = false;
        checkConnect();
    }
private:
    io_service& m_ios;
    tcp::socket m_sock;
    tcp::endpoint m_serverAddr;
    std::shared_ptr<RWHandler> m_eventHandler;
    bool m_connected;
    std::shared_ptr<std::thread> m_chkThread;
};
#endif // CONNECTOR_H
