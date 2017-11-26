#include<iostream>
#include<Sales_item.h>
void ch2_1_2()
{
    bool b=42;
    std::cout<<"b\'s type is bool,when assign 42 to b,b="<<b<<std::endl;

    int i=b;
    std::cout<<"b=true,i=b="<<i<<std::endl;

    i=3.14;
    double pi = i;//3.14的小数部分被舍弃，只保留了3到i
    std::cout<<"int i=3.14;double pi=i="<<pi<<std::endl;
    unsigned char c= -1;
    std::cout<<"when char occupy 8 bit, unsigned char c= -1= "<<c<<std::endl;
    signed char c2 =256;
    std::cout<<"when char occupy 8 bit,signed char c2=256 = "<<c2<<std::endl;//c2是 未定义的

    i=42;
    if(i)
    {
        i=0;
    }
    std::cout<<"when i is ture,assign 0 to i:i="<<i<<std::endl;

    unsigned u=40;
    i=-42;
    std::cout<<"int i=-42,i+i="<<i+i<<std::endl;
    std::cout<<"unsigned u=40,u+i="<<u+i<<std::endl;

    for(i=10;i>=0;i--)
    {
        std::cout<<i<<"\t";
    }
    std::cout<<std::endl;

//    for(unsigned ui=10;ui>=0;ui--)
//    {
//        //会死循环
//        std::cout<<ui<<"\t";
//    }
//    std::cout<<std::endl;

    u=11;
    while(u>0)
    {
        u--;
        std::cout<<u<<"\t";
    }
    std::cout<<std::endl;


}

/*
 * 字面值常量
*/
void ch2_1_3()
{
    std::cout<<"a"<<std::endl;
    std::cout<<"Hello,world."<<std::endl;
    std::cout<<"a readlly,really long string literal "
               "that apans tow lines"<<std::endl;
    std::cout<<"\n";
    std::cout<<"\t Hi!\n";

    std::cout<<"Hi \x4do\115!\n";

    std::cout<<"\115"<<"\n";


}

/*
 * 变量
*/
void ch2_2_1()
{
    //变量定义


    int sum=0,values,unitsOfSlod=0;

    Sales_item book;
    std::string isbn("12-sddd-23");

    double price=12.5,discount = price*0.25;

    //列表初始化
    int unitsSlod1=0;
    int unitsSlod2={0};
    int unitsSlod3{0};
    int unitsSlod4(0);
    //当用于内置变量时，如果使用列表初始化并且初始值存在丢失信息的风险，编译器将报错
    double pi = 3.141592653;
    //int intPi{pi}; // error: C2397: conversion from 'double' to 'int' requires a narrowing conversion
    int intPi(pi); //转化不会出现编译问题，但会丢失精度

    //C++支持分离式编译
    //变量只能被定义一次，但可以被多次声明；如果想声明一个变量而非定义它，需要在变量类型前加extern，并且不要显示地初始化变量\n";
}

int reused=42;
void ch2_2_1_1()
{
    //嵌套的作用域
    int unique=0;
    std::cout<<reused<<" "<<unique<<std::endl;
    int reused=0;
    //使用局部变量
    std::cout<<reused<<" "<<unique<<std::endl;
    //指定调用全局变量
    std::cout<<::reused<<" "<<unique<<std::endl;

}

void ch2_3_1()
{
    //引用

    int ival=1024;
    int &refval = ival;
    //int &refVal2; //引用必须初始化
    int &refVal2 = refval;
    int i = refVal2;
    //引用类似的初始值必须是一个对象，对应的类型应该一致
    double dval =3.14;
    //int &refVal3 = dval;

}

void ch2_3_2()
{
    //指针
    int *ip1,*ip2;//ip1,ip2皆为指向int类型对象的指针
    double *dp1,dp2;//dp1是指向double的指针

    int ival=42;
    int *p = &ival;    //访问对象地址
    std::cout<<*p<<std::endl; //利用指针访问对象
    //指针的四种状态：
    //指向一个对象
    //指向紧邻对象所占用空间的下一个位置
    //没有指向任何对象，空指针
    //无效指针

}
void testCh2()
{
//    ch2_1_2();
//    ch2_1_3();
//    ch2_2_1();
    ch2_2_1_1();
    ch2_3_1();
    ch2_3_2();
}
