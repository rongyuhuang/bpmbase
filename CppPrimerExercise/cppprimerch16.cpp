#include<iostream>
#include<string>
#include<list>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<memory>
#include<functional>
#include<sstream>
#include<fstream>

#include"blob.h"

template <typename T>
int compare(T& v1,T& v2)
{
    if(v1<v2)
        return -1;
    if(v1>v2)
        return 1;
    return 0;
}

//非类型模板参数：模板实参必须是常量表达式
//template <unsigned N,unsigned M>
//int compare(const char[&p1][N],const char[&p2][M])
//{
//    return strcmp(p1,p2);
//}


template <typename T> inline T min(const T& v1,const T& v2)
{
    return v1 > v2 ? v2:v1;
}
void ch16_1()
{
    int i1=34,i2=34;
    std::cout<<compare(i1,i2)<<std::endl;
    std::vector<int> vi1{1,2,3,4},vi2{23,34,1};
    std::cout<<compare(vi1,vi2)<<std::endl;
    /*
     * 1.非类型模板参数的模板实参必须是常量表达式
     * 2.模板函数的声明可以为 inline或constexpr,需放在模板参数列表之后
     * 3.模板程序应该尽量减少对实参类型的要求
     * 4.函数模板和类模板成员函数的定义通常凡在头文件中。
    */

    //Blob<int> ia;
    Blob<int> ia1={0,1,2,3,4,5,6,7,8,9};
    std::cout<<ia1.size()<<std::endl;
    std::cout<<ia1.back()<<std::endl;
    for(std::size_t i=0;i!=ia1.size();++i)
    {
        ia1[i] = i*i;
    }

}
void testCh16()
{
    std::cout<<"Start test chapter 16"<<std::endl;
    ch16_1();
}
