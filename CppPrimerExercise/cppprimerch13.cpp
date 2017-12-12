#include<string>
#include<vector>
#include<map>
#include<set>
#include<memory>
#include<iostream>
#include<sstream>
#include<fstream>

#include"sales_data.h"
#include"message.h"
#include"strvec.h"

void fcn(std::vector<int>);//参数进行拷贝初始化

class HasPtr{
    friend void swap(HasPtr&,HasPtr&);
public:
    HasPtr(const std::string &s=std::string()):ps(new std::string(s)),i(0){}
    ~HasPtr() {delete ps;}
    HasPtr(const HasPtr& rhs):ps(new std::string(*rhs.ps)),i(rhs.i){}
//    HasPtr& operator=(const HasPtr& rhs){
//        auto tempS = new std::string(*rhs.ps); //拷贝底层string
//        delete ps;   //释放旧内存
//        ps = tempS;  //从右侧运算对象拷贝数据到本对象
//        i = rhs.i;
//        return *this;
//    }
    //移动构造函数
    HasPtr(HasPtr&& rhs)noexcept:ps(rhs.ps),i(rhs.i){rhs.ps=0;}

    HasPtr& operator =(HasPtr rhs){swap(*this,rhs);return *this;}
private:
    std::string *ps;
    int i;
};
void swap(HasPtr& lhs,HasPtr& rhs)
{
    using std::swap;
    swap(lhs.ps,rhs.ps);
    swap(lhs.i,rhs.i);
}
void ch13_1()
{
    std::string dots(10,'.'); //直接初始化
    std::string s(dots); //直接初始化
    std::string s2 = dots ; //拷贝初始化
    std::string bookNo = "9-11-12121-9"; //拷贝初始化
    std::string nines=std::string(20,'9'); //拷贝初始化
    //拷贝初始化不仅在用 = 定义变量时发生，在下列情境中也会发生：
    /*
     * 将一个对象作为实参传递给一个非引用类型的形参
     * 从一个返回类型为非引用类型的函数中返回一个对象
     * 用花括号列表初始化一个数组中的元素或一个聚合类中的成员
    */

    //拷贝初始化的限制
    //如果使用的初始化值要求通过一个explicit的构造函数来进行类型转化，那么使用拷贝初始化还是直接初始化就无关紧要了
    std::vector<int> v1(10); //直接初始化
    //std::vector<int> v2=10; //error:接受大小参数的构造函数是explicit
    //fcn(10)      //不能用一个explicit的的构造函数拷贝一个实参
    fcn(std::vector<int>(10));

    //析构函数执行顺序：函数体，销毁成员（成员按初始化顺序的逆序销毁）
    //调用时机：
    /*变量在离开其作用域
     * 当一个对象被销毁时，其成员被销毁
     * 容器被销毁时，其元素被销毁
     * 对于动态分配的对象，当对指向它的指针应用delete运算符时被销毁
     * 对于临时对象，当创建它的完整表达时结束时
    */
    {
        Sales_data *p = new Sales_data; //p内置指针
        auto p2 = std::make_shared<Sales_data>(); //p3 是 shared_ptr
        Sales_data item(*p); //拷贝构造函数
        std::vector<Sales_data> vec; //局部对象
        vec.push_back(*p2);
        delete p;
    }

    //三/五法则
    /*需要析构函数的类也需要拷贝和赋值操作
     *需要拷贝操作的类也需要赋值操作，反之亦然（比如一个类对每个对象分配一个独一无二的ID）
     * 只能对具有合成版本的成员函数使用=default,即默认的构造函数或者拷贝控制成员
     * 大多数类应该定义默认构造函数、拷贝构造函数和拷贝赋值运算符，无论是显式的还是隐式的
     * 析构函数不能是删除的成员
     * 合成的拷贝控制成员可能是删除的
    */
}

void ch13_234()
{
    Message m1("Hi,"),m2("C++ Primer");
    Folder f1,f2;
    m1.save(f1);
    m1.save(f2);
    m2.save(f1);
    m1.printInfo();
    m2.printInfo();
    f1.printInfo();
    f2.printInfo();

}

void ch13_5()
{
    StrVec sv;
    sv.push_back("hi");
    sv.push_back("c++");
    sv.push_back("5th edit.");
    for(const auto& s:sv)
    {
        std::cout<<s<<"\t";
    }
    std::cout<<std::endl;
    auto beg = sv.begin();
    while(beg !=sv.end())
    {
        std::cout<<*beg++<<"\t";
    }
    std::cout<<std::endl;
}

void ch13_6()
{
    HasPtr hp("Hi"),hp2;
    hp2 = hp; //hp是左值，hp通过拷贝构造函数来拷贝
    hp = std::move(hp2); //移动构造函数移动hp2

    StrVec sv;
    std::string s1="some one like you";
    sv.push_back(s1); // push_back(const std::string&)
    sv.push_back("haha");//push_back(std::string&&)

    //右值和左值引用成员函数
    /*
     * 1.在成员函数的参数列表后面放置一个 引用限定符/&，只能向可修改的左值赋值
     * 2.在成员函数的参数列表后面放置2个 引用限定符/&&，只能向可修改的右值赋值
     * 3.一个函数可以同时用const和引用限定，const在引用限定前
    */

}
void testCh13()
{
    std::cout<<"Start test chapter 13"<<std::endl;
    ch13_1();
    ch13_234();
    ch13_5();
    ch13_6();
}

void fcn(std::vector<int>vi)
{
    for(auto const x: vi)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;
}
