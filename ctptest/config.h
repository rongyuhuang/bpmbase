#ifndef CONFIG_H
#define CONFIG_H

#include<string>
#include<vector>
#include<list>

#include"utils/ini.h"
#include"utils/strutil.h"

#include<fstream>
class Config{
public:
    std::string brokerID,mdFront,userID,password;
    std::vector<std::string> symbols;
    std::string brokerName;
public:
    Config()
    {
        brokerName="sim03";
        init();
    }
    Config(std::string brokername_):brokerName(brokername_)
    {
        init();
    }

    Config(const Config& cfg)
    {
        brokerName=cfg.brokerName;
        init();
    }

    Config& operator =(const Config& rhs)
    {
        brokerName =rhs.brokerName;
        init();
        return *this;
    }
private:
    void init()
    {
        auto cfgFile = StrUtil::printf("c:/temp/%s.ini",brokerName.c_str());
        typedef INI<> ini;
        ini cfg(cfgFile,true);
        cfg.select("MD");
        brokerID = cfg.get("BrokerID","9999");
        mdFront = cfg.get("MdFront","tcp://180.168.146.187:10031");
        userID = cfg.get("UserID","038262");
        password = cfg.get("Password","000000");
        std::string s = cfg.get("Symbols","i1805");
        symbols = StrUtil::split(s,";");
    }
};
#endif // CONFIG_H
