#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include"sales_data.h"

void ch8_1()
{
    //IO对象无拷贝或赋值

    int ival;
    std::cin>>ival;
    auto state = std::cin.rdstate();
    std::cout<<state<<std::endl;

    //管理条件状态
//    std::cin.setstate(3);
//    std::cin.clear();

    std::cout<<"hi"<<std::endl;
    std::cout<<"Hi"<<std::flush;
    std::cout<<"HI"<<std::ends;

    std::cout<<std::unitbuf;
    std::cout<<std::nounitbuf;

    //关联输入和输出流
    std::cin.tie(&std::cout);
    std::ostream* oldTie = std::cin.tie(nullptr);

    std::cin.tie(&std::cerr);
    std::cin.tie(oldTie);

}


void ch8_2()
{
    //文件输入输出

    std::ifstream in;//("in.txt",std::ios::in);
    in.open("in.txt",std::ios::in);
    if(in.is_open()==false)
    {
        std::cout<<"can\'t open file."<<std::endl;
    }
    std::ofstream out("out.txt",std::ios::out);

    Sales_data total;
    if(read(in,total))
    {
        Sales_data tran;
        while(read(in,tran))
        {
            if(total.isbn() == tran.isbn())
            {
                total.combine(tran);
            }
            else
            {
                print(out,total)<<std::endl;
                total = tran;
            }
        }
        print(out,total)<<std::endl;
    }
    else
    {
        std::cerr<<"No data!"<<std::endl;
    }

}
struct PersonInfo
{
    std::string name;
    std::vector<std::string> phones;
};

bool valid(const std::string& phone)
{
    if(phone.size()==11 && phone[0]=='1')
    {
        return true;
    }
    return false;

}

std::string format(const std::string& phone)
{
    std::string fmt;
    fmt = phone.substr(0,3)+"-"+phone.substr(3,4)+"-"+phone.substr(6,4);
    return fmt;
}
void ch8_3()
{
   std::string line,word;
   std::vector<PersonInfo> peoples;
   while(std::getline(std::cin,line))
   {
       PersonInfo info;
       std::istringstream record(line);
       record>>info.name;
       while(record>>word)
       {
           info.phones.push_back(word);
       }
       peoples.push_back(info);
   }

  for(const PersonInfo& info:peoples)
  {
      std::ostringstream formatted,badNums;
      for(const std::string &phone:info.phones)
      {
          if(!valid(phone))
          {
              badNums<<" "<<phone;
          }
          else
          {
              formatted<<" "<<format(phone);
          }
      }
      if(badNums.str().empty())
      {
          std::cout<<info.name<<" "<<formatted.str()<<std::endl;
      }
      else
      {
          std::cerr<<" input error:"<<info.name<<"invalid phones:"<<badNums.str()<<std::endl;
      }
  }
}
void testCh8()
{
    std::cout<<"Start test chapter 8"<<std::endl;
    //ch8_1();
    ch8_2();
    ch8_3();
}
