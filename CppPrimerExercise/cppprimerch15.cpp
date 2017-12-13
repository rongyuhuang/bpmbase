#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<functional>
#include<memory>

#include"quote.h"

void ch15_12()
{
    Quote q1;
    BulkQuote bq1;
    Quote* pq1 = &q1;
    pq1 = &bq1;
    Quote& rq1 = bq1;

    Quote q2(std::string("125-1-23-9"),25);
    std::cout<<q2.net_price(4)<<std::endl;
    print_total(std::cout,q2,4);
    BulkQuote bq2(std::string("125-1-23-9"),25,10,0.8);
    std::cout<<bq2.net_price(15)<<std::endl;

    print_total(std::cout,bq2,15);
    /*1.每个类控制自己的成员初始化过程
     * 2.每个类负责定义各自的接口
     * 3.派生类的声明，不包含它的派生列表
     * 4.一个类想做基类，必须已经定义
     * 5.组织一个类派生，可以在类名后跟 final
     * 6.不存在从基类向派生类的隐式类型转换，在对象之间不存在类型转换
    */

}

void ch15_34()
{
    /*虚函数
     * 1.对虚函数的调用可能在运行时才被解析
     * 2.动态绑定只有当我们通过指针或引用调用虚函数才会发生
     * 3.如果虚函数使用默认实参，则基类和派生中定义的默认实参最好一致
     * 4.只有成员函数或者友元中的代码才需要懂副作用运算符
    */
}
void testCh15()
{
    std::cout<<"Start to test chapter 15"<<std::endl;
    ch15_12();
    ch15_34();
}
