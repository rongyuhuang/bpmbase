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


template <typename T>
int compare(T& v1,T& v2)
{
    if(v1<v2)
        return -1;
    if(v1>v2)
        return 1;
    return 0;
}
void ch16_1()
{
    int i1=34,i2=34;
    std::cout<<compare(i1,i2)<<std::endl;
    std::vector<int> vi1{1,2,3,4},vi2{23,34,1};
    std::cout<<compare(vi1,vi2)<<std::endl;
}
void testCh16()
{
    std::cout<<"Start test chapter 16"<<std::endl;
    ch16_1();
}
