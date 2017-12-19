#include<iostream>
#include<functional>
#include<set>
#include<vector>
#include<string>
#include<map>
#include<memory>

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


void ch1_3()
{
    /*列表初始化
     * 1.在C++98/03中，对于普通数组和PROD类型，可以使用初始化列表进行初始化
    */

}
void testCh1()
{
    std::cout<<"Start test chapter 1"<<std::endl;
    ch1_1();
    ch1_2();
}
