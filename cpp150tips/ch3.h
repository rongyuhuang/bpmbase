/*
 * 内存管理相关事宜
*/
#ifndef CH3_H
#define CH3_H

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
const int COUNT=10; //常量存储区
void tip27()
{
    //区分内存分配的方式
    std::string * pStr = new std::string[COUNT];
    //pStr 局部变量，位于栈内； new std::string[COUNT] ,动态分配的堆空间
    delete[] pStr;
}


class Test
{
public:
    Test(){std::cout<<"ctor"<<std::endl;}
    ~Test(){std::cout<<"dctor"<<std::endl;}
    void Hello(){std::cout<<"Hi,C++ 150 tips"<<std::endl;}
};
//new/delete new[]/delete[] 必须配对使用
void tip28()
{
    std::cout<<"Test 1"<<std::endl;
    Test* t1 = new Test[3];
    delete [] t1;

    Test* t2 = new Test;
    delete t2;
}
void testCh3()
{
    tip27();
    tip28();
}
#endif // CH3_H
