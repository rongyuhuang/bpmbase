#include<iostream>
#include<thread>
#include<memory>
#include<list>
#include<vector>

#include"strblob.h"

void ch12_1_1()
{
    //    std::shared_ptr<std::string> p1;
    //    std::shared_ptr<std::list<int>> p2;
    //    if(p1 && p1->empty())
    //    {
    //        *p1="hi";
    //    }

    //    std::shared_ptr<int> p3 = std::make_shared<int>(34);
    //    std::cout<<*p3<<" @ "<<p3<<std::endl;
    //    std::shared_ptr<std::string> p4=std::make_shared<std::string>("C++ Primer 5th Ed.");
    //    std::cout<<*p4<<std::endl;
    //    std::shared_ptr<int> p5=std::make_shared<int>();
    //    std::cout<<*p5<<std::endl;

    //    //当进行拷贝或者复制操作时，每个shared_ptr都会记录有多少个其他shared_ptr指向相同的对象
    //    auto p6(p3); //p3,p6 指向相同的对象
    //    std::cout<<*p6<<std::endl;

    //    //当指向一个对象的最后一个shared_ptr被销毁时，shared_ptr类会自动销毁此对象
    //    //如果将shared_ptr存放于一个容器中，而后不再需要全部元素，需要用erase删除不再需要的元素
    //    //程序使用动态内存的三大原因
    //    //1.程序不知自己需要使用多少对象
    //    //2.程序不知道所需对象的准确类型
    //    //3.程序需要在多个对象间共享数据

    //    std::vector<std::string> v1;
    //    {
    //        std::vector<std::string> v2={"a","an","the"};
    //        for(const auto x:v2)
    //        {
    //            std::cout<<x<<" @ "<<&x<<"\t";
    //        }
    //        std::cout<<std::endl;
    //        v1= v2;
    //    }
    //    for(const auto x:v1)
    //    {
    //        std::cout<<x<<" @ "<<&x<<"\t";
    //    }
    //    std::cout<<std::endl;
    StrBlob b1;
    {
        StrBlob b2={"a","an","the"};
        b1=b2;
        b2.push_back("sd");
    }
    std::cout<<b1.size()<<std::endl;
}

void ch12_1_2()
{
    //直接管理内存
    int* pi1 = new int; //pi1指向一个未初始化的int
    std::string* ps1= new std::string ;//初始化为 空字符串

    //直接初始化
    int* pi2 = new int(12); //初始化为指向对象的值为12
    std::string *ps2 = new std::string(10,'s');//*ps为 ssssssssss
    std::vector<int> *pvi = new std::vector<int>{0,1,2,3,4,5,9,6,7,8};

    //值初始化
    int* pi3 = new int(); //值初始化为 0
    std::string *ps3 = new std::string(); //值初始化为 空 string

    //动态分配const对象
    const int* cpi1 = new const int(10124);
    const std::string *cps1 = new const std::string;

    delete pi1;
    delete ps1;
    delete pi2;
    delete ps2;
    delete pvi;
    delete pi3;
    delete ps3;
    delete cpi1;
    delete cps1;

    //delete 指针后，再重置指针，可以提供有限的保护
    cps1=nullptr;

}

std::shared_ptr<int> clone(int p)
{
    return std::shared_ptr<int>(new int(p));
}
void ch12_1_3()
{
    //shared_ptr ,new 混用
    std::shared_ptr<int> p1(new int(12));
    //接受指针参数的智能指针构造函数是 explicit的，因此 不能将 内置指针隐式转化为 智能指针
    int p=23;
    auto pp = clone(p);

    //当讲一个 shared_ptr绑定到一个一个普通指针，就将内存的管理交给了shared_ptr,
    //不该再使用内置指针访问shared_ptr指向的对象
    //也不用用get 初始化另外一个智能指针或者为智能指针赋值
    p1.reset(new int(1024));
}

void ch12_1_4()
{
    //智能指针的使用规范
    //1.不使用相同的内置指针指初始化多个智能指针
    //2.不 delete get()返回的指针
    //3.不使用get()c初始化或reset另一个智能指针
    //4.如果使用get()返回的指针,记住当最后一个对应的智能指针销毁后，该指针就无效了
    //5.使用智能指针管理的资源不是new分配的内存，记住传递给它一个删除器
}


void ch12_1_5()
{
    //unique_ptr
    //拥有其指向的对象，不支持普通的拷贝或者赋值操作
    std::unique_ptr<int> p1(new int(12));
    std::unique_ptr<int> p2 (p1.release());// 将p1指向对象的所有权转移给p2
    std::unique_ptr<int> p3(new int(1024));
    p2.reset(p3.release());// 将p3指向对象的所有权转移给p2

    //删除器
}

void ch12_1_6()
{
    //weak_ptr
    //是一种不控制所指对象生存期的智能指针，它指向一个shared_ptr管理的对象。
    //将一个weak_ptr绑定到一个shared_ptr 不改变 shared_ptr的引用计数；
    //一旦最后一个指向对象的shared_ptr被销毁，对象就会被释放，不管是否有绑定weak_ptr


}
void ch12_2()
{
    typedef int arrT[42];
    int* pi = new arrT; //分配一个数组会得到一个元素类型的指针
    delete []pi;  //释放动态数组

    int *pi1=new int[12];//12个未初始化的int
    int *pi2 = new int[12]();//12个初始化为0
    for(int* p=pi2;p!=pi2+12;++p)
    {
        *p=5;
        std::cout<<*p<<"\t";
    }
    std::cout<<std::endl;
    std::string *ps1 = new std::string[10];//10个初始化为空string
    std::string *ps2 = new std::string[10]();//10个空string

    delete [] pi;
    delete [] pi1;
    delete [] pi2;
    delete [] ps1;
    delete []ps2;


    //智能指针和动态数组
    std::unique_ptr<int[]> up(new int[12]);
    for(size_t i=0;i<12;++i)
    {
        up[i]=i*2;
        std::cout<<up[i]<<"\t";

    }
    std::cout<<std::endl;
    up.release();//delete []

    std::shared_ptr<int[]> sp(new int[12](),[](int*p){delete []p;});
    for(size_t i=0;i<12;++i)
    {
        *(sp.get()+i)=i*3;
        std::cout<<(*(sp.get()+i))<<"\t";
    }
    std::cout<<std::endl;
}

void ch12_3()
{}
void testCh12()
{
    std::cout<<"Start test chapter 12"<<std::endl;
    ch12_1_1();
    ch12_1_2();
    ch12_1_3();
    ch12_2();
    ch12_3();
}
