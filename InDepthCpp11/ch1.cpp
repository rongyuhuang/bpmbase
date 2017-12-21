#include<iostream>
#include<functional>
#include<set>
#include<vector>
#include<string>
#include<map>
#include<memory>
#include<algorithm> //for std::for_each
#include<tuple>
#include"iteratorimp.h"

class Foo{
public:
    static const int Number=0;
    int x;
};
void ch1_1()
{
    std::cout<<"1.1类型推导"<<std::endl;
    /*使用auto声明的变量必须马上初始化，以让编译器推导出它的实际类型
     * 1.当不声明为指针或引用时，auto的推导结果和初始化表达式抛弃引用和cv限定符后类型一致
     * 2.当生命为指针或引用时，auto的推导结果将保持初始化表达式的cv属性
    */
    auto x=7; //x推导为 int
    auto pi = new auto(1); //pi推导为 int*
    const auto *v=&x,u=6;//v推导为 const int*,u推导为const int
    static auto y = 0.0; //y推导为double
    //auto int r;
    //auto s;

    x=0;
    auto *a=&x; //a推导为 int*,auto为int
    auto b= &x;//b推导为int*,auto 为int*
    auto&c = x; //c推导为int&,auto 为int
    auto d = c; //d推导为int,auto为int
    const auto e = x;// e推导为const int,auto 为int
    auto f =e; //f推导为int
    const auto& g = x; //g推导为const int&,auto为int
    auto& h=g; // h推导为const int&,auto为const int

    /*decltype 能获知表达式的类型.decltype(exp)的推导规则如下：
     * 1.exp是标识符、类访问表达式，decltype(exp)和exp的类型一致
     * 2.exp是函数调用，decltype(exp)和返回值的类型一致:函数返回是一个纯右值/prvalue，只有类类型可以携带cv限定符
     * 3.若exp是左值，则decltype(exp)是exp类型的左值引用；否则和exp类型一致
     *
    */
    //1)对于标识符、类访问表达式
    int n1=0;
    volatile const int &x1 =n1;
    decltype(n1) a1=n1; //a1 -> const int
    decltype(x1) b1 = x1; //b1 -> const volatile  int
    decltype(Foo::Number) c1=0; //c1 -> const int
    Foo fo;
    decltype(fo.x) d1 = 0; // d1 -> int 类访问表达式

    //2)对于函数调用


    //3)带括号的表达式和加法运算表达式
    decltype((fo.x)) e1 = d1; // e -> int &; fo.x是一个左值，括号表达式也是个左值，根据推导规则3，则e是左值
    int f1=12,g1=23;
    decltype(f1+g1) h1=23; // h1-> int,f1+g1返回一个右值
    decltype(f1+=g1) j1 = h1; //j1 -> int&，f1+=g1返回一个左值

    //decltype的应用
    //1.简化变量声明
    //2.auto和decltype结合：返回类型后置语法
}

//C++98/03 外敷类
template <typename Val>
struct str_map{
    typedef std::map<std::string ,Val> type;
};

template <typename Val>
using str_map_t =std::map<std::string,Val>;


//identity外敷模板禁用了形参val的类型自动推导
template <typename T>
struct identity{
    typedef T type;
};

template <typename T=int>
void func1(typename identity<T>::type val,T=0)
{}

void ch1_2()
{
    /*模板的细节改进
     * 1.在C++98/03的泛型编程中，>>会被编译器解释成为右移操作符，而不是模板参数列表的结束；
     * 在C++11标准中，要求编译器对模板的右尖括号单独处理
     * 2.typedef的定义方法和变量的声明类似，可凸显C++中的语法一致性，但有时会提高代码阅读难度
     * using 后面紧跟新标识符，与赋值语法类似，将 现有的类型赋给新类型将使用自动推导出的参数类型
     * 3.函数模板的默认模板参数：所有模板参数都有默认参数时，函数模板的调用与普通函数一致，若显式指定则从左往右填充；
     * 对于类模板，所有模板参数都有默认值时，在使用时也必须在模板名后跟 <> 来实例化；
     * 当默认模板参数和模板参数自动推导同时使用时，若函数模板无法自动推导出参数类型，则编译器将使用默认模板参数，
     * 否则将使用自动推导出的参数类型
    */

    str_map<int>::type map1;

    str_map_t<int> map2;

    func1(123); //T -> int：func1的模板参数中T的默认类型为int,在func1(123)中,val参数为int
    func1(12,1.3); // T -> double,在func1(12,1.3)中，第二个参数1.3为double,T优先推导为double
}

