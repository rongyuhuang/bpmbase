#ifndef SERVER_H
#define SERVER_H
#include<boost/asio/buffer.hpp>
#include<unordered_map>
#include<numeric>

#include"message.h"
#include"rwhandler.h"

const int MAX_CONN_NUM=65536;
const int MAXRCV_SIZE=65536;

class Server{
public:
    Server(io_service& ios,short port):
        m_ios(ios),
        m_acceptor(ios,tcp::endpoint(tcp::v4(),port)),
        m_connIDs(MAX_CONN_NUM)
    {
        m_connIDs.resize(MAX_CONN_NUM);
        std::iota(m_connIDs.begin(),m_connIDs.end(),1);
    }

    ~Server()
    {}

    void Accept()
    {
        std::cout<<"Start to listening..."<<std::endl;

        std::shared_ptr<RWHandler> handler = createHandler();

        m_acceptor.async_accept(handler->GetSocket(),
                                [this,handler](const boost::system::error_code& ec)
        {
            if(ec)
            {
                std::cout<<"Accept Error:["<<ec.value()<<"->"<<ec.message()<<"]"<<std::endl;
                handleAcpError(handler,ec);
                return;
            }
            m_handlers.insert(std::make_pair(handler->GetConnID(),handler));
            std::cout<<"Current connector count:"<<m_handlers.size()<<std::endl;
            handler->HandleRead();
            Accept();
        });
    }
private:
    void handleAcpError(std::shared_ptr<RWHandler> eventHandler,const boost::system::error_code& ec)
    {
        std::cout<<"Error:reason["<<ec.value()<<"->"<<ec.message()<<"]"<<std::endl;
        eventHandler->CloseSocket();
        stopAccept();
    }

    void stopAccept()
    {
        boost::system::error_code ec;
        m_acceptor.cancel(ec);
        m_acceptor.close(ec);
        m_ios.stop();
    }

    std::shared_ptr<RWHandler> createHandler()
    {
        int connID = m_connIDs.front();
        m_connIDs.pop_front();
        std::shared_ptr<RWHandler> handler = std::make_shared<RWHandler>(m_ios);
        handler->SetConnID(connID);
        handler->SetCallbackError([this](int connID){recyclConnID(connID);});
        return handler;
    }

    void recyclConnID(int connID)
    {
        auto it =m_handlers.find(connID);
        if(it != m_handlers.end())
        {
            m_handlers.erase(it);
        }
        std::cout<<"Current connectors count:"<<m_handlers.size()<<std::endl;
        m_connIDs.push_back(connID);
    }
private:
    io_service& m_ios;
    tcp::acceptor m_acceptor;
    std::unordered_map<int,std::shared_ptr<RWHandler>> m_handlers;
    std::list<int> m_connIDs;
};
#endif // SERVER_H
