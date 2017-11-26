#include <iostream>

#include "Sales_item.h"
/*
 * 读取两个数，求它们的和
*/
void ch1_01()
{
    //一个使用IO库的程序
    std::cout<<"Enter two numbers:"<<std::endl;
    int v1=0,v2=0;
    std::cin>>v1>>v2;   //读取两个数据
    std::cout<<v1<<" + "<<v2 <<" = "<<v1+v2<<std::endl;

    return;
}

void ch1_02()
{
    /*
     *  //**此处不能再使用* /,即 注释不能对/ * * /嵌套使用
    */
}

void ch1_03()
{
    //展示 while
    int sum=0,val=1;
    //只要val<=10，循环就会一直执行
    while(val<=10)
    {
        sum+=val;
        val++;
    }
    std::cout<<"Sum of 1 to 10 inclusive is "<<sum<<std::endl;
}

void ch1_04()
{
    //展示for
    int sum=0;
    for(int val=1;val<=10;val++)
    {
        sum+=val;
    }
    std::cout<<"Sum of 1 to 10 inclusive is "<<sum<<std::endl;
}

/*
 * 读取数量不定的输入数据
*/
void ch1_05()
{
    int sum = 0,value=0;
    //读取数据直至EOF，累计所有输入数据并输出
    while(std::cin>>value)
    {
        sum+=value;
    }
    std::cout<<"Sum is "<<sum<<std::endl;
}


/*
 * 使用if来统计输入数字出现的次数
*/
void ch1_06()
{
    int currentVal=0,val=0;//currentVal用来保存当前统计次数的数字，val用来保存当前输入的数字

    if(std::cin>>currentVal)
    {
        int cnt=0;
        while(std::cin>>val)
        {
            if(currentVal == val)
            {
                //输入数据相当则计数
              cnt++;
            }
            else
            {
                //输入数据不同则打印本次统计的数字的出现次数，并重置统计的数字何其出现的次数
                std::cout<<currentVal<<" occurs "<<cnt<<" times."<<std::endl;
                currentVal = val;
                cnt=1;
            }
        }
        std::cout<<currentVal<<" occurs "<<cnt<<" times."<<std::endl;
    }
}


void ch1_07()
{
    //book show
    Sales_item book;
    //输入书籍的 ISBN，volume，price
    std::cin>>book;
    //输出书籍的ISBN，volume,amount,avgprice
    std::cout<<book<<std::endl;
}

void ch1_08()
{
    Sales_item book1,book2;
    std::cin>>book1>>book2;
    std::cout<<book1+book2<<std::endl;
}

void ch1_09()
{
    //成员函数
    Sales_item book1,book2;
    std::cin>>book1>>book2;
    if(book1.isbn()==book2.isbn())
    {
        std::cout<<book1+book2<<std::endl;
    }
    else
    {
        std::cerr<<"Two books must be same ISBM"<<std::endl;
    }
}

void ch1_10()
{
    //建大的书店供销系统
    Sales_item total;
    if(std::cin>>total)
    {
        Sales_item tran;
        while(std::cin>>tran)
        {
            if(total.isbn()==tran.isbn())
            {
                //如果ISBN一致，更新总销售额
                total+=tran;
            }
            else
            {
                //如果ISBN不一致，则打印前一ISBN的销售情况，并更新要统计书本的信息
                std::cout<<total<<std::endl;
                total = tran;
            }
        }
        //打印最后一次处理的书籍情况
        std::cout<<total<<std::endl;
    }
    else
    {
        std::cerr<<"No book data!!!"<<std::endl;
    }
}

void testCh1()
{
        ch1_01();
        ch1_02();
        ch1_03();
        ch1_04();
        ch1_05();
        ch1_06();
        ch1_07();
        ch1_08();
        ch1_09();
        ch1_10();
}
