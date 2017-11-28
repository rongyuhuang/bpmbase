#include<iostream>
#include<vector>
#include<string>
#include<exception>
#include<stdexcept>
#include"Sales_item.h"

void ch5_123()
{
    ;
    //多余的空语句并不是无害
//    std::vector<std::string> svec{"Chapter","5","is statement"};
//    auto iter= svec.begin();
//    while(iter !=svec.end());
//    ++iter;
    int val=0,sum=0;
    while(val<=10)
    {
        sum+=val;
        ++val;
    }
    std::cout<<sum<<std::endl;
    //块不已分号作为结束

    //语句作用域

    // if else
    int grade;
    std::string result;
    const std::vector<std::string> scores ={"F","E","D","C","B","A","A++"};
    std::cout<<"Input grade"<<std::endl;
    std::cin>>grade;
    if(grade<60)
    {
        result = scores[0];
    }
    else
    {
        result = scores[(grade-50)/10];
    }
    std::cout<<grade<<" is "<<result<<std::endl;

    //switch case
    std::string s{"qweertyfgddsaghjgfcfxzsasds"};
    int vowelCnt=0,otherCnt=0;
    for(auto x:s)
    {
        switch (x) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            ++vowelCnt;
            break;
        default:
            ++otherCnt;
            break;
        }
    }
    std::cout<<s<<" has "<<vowelCnt<<" vowel,"<<otherCnt<<" other"<<std::endl;
}

void ch5_45()
{
    //迭代语句
    std::vector<int> ivec ;
    int i;
    std::cout<<"input number"<<std::endl;
    while(std::cin>>i)
    {
        ivec.push_back(i);
    }

    auto beg = ivec.begin();
    while(beg !=ivec.end() &&*beg>=0)
    {
        ++beg;
    }
    if(beg==ivec.end())
    {
        std::cout<<"All element of ivec >=0"<<std::endl;
    }

    for(beg=ivec.begin();beg!=ivec.end();++beg)
    {
        std::cout<<*beg<<"\t";
    }
    std::cout<<std::endl;

    for(auto &x:ivec)
    {
        x*=2;
    }
    beg = ivec.begin();
    auto end=ivec.end();
    for(;beg!=end;++beg)
    {
        std::cout<<*beg<<"\t";
    }
    std::cout<<std::endl;

    std::string rsp;
    do{
        std::cout<<"input two numbers:"<<std::endl;
        int v1,v2;
        std::cin>>v1>>v2;
        std::cout<<v1<<"+"<<v2<<"="<<v1+v2<<std::endl;
        std::cout<<"Continue input y,else input n";
        std::cin>>rsp;
    }while(!rsp.empty() && rsp[0] !='n');
}

void ch5_6()
{
    Sales_item item1,item2;
    std::cout<<"input two book info"<<std::endl;
    while(std::cin>>item1>>item2)
    {
        try
        {
            if(item1.isbn()==item2.isbn())
            {
                std::cout<<item1+item2<<std::endl;
            }
            else
            {
                throw ;//runtime_error()
            }
        }
        catch(int i)
        {
            std::cout<<"two book have different isbn"<<std::endl;

            std::cout<<"Try again input \'y\', else \'n\'"<<std::endl;
            char c;
            std::cin>>c;
            if(!std::cin || c=='n')
            {
                break;
            }
        }
    }
}
void testCh5()
{
    std::cout<<"Start test chapter 5"<<std::endl;
    ch5_123();
    ch5_45();
    ch5_6();
}