struct Foo3{
public:
    int x;
    std::string str="default";
protected:
    double y;
public:
    Foo3(int x_,std::string str_,double y_):
        x(x_),str(str_),y(y_){std::cout<<"Foo3 constructor"<<std::endl;}
};

struct ST1{
    int x;
    int y;
private:
    double z;

};

struct Foo4{
    ST1 st;
    int x;
    int y;
};

class FooVector{
private:
    std::vector<int> content_;

public:
    FooVector(std::initializer_list<int> il)
    {
        for(auto it = il.begin();it!=il.end();++it)
        {
            content_.push_back(*it);
        }
    }
    void print()
    {
        for(const auto x:content_)
        {
            std::cout<<x<<"\t";
        }
        std::cout<<std::endl;
    }
};
void ch1_3()
{
    /*列表初始化
     * 1.在C++98/03中，对于普通数组和PROD类型，可以使用初始化列表进行初始化
     * 2.在C++11中，可以直接在变量名后面跟上初始化列表来进行对象初始化
     * 3.在使用初始化列表时，以下类型会被C++认为是聚合体：
     * 1)类型是一个普通数组
     * 2）类型是一个类，且 无用户自定义的构造函数、无私有或保护的非静态数据成员、无基类、无虚函数、不能有{}和= 直接初始化的非静态数据成员
     * 4.对于聚合类型，初始化列表相当于对其中的每个元素分别赋值；
     * 而对于非聚合类型，需要先自定义一些合适的构造函数，此时使用初始化列表将调用它对应的构造函数
     * 5.初始化列表可防止类型收窄
     * 6.
    */
    int x[]={1,4,5};
    float y[4][3]={
        {1,3,5},
        {2,4,6},
        {7,9,11},
        {8,10,12}
    };
    Foo f1{3}; //含有一个公有的静态数据成员，一个公有的数据成员，传一个int就可以初始化 Foo
    std::cout<<f1.Number<<"\t"<<f1.x<<std::endl;
    //Foo3 f31{12,1.2}; //error：Foo3有一个保护的非静态数据成员
    Foo3 f32={12,"Mo",2.5};
    std::cout<<f32.x<<"\t"<<f32.str<<std::endl;

    Foo4 f41={{},12,23};
    std::cout<<f41.st.x<<"\t"<<f41.st.y<<"\t"<<f41.x<<"\t"<<f41.y<<std::endl;


    /*初始化列表
     * 1.可使用 std::initializer_list这个轻量级的类模板实现 任意长度的初始化列表
     * 2.std::initializer_list是一个轻量级容器类型，内部定义了iterator等容器必需的概念
     * 3.对于std::initializer_list<T>而言，他可以接受任意长度的初始化列表，但要求必须是同类型T或者可转换为T
     * 4.它有三个接口:size(),begin(),end()
     * 5.他只能被整体初始化或赋值
     * 6.std::initializer_list的访问只能通过begin()和end()循环遍历，遍历时取得的值是只读的
     * 7.std::initializer_list内部不保存初始化列表中的元素，只存储了列表中元素的引用
    */
    FooVector fv1={123,2342,1213,123,3424,546,63,512};
    fv1.print();
    std::initializer_list<int> il1;
    std::cout<<il1.size()<<std::endl;
    il1 ={23,23,5,6,7,78,34};
    std::cout<<il1.size()<<std::endl;

    /*防止类型收窄
     *
    */
    int a1=1.1;
    //int b1 ={10.2}; //error

    const int c1=1024, d1=2;
    char e1 = c1;
    //char f1 ={c1}; //error
    char g1 = d1;
    char h1 ={d1};
}

void doCout(int n)
{
    std::cout<<n<<"\t";
}
void ch1_4()
{
    /*基于范围的for循环
     * 1.在遍历时想修改容器中的值，需用使用引用
     * 2.遍历容器时，键值的可读性取决于容器的类型，比如map的val.first为只读，set的val为只读
     * 3.：后面的表达式只会被执行一次
    */
    std::vector<int> iv1={12,434,5,8,6,2};
    //原始方案
    for(auto it=iv1.begin();it !=iv1.end();++it)
    {
        std::cout<<*it<<"\t";
    }
    std::cout<<std::endl;

    //for_each算法
    std::for_each(iv1.begin(),iv1.end(),doCout);
    std::cout<<std::endl;

    //基于范围的for
    for(const auto x:iv1)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    std::cout<<"range(15)"<<std::endl;
    for(int i:range(15))
    {
        std::cout<<i<<"\t";
    }
    std::cout<<std::endl;

    std::cout<<"range(2,6)"<<std::endl;
    for(int i:range(2,6))
    {
        std::cout<<i<<"\t";
    }
    std::cout<<std::endl;

    std::cout<<"range(1,15,3)"<<std::endl;
    for(int i:range(1,15,3))
    {
        std::cout<<i<<"\t";
    }
    std::cout<<std::endl;
}

