#include<iostream>
#include<string>
#include<vector>
#include<fmt/format.h>

int fact(int val)
{
    int ret=1;
    while(val>1)
    {
        ret*=val;
        --val;

        // ret*= val--;
    }
    return ret;
}

size_t countCalls()
{
    static size_t callCnt=0;//局部静态对象
    return ++callCnt;
}

void ch6_1()
{
    //函数定义
    std::cout<<"7!="<<fact(7)<<std::endl;
    for(int i=0;i<12;i++)
    {
        std::cout<<countCalls()<<std::endl;
    }
}


void reset(int *pointeri)
{
    //指针形参
    *pointeri=10; //改变了 pointeri所指对象的值
    pointeri=0; // 只改变了 pi的局部拷贝(pointeri)，没改变 pi
}

void reset(int &ival)
{
    //传引用参数
    ival=25;
}

bool isShorter(const std::string& s1,const std::string& s2)
{
    //使用引用避免拷贝
    return s1.size() <s2.size();
}

std::string::size_type find_char(const std::string &s,char c,std::string::size_type& occurs)
{
    //使用引用形参返回额外信息
    auto ret = s.size();
    for(decltype(ret) i=0;i<s.size();++i)
    {
        if(s[i]==c)
        {
            if(ret==s.size())
            {
                ret =i;
            }
            ++occurs;
        }
    }
    return ret;
}

bool isSentence(const std::string &s)
{
    std::string ss(s);
    std::string::size_type occurs=0;
    return find_char(ss,'.',occurs)==ss.size()-1 && occurs==1;
}

void print(const int*);
void print(const int[]);
void print(const int[10]);

//数组形参，是以指针形式传递的，可以采用以下方式管理
//1.使用标记指定数组长度
void print(const char *cp)
{
    if(cp)
    {
        while(*cp)
        {
            std::cout<<*cp++;
        }
    }
}

//2.使用规范库规范
void print(const int * begin,const int* end)
{
    while(begin !=end)
    {
        std::cout<<*begin++<<std::endl;
    }
}

//3.显示传递一个表示数组大小的形参
void print(const int arr[],size_t size)
{
    for(size_t i=0;i!=size;++i)
    {
        std::cout<<arr[i]<<std::endl;
    }
}


void errorMsg(std::initializer_list<std::string> il)
{
    for(auto begin=il.begin();begin!=il.end();++begin)
    {
        std::cout<<*begin<<std::endl;
    }
}
void ch6_2()
{
    //参数传递
    int i=5,*pi=&i;
    std::cout<<"i="<<i<<std::endl;
    reset(pi);
    std::cout<<"*pi="<<*pi<<"\t &pi="<<&pi<<std::endl;
    reset(i);
    std::cout<<"int &ival=i,ival=25,i="<<i<<std::endl;

    std::string s1("DaShi"),s2("XiaoYi");
    std::cout<<s1<<" short than "<<s2<<" is "<<isShorter(s1,s2)<<std::endl;
    std::string::size_type occurs=0;
    auto firstIndex = find_char(s2,'i',occurs);
    std::cout<<" find i in XiaoYi ,first index="<<firstIndex<<",occurs="<<occurs<<std::endl;
    std::cout<<s2<<" is a sentence,which is "<<isSentence(s2)<<std::endl;

    errorMsg({s1,s2});
}

void swap(int &v1,int &v2)
{
    if(v1==v2)
    {
        return;
    }
    int temp=v2;
    v2=v1;
    v1=temp;
}

bool sub_strrange(const std::string &s1,const std::string &s2)
{
    if(s1.size()==s2.size())
    {
        return s1==s2;
    }
    auto size = s1.size()< s2.size() ? s1.size():s2.size();
    for(std::string::size_type i=0;i<size;++i)
    {
        if(s1[i]!=s2[i])
        {
            return false;
        }
    }
    return true;
}

char& getVal(std::string s,std::string::size_type ix)
{
    if(ix<s.size())
    {
        return s[ix];
    }
    else
    {
        return s[0];
    }
}

//递归
int factorial(int val)
{
    if(val>1)
    {
        return factorial(val-1)*val;
    }
    return 1;
}

typedef  int arrT[10];
using arrT1=int[10];
arrT* func(int i);
void ch6_3()
{
    //返回值和类型
    int v1=23,v2=34;
    std::cout<<"before swap:v1="<<v1<<",v2="<<v2<<std::endl;
    swap(v1,v2);
    std::cout<<"after swap:v1="<<v1<<",v2="<<v2<<std::endl;

    std::string s1("github"),s2("github.com");
    std::cout<<"sub_strrange("<<s1<<","<<s2<<")="<<sub_strrange(s1,s2)<<std::endl;

    //不要返回局部对象的引用或指针

    //引用返回左值
    std::string notion("input your name");
    std::cout<<notion<<std::endl;
    getVal(notion,0)='I';
    std::cout<<notion<<std::endl;

    std::cout<<"factorial(6)="<<factorial(6)<<std::endl;
}

//函数重载
void ch6_4()
{
//判断两个形参的类型是否相异
    //一个拥有顶层const的形参无法和另一个没有顶层const的形参区分开来


}

typedef std::string::size_type sz;

void screen(sz height=10,sz width=6,char background=' ')
{
    std::cout<<height*width<<std::endl;

}

inline const std::string & shorterString(const std::string & s1,const std::string &s2)
{
    assert(s1.empty()==false);
    assert(s2.empty()==false);
    return s1.size()<s2.size() ?s1:s2;
}
void ch6_5()
{
    screen();
    screen(23,2);
    std::string s1("inline"),s2("inline function");
    std::cout<<shorterString(s1,s2)<<std::endl;
}

void ch6_6()
{
    //函数匹配
    //1.确定候选函数和可行函数
    //2.寻找最佳匹配
    //3.
}


bool lengthCompare(const std::string &s1,const std::string &s2)
{
    return s1.size()>s2.size();
}

bool (*pf)(const std::string&,const std::string&);
//函数指针
void ch6_7()
{
    std::string s1("C#"),s2("C++");
    bool b1 = lengthCompare(s1,s2);
    pf = lengthCompare;
    bool b2 = pf(s1,s2);
    bool b3 =(*pf)(s1,s2);
    std::cout<<"b1="<<b1<<",b2="<<b2<<",b3="<<b3<<std::endl;
}
void testCh6()
{
    std::cout<<"Starting test chapter 6"<<std::endl;
    ch6_1();
    ch6_2();
    ch6_3();
    ch6_4();
    ch6_5();
    ch6_6();
    ch6_7();

}
