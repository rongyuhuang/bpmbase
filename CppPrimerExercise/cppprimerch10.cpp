#include<iostream>
#include<vector>
#include<stack>
#include<list>

#include<iterator> //back_inserter
#include<algorithm>
#include<numeric>


void ch10_1()
{
    std::vector<int> ivec={12,12,11,23,43,42,67};
    int val=42;
    auto result= find(ivec.cbegin(),ivec.cend(),val) ;
    std::cout<<"The value "<<val
            <<(result==ivec.end()? " is not present":" is present.")<<std::endl;

    int ia[]={23,12,210,23,109,80,32,43};
    val=109;
    int* iaResult = std::find(std::begin(ia),std::end(ia),val);
    std::cout<<"The value "<<val
            <<(iaResult==std::end(ia)? " is not present":" is present.")<<std::endl;

    //迭代器令算法不依赖于容器，但算法依赖于元素类型的操作

}

void ch10_2()
{
    //只读算法
    std::vector<int> ivec={12,23,12,43,546,64,342};
    int isum=std::accumulate(ivec.cbegin(),ivec.cend(),0);
    std::cout<<isum<<std::endl;

    std::vector<std::string> svec={"Hi,","C++ Primer ","5th Ed."};
    auto sSum = std::accumulate(svec.cbegin(),svec.cend(),std::string(""));
    std::cout<<sSum<<std::endl;

    std::vector<std::string> svec2={"Hi,","C++ Primer"};
    //auto eqResult = equal(svec.cbegin(),svec.cend(),svec2.cbegin());
    //std::cout<<eqResult<<std::endl;
    fill(ivec.begin(),ivec.end(),0);
    std::cout<<"fill 0 to ivec"<<std::endl;
    for(const auto x:ivec)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    fill_n(ivec.begin(),ivec.size()/2,20);
    std::cout<<"fill 20 to half of ivec"<<std::endl;
    for(const auto x:ivec)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    //fill_n 注意不越界

    ivec.clear();
    auto it = std::back_inserter(ivec);
    *it=45;
    fill_n(std::back_inserter(ivec),10,0);
    std::cout<<"clear ivec,then using back_inserter fill 45,(10,0) to ivec"<<std::endl;
    for(const auto x:ivec)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    int a1[]={0,1,2,3,4,5,6,7,8,9};
    int a2[sizeof(a1)/sizeof(*a1)];
    //目的序列至少要包含与输入序列一样多的元素
    auto ret = copy(std::begin(a1),std::end(a1),a2);
    //ret指向a2尾部

    std::replace(ivec.begin(),ivec.end(),45,20);


}
void testCh10()
{
    std::cout<<"Start test chapter 10"<<std::endl;
    ch10_1();
    ch10_2();
}
