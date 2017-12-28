#ifndef MESSAGE_H
#define MESSAGE_H

#include<iostream>
#include<string>
#include<stdlib.h>

class Message{
public:
    enum{header_length =4};
    enum{body_max_length=512};
    Message():body_length(0){}

    const char* Data()const
    {
        return data;
    }

    char* Data()
    {
        return data;
    }
    std::size_t Length()const
    {
        return header_length+body_length;
    }

    const char* Body()const
    {
        return data+header_length;
    }
    char* Body()
    {
        return data+header_length;
    }
    std::size_t BodyLength()const
    {
        return body_length;
    }

    void BodyLength(std::size_t newLen)
    {
        body_length = newLen;
        if(body_length> body_max_length)
        {
            body_length = body_max_length;
        }
    }

    bool DecodeHeader()
    {
        char header[header_length+1]="";
        std::strncat(header,data,header_length);
        body_length = std::atoi(header) - header_length;
        if(body_length > body_max_length)
        {
            body_length=0;
            return false;
        }
        return true;
    }

    bool EncodeHeader()
    {
        char header[header_length+1]="";
        std::sprintf(header,"%4d",body_length);
        std::memcpy(data,header,header_length);
    }
private:
    std::size_t body_length;
    char data[header_length+body_max_length];
};
#endif // MESSAGE_H
