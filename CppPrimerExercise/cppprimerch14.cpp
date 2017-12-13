#include<iostream>
#include<functional>
#include<string>
#include<vector>
#include<set>
#include<map>

#include"sales_data.h"
#include"strvec.h"

void ch14_1()
{
    //除了重载的函数调用运算符 operator()之外，其它重载运算符不能含有默认实参
    //通常情况下，不应该重载逗号、取地址、逻辑与和逻辑或运算符
    //重载运算的一些建议
    /*1.如果类执行IO操作，则定义移位运算符使其与内置类型的IO保持一致
     * 2.如果类的某个操作是检查相等性，则定义 operator==,通常还意味着定义 operator!=
     * 3.如果类包含在一个内在的单序比较操作，则定义 operator<,通常还意味着需要定以其他比较关系操作
     * 4.重载运算符的返回类型通常情况下应该与内置版本的返回类型兼容
    */
}

void ch14_23()
{
    /* 1.输入输出运算符必须是非成员函数
     * 2.输出运算符尽量减少格式化操作
     * 3.输入运算符必须处理输入可能失败的情况
     *
    */
    //重载输入运算符
    Sales_data sd1;
    std::cin>>sd1;
    std::cout<<sd1.isbn()<<std::endl;
    //重载输出运算符
    std::cout<<sd1<<std::endl;

    /* 1.operator ==,operator != 非成员函数
     * 2.operator == 应该能判断依租给定对象是否含有重复数据
     * 3.相等运算符具有传递性
     * 4.如果某个类在逻辑上具有相等性的含义，应该定义operator==,可以使得用户更容易使用标准库算法处理这个类
     *
    */
    Sales_data sd2;
    std::cin>>sd2;
    bool result = sd2==sd1;
    std::cout<<sd1;
    std::cout<<" == "<<sd2<<" is "<<(result?"true":"false")<<std::endl;

    /*通常关系运算符应该
     * 1.定义顺序关系，令其与关联容器中对关键字的要求一直，并且
     * 2.如果类同时也含有 ==运算符的话，则定义一种关系令其与==保持一致。
     * 特别是 如果两个对象是 !=的，那么其中一个对象应该<另外一个
     * 3.如果存在唯一一种逻辑可靠的<定义，则应该考虑为这个类定义<运算符。
     * 如果类同时还包含==，则当且仅当<的定义和==产生的结果一致是才定义<运算符
    */
}

void ch14_45()
{
    /*赋值运算符 必须定义为 成员函数
    */
    //    StrVec sv;
    //    sv = {"Hi","C++ Primer"};
    //    auto beg = sv.begin();
    //    while(beg != sv.end())
    //    {
    //        std::cout<<*beg<<"\t";
    //        ++beg;
    //    }
    //    std::cout<<std::endl;

    //复合赋值运算符
    Sales_data sd1,sd2;
    std::cin>>sd1>>sd2;
    if(sd1.isbn()==sd2.isbn())
    {
        sd1 += sd2;
        std::cout<<sd1<<std::endl;
    }
    else
    {
        std::cout<<"sd1 and sd2 is not same ISBN"<<std::endl;
    }
    /*下标运算符必须是成员函数
     * 1.如果一个类包含下标运算符，则它通常会定义两个版本：一个返回普通引用，另一个是类的常量成员并且返回常量引用
     *
    */
    StrVec sv1;
    sv1.push_back("C++");
    sv1.push_back("Primer");
    sv1.push_back("5th");
    const StrVec csv1 = sv1;
    //    if(sv1.size()&& sv1[0].empty()==false)
    //    {
    //        sv1[0]="zero";
    //        //csv1[0]="zip";
    //    }
}

void ch14_67()
{
    /*递增递减运算符
     *1.递增递减运算符的类应该同时定义前置版本和后置版本，这些运算符应该被定义为类的成员
     * 2.前置版本应该返回递增或递减后对象的引用；
     * 后置版本应该返回递增或递减前的原值
     * 3.为了区分前置和后置运算符，后置版本接受一个额外的(不被使用的)int类型的形参，
     * 当我们使用后置版本时，编译器为这个形参提供一个值为0的实参
    */

    /*成员访问运算符
     * 1.箭头运算符必须是类的成员，解引用运算符通常是类的成员
     * 2.重载的箭头运算符必须返回类的指针或者自定义了箭头运算符的某个类的对象
    */


}
struct absInt{
    int operator()(int val)const
    {
        return val <0 ? -val:val;
    }
};

class PrintString{
public:
    PrintString(std::ostream& o=std::cout,char c=' '):
        os(o),sep(c){}
    void operator()(const std::string s)const {os<<s<<sep;}

private:
    std::ostream& os;
    char sep;
};

