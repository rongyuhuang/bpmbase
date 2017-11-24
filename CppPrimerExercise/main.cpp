#include <iostream>
#include<stdlib.h>

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
int main(int argc, char *argv[])
{
    ch1_01();
    ch1_02();
    ch1_03();
    ch1_04();
    ch1_05();
    system("pause");
    return 0;
}
