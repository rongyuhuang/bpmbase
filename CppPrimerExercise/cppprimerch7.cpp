#include<iostream>
#include<vector>
#include<string>
#include"sales_data.h"
#include"screen.h"

void ch7_1()
{
    Sales_data total;
    if(read(std::cin,total))
    {
        Sales_data tran;
        while(read(std::cin,tran))
        {
            if(total.isbn()==tran.isbn())
            {
                total.combine(tran);
            }
            else
            {
                print(std::cout,total)<<std::endl;
                total = tran;
            }
        }
        print(std::cout,total)<<std::endl;
    }
    else
    {
        std::cerr<<"No data"<<std::endl;
    }
}

void ch7_3()
{
    Screen myScreen(3,5,'a');
    char ch = myScreen.get();
    std::cout<<ch<<std::endl;
    ch =myScreen.get(0,0);
    std::cout<<ch<<std::endl;

    myScreen.move(1,3).set('#');
    std::cout<<myScreen.get(1,3)<<std::endl;

    myScreen.display(std::cout);
    std::cout<<std::endl;
    const Screen blank(1,3,' ');
    blank.display(std::cout);
    std::cout<<std::endl;
}


void ch7_4()
{
    //类的作用域

}


class ConstRef
{
public:
    ConstRef(int ii);
    private:
    int i;
    const int ci;
    int& ri;
};
ConstRef::ConstRef(int ii):i(ii),ci(ii),ri(ii)
{}


class Debug
{
public :
    constexpr Debug(bool b=true):hw(b),io(b),other(b){}
    constexpr Debug(bool h,bool i,bool o):hw(h),io(i),other(o){}
    constexpr bool any(){return hw || io||other;}
    void set_io(bool b){io=b;}
    void set_hw(bool b){hw=b;}
    void set_other(bool b){other=b;}
private:
    bool hw;
    bool io;
    bool other;
};
void ch7_5()
{
    //构造函数的初始化
    //1.类中成员有const,引用，或者某种未提供默认构造函数的类类型时，必须通过构造函数初始值列表为这些成员提供初始值
    //2.成员初始化的顺序，与他在类中定义的出现顺序一致：
    //最好令构造函数初始值的顺序与成员声明的顺序保持一致，如果不可能的话，尽量避免使用某些成员初始化其他成员

    //3.如果一个构造函数为所有参数都提供了默认实参

    //委托构造函数
    //一个委托构造函数使用它所属类的其他构造函数执行他的初始化过程

    //默认构造函数的作用
    //1.默认初始化在以下情况下发生：
    //a.在块作用域内不使用任何初始值定义一个非静态变量或者数组时
    //b.当一个类本身含有类类型的成员，其使用合成的默认构造函数时
    //c.当类类型的成员没有在构造函数初始值列表中显示的初始化时

    //值初始化在以下情况下发生：
    //a.在数组初始化的过程中，提供的初始值数量小于数组的大小时
    //b.不适用初始值定义一个局部静态变量时
    //c.形如T()的表达是显式的请求值初始化时，比如vector

    //隐式的类类型转换
    //如果构造函数值接受一个实参，它实际上定义了转换为此类类型的隐式转换机制
    //Note：只允许一步类类型转换，类类型转换不总是有效
    //如果想抑制 构造函数定义的隐式转换，在可将对应的构造函数声明 加  explicit 加以阻止
    //explicit 构造函数只能用于直接初始化
    std::string bookid="9-999-1234-9";
    Sales_data total("9-999-1234-9",12,30);
    total.combine(bookid);
    (std::cout,total);
    std::cout<<std::endl;

    //聚合类

    //字面值常量类
    //constexpr 构造函数
    constexpr Debug io_sub(false,true,true);
    if(io_sub.any())
    {
        std::cerr<<"print appropriate error message"<<std::endl;
    }
    constexpr Debug prod(false);
    if(prod.any())
    {
        std::cerr<<"print an error message"<<std::endl;
    }
}


class Account
{
private:
    std::string owner;
    double amount;
    static double initRate(){return 0.03;}
    static  double interestRate;

public:
    //Account(double r):interestRate(r){amount=1000;}
    void calculate(){amount+=amount*interestRate;}
    static double rate(){return interestRate;}
    static void rate(double r){interestRate=r;}
};
double Account::interestRate=Account::initRate();
void ch7_6()
{
    //类的静态成员
    //即使一个常量静态数据成员在类内部被初始化了，通常情况下 也应该在类的外部定义一下该成员
    //静态、指针成员可以是不完全类型，数据成员必须是完全类型
    double r;

    Account::rate(25.6);
    r=Account::rate();
    std::cout<<r<<std::endl;
}
void testCh7()
{
    std::cout<<"Start test chapter 7"<<std::endl;
    //ch7_1();
    ch7_3();
    ch7_4();
    ch7_5();
    ch7_6();
}