int add(int a,int b)
{
    return a+b;
}
struct divide{
    int operator()(int a,int b)
    {
        return a/b;
    }
};
void ch14_8()
{
    /*函数调用运算符
     * 1.函数调用运算符必须是成员函数，一个类可以定义多个不同版本的调用运算符，对应的参数数量或类型应有不同
     * 2.
    */
    int i=-52;
    absInt absObj;
    std::cout<<absObj(i)<<std::endl;

    PrintString printer;
    printer("Hi,C++");
    PrintString errPrinter(std::cout,'\n');
    errPrinter("Hi,C++");

    //标准库定义的函数对象
    std::plus<int> intAdd;
    std::negate<int> intNegate;
    int sum = intAdd(10,20);
    std::cout<<sum<<std::endl;
    sum = intNegate(sum);
    std::cout<<sum<<std::endl;

    /*可调用对象和function
     * 1.可调用对象包括：函数、函数指针、lambda表达式、bind创建的对象以及重载了调用运算符的类
     * 2.不同类型可能具有相同的调用形式
     * 3.解决重载函数的二义性问题，是通过存储函数指针
    */
    //        std::map<std::string,std::function<int(int,int)>> intops={
    //        {"+",add},
    //        {"-",std::minus<int>()},
    //        {"*",[](int i,int j){return i*j;}},
    //        {"/",divide()}};
    std::map<std::string,std::function<int(int,int)>> intops;
    int (*fpAdd)(int,int)=add;
    intops.insert(std::make_pair("+",fpAdd));  // 函数指针
    intops.insert(std::make_pair("-",std::minus<int>()));// 标准库函数对象
    intops.insert(std::make_pair("*",[](int i,int j){return i*j;})); //匿名lambda
    intops.insert(std::make_pair("/",divide())); //自定义函数对象
    intops.insert(std::make_pair("%",[](int i,int j){return i%j;}));
    int a=15,b=3;
    std::cout<<intops["+"](a,b)<<std::endl;
    std::cout<<intops["-"](a,b)<<std::endl;
    std::cout<<intops["*"](a,b)<<std::endl;
    std::cout<<intops["/"](a,b)<<std::endl;
    std::cout<<intops["%"](a,b)<<std::endl;
}

class SmallInt{
    friend SmallInt operator +(const SmallInt&,const SmallInt&);
public:
    SmallInt(int i=0):val(i)
    {
        if(i<0||i>255)
        {
            throw std::out_of_range("Bad SmallInt value");
        }
    }
    //类型转换运算符
    explicit operator int()const {return val;}
    //显式的类型转换符，编译不会将一个显式的类型转换符用于隐式类型转换

private:
    std::size_t val;
};

SmallInt operator +(const SmallInt& lhs,const SmallInt& rhs)
{
    return SmallInt(lhs.val+rhs.val);
}
void ch14_9()
{
    /*类型转换运算符
     * 1.类型转换运算符是一种特殊的成员函数，负责将一个类类型的值转换为其他类型
     * 2.他不能声明返回类型，形参列表必须为空，类型转换函数通常为const
     * 3.如果表达式被用作条件时，显示的类型转化将被隐式的执行
     * 4.IO库定义一个向bool显式类型转换符
    */
    SmallInt si1;
    si1 =3; //3隐式转化为SmallInt，再调用 SmallInt::operator=
    //    std::cout<<si1<<std::endl;
    //    si1 = si1+4;
    //    std::cout<<si1<<std::endl;
    //加上explicit
    std::cout<<static_cast<int>(si1)<<std::endl;
    si1 = static_cast<int>(si1)+4;
    std::cout<<static_cast<int>(si1)<<std::endl;

    /*避免有二义性的类型转换，以下两种情况可能产生多重转换路径
     * 1.两个类提供相同的类型转换
     * 2.类定义了多个转换规则，但这些转换涉及的类类型本身可以通过其他类型联系在一起，最典型的是算术运算符
     * 对于给定的类来说，最好只定义最多一个与算术类型有关的转换规则
     * 3.如果在调用重载函数时我们需要使用构造函数或强制类型转换来改变实参的类型时，这意味着程序设计不足
     *
    */

    /*函数匹配与重载运算符
     * 1.表达式中运算符的候选函数集既应包含成员函数，也应该包括非成员函数
     * 2.如果一个类及提供了转换目标是算数类型的类型转换，也提供了重载的运算符，则将会遇到重载运算符与内置运算符的二义性问题
    */
    SmallInt si2(3);
    SmallInt si3 = si1+si2; // 使用重载的operator+
    //int ival = si3+0; //二义性错误
    std::cout<<static_cast<int>(si3)<<std::endl;

}
void testCh14()
{
    std::cout<<"Start test chapter 14"<<std::endl;
    ch14_1();
    //ch14_23();
    //ch14_45();
    //ch14_67();
    //ch14_8();
    ch14_9();
}
