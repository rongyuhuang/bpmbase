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

void testCh7()
{
    std::cout<<"Start test chapter 7"<<std::endl;
    //ch7_1();
    ch7_3();
}