void func5()
{
    std::cout<<__FUNCTION__<<std::endl;
}

class Foo5{
public:
    static int foo_func(int a)
    {
        std::cout<<__FUNCTION__<<"("<<a<<")"<<std::endl;
        return a;
    }
};

class Bar5{
public:
    int operator()(int a)
    {
        std::cout<<__FUNCTION__<<"("<<a<<")"<<std::endl;
        return a;
    }
};

class A5{
private:
    std::function<void()> callback_;

public:
    A5(const std::function<void()>& f):
        callback_(f){}
    void notify(void)
    {
        callback_();
    }
};

class Foo5_1{
public:
    void operator()(void)
    {
        std::cout<<__FUNCTION__<<std::endl;
    }
};

void call_when_even(int x,const std::function<void(int)>& f)
{
    if(!(x&1))
    {
        f(x);
    }
}

void output(int x)
{
    std::cout<<x<<" ";
}

void ch1_5()
{
    /*std::function和bind绑定器
     * 1.可调用对象
     * 1）函数指针
     * 2）具有operator()成员函数的类
     * 3）可被转换为函数指针的类对象
     * 4）类成员(函数)的指针
    */
    //std::function的基本用法
    std::function<void(void)> fr1 =func5;  //绑定一个普通函数
    fr1();
    std::function<int(int)> fr2 = Foo5::foo_func; //绑定一个类的静态函数
    std::cout<<fr2(123)<<std::endl;

    Bar5 bar;
    fr2 = bar;  //绑定一个仿函数
    std::cout<<fr2(45)<<std::endl;

    //std::fuction作为回调函数
    Foo5_1 foo5;
    A5 a5(foo5);
    a5.notify();

    //std::function作为函数入参
    for(int i =0;i<10;i++)
    {
        call_when_even(i,output);
    }
    std::cout<<std::endl;

    /*std::bind将可调用对象与其参数一起进行绑定，绑定后的结果可以用std::function保存，并延迟调用到任何我们需要的时候
     * 1.将可调用对象与其参数仪器绑定成一个仿函数
     * 2.将多元可调用对象转成一元或者(n-1)元调用对象，即只绑定部分参数
     *
    */

    auto fr3 = std::bind(output,std::placeholders::_1);
    for(int i=10;i<25;++i)
    {
        call_when_even(i,fr3);
    }
}

class A6{
public:
    int i_=0;

    void func(int x,int y)
    {
        //auto x1 = []{return i_;}; //没有捕获外部变量i_
        auto x2 = [=]{return i_+x+y;}; //
        std::cout<<x2()<<std::endl;
        auto x3 = [&]{return i_+x+y;};
        std::cout<<x3()<<std::endl;
        auto x4 = [this]{return i_;}; //捕获this指针
        std::cout<<x4()<<std::endl;
        //auto x5= [this]{return i_+x+y;}; //没有捕获 x,y
        auto x6 = [this,x,y]{return i_+x+y;};
        std::cout<<x6()<<std::endl;
        auto x7 = [this]{return i_++;};
        std::cout<<x7()<<std::endl;
    }
};

void ch1_6()
{
    A6 a6;
    a6.func(1,2);
}

void ch1_7()
{
    std::tuple<const char*,int> tp = std::make_tuple("sendPack",12);

    int x=1,y=2;
    std::string s="Hello";
    auto tp1 = std::tie(x,y,s);
    //获取元组
    //std::cout<<tp1.get<1>()<<"\t"<<tp1.get<0>()<<std::endl;
    int x1,y1;
    std::string s1;
    std::tie(x1,y1,s1) = tp1;
    std::cout<<x1<<"\t"<<y1<<"\t"<<s1<<std::endl;
    //只解第三个
    std::tie(std::ignore,std::ignore,s1)=tp1;

    //创建右值的引用元组
    std::map<int,std::string> m;
    m.emplace(std::piecewise_construct,std::forward_as_tuple(10),
              std::forward_as_tuple(20,'a'));
}

void ch1_8()
{}
void testCh1()
{
    std::cout<<"Start test chapter 1"<<std::endl;
    ch1_1();
    ch1_2();
    ch1_3();
    ch1_4();
    ch1_5();
    ch1_6();
    ch1_7();
}
