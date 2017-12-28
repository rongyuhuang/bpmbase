#ifndef RWHANDLER_H
#define RWHANDLER_H

#include<array>
#include<functional>
#include<iostream>

#include<boost/asio.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost;

#include"message.h"

const int MAX_IP_PACK_SIZE=65536;
const int HEAD_LEN=4;

class RWHandler:public std::enable_shared_from_this<RWHandler>{
public:
    RWHandler(io_service& ios):m_sock(ios){}
    ~RWHandler()
    {}

    void HandleRead()
    {
//        async_read(m_sock,buff(m_buff),transfer_at_least(HEAD_LEN),
//                   [this](const boost::system::error_code& ec,std::size_t size)
        //tcp粘包问题解决
        auto self = shared_from_this();
        //先收包头

        async_read(m_sock,buffer(m_msg.Data(),HEAD_LEN),
                   [this](const boost::system::error_code& ec,std::size_t size)
        {
            //解析包体长度
            if(ec !=nullptr || !m_msg.DecodeHeader())
            {
               HandleError(ec);
               return;
            }
            ReadBody();
        });
    }

    //解析包体
    void ReadBody()
    {
        auto self = shared_from_this();
        async_read(m_sock,buffer(m_msg.Body(),m_msg.BodyLength()),
                                [this,self](const boost::system::error_code& ec,std::size_t size)
        {
          if(ec !=nullptr)
          {
              HandleError(ec);
              return;
          }
          //收到完整的数据，回调到应用层
          std::cout<<m_buff.data()+HEAD_LEN<<std::endl;
          HandleRead(); //发起下一次异步毒，继续接收数据
        });
    }
    void HandleWrite(char* data,int len)
    {
        boost::system::error_code ec;
        write(m_sock,buffer(data,len),ec);
        if(ec != nullptr)
        {
            HandleError(ec);
        }
    }

    tcp::socket& GetSocket()
    {
        return m_sock;
    }

    void CloseSocket()
    {
        boost::system::error_code ec;
        m_sock.shutdown(tcp::socket::shutdown_send,ec);
        m_sock.close(ec);
    }

    void SetConnID(int connID)
    {
        m_connID = connID;
    }

    int GetConnID()const
    {
        return m_connID;
    }

    template<typename T>
    void SetCallbackError(T f)
    {
        m_callbackError =f;
    }
private:

    void HandleError(const boost::system::error_code& ec)
    {
         CloseSocket();
         std::cout<<ec.message()<<std::endl;
         if(m_callbackError)
         {
             m_callbackError(m_connID);
         }
    }
private:
    tcp::socket m_sock;
    std::array<char,MAX_IP_PACK_SIZE> m_buff;
    int m_connID;
    std::function<void(int)> m_callbackError;
    Message m_msg;
};
#endif // RWHANDLER_H
