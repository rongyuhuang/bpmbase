#include<iostream>
#include<string>
#include<cctype>
#include<vector>

#include"sales_item.h"

void ch3_1()
{
    using std::cin;
    int i;
    cin>>i;
    std::cout<<"i="<<i<<std::endl;

    std::cout<<"Enter tow numbers:"<<std::endl;
    int v1,v2;
    std::cin>>v1>>v2;
    std::cout<<"The sum of "<<v1<<" and "<<v2<<" is "<<v1+v2<<std::endl;
}

void ch3_2()
{
    std::cout<<"标准库类型 string"<<std::endl;
    std::string s1;
    std::string s2=s1;
    std::string s3 ="C++";
    std::string s4("Primer");
    std::string s5(5,'+');
    std::cout<<s3<<".empty() is "<<s3.empty()<<std::endl;

    std::string strHi ="Hello",strWorld="World.";
    s4= strHi+",";
    //s5 = "Hello"+","; // 错误，字面值不能直接相加

    std::string s("Learning C++ @2017");
    int numCnt=0,charCnt;

    for(auto c:s)
    {
        if(isdigit(c))
        {
            numCnt++;
            continue;
        }
        if(isalpha(c))
        {
            charCnt++;
            continue;
        }
    }
    std::cout<<s<<" has "<<numCnt<<" digit,and has "<<charCnt<<" char"<<std::endl;

    const std::string hexDigits ="0123456789ABCDEF";
    std::cout<<"Enter a series of number between 0 and 15,separated by space,hit ENTER to finish"<<std::endl;
    std::string hexStr;
    std::string::size_type n;
    while(std::cin>>n)
    {
        if(n<hexDigits.size())
        {
            hexStr += hexDigits[n];
        }
    }
    std::cout<<"Your hex number is"<<hexStr<<std::endl;

}

void ch3_3()
{
    //vector
    std::vector<int> ivec;
    std::vector<std::string> v1{"a","b","c"};
    std::vector<std::string> v2(10,"m");

    std::vector<int> intVec(10,0);
    std::cout<<"in the beginng ,0 is in intVect\r\n";
    for(auto i:intVec)
    {
        std::cout<<i<<"\t";
    }
    std::cout<<std::endl;
    for(int i=0;i<intVec.size();i++)
    {
        intVec.push_back(i);
    }
    std::cout<<"update element in intVec\r\n";
    for(auto i :intVec)
    {
        std::cout<<i<<"\t";
    }
    std::cout<<std::endl;

    std::cout<<"stat scores(11 classes)"<<std::endl;
    std::vector<unsigned> scores{11,0};
    unsigned grade;
    while(std::cin>>grade)
    {
        if(grade<=100)
        {
            scores[grade/10]++;
        }
    }
    for(auto c :scores)
    {
        std::cout<<c<<"\t";
    }
    std::cout<<std::endl;
}

void ch3_4()
{
    //iterator
    //首字母大写
    std::string s("someone like you");
    if(s.begin()!=s.end())
    {
        auto it = s.begin();
        *it = toupper(*it);
    }
    std::cout<<s<<std::endl;

    for(auto it=s.begin();it!=s.end()&&!isspace(*it);it++)
    {
        *it=toupper(*it);
    }
    std::cout<<s<<std::endl;

    //二分法
    std::vector<int> text{12,34,54,78,120,456,1024,5630,8523,52789};
    int des=345;
    auto beg = text.begin();
    auto end = text.end();
    auto mid = beg+(end-beg)/2;
    while(mid!=end &&*mid !=des)
    {
        if(des<*mid)
        {
            end=mid;
        }
        else
        {
            beg=mid+1;
        }
        mid = beg+(end-beg)/2;
    }
    if(mid==end)
    {
        std::cout<<"find "<<des<<" fail."<<std::endl;
    }
    else
    {
        std::cout<<"find "<<*mid<<std::endl;
    }
}

void ch3_5()
{
    //数组
    const unsigned sz=3;
    int iarr[sz]={1,3,4};

    char str[]={'c','+','+'};

    unsigned scores[11]={};
    unsigned grade;
    while(std::cin>>grade)
    {
        if(grade<=100)
        {
            scores[grade/10]++;
        }
    }
    for(auto x:scores)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

}

void ch3_6()
{
    //多维数组
    int arr[10][4]={0};

    int ia[3][4]={
        {0,1,2,3},
        {4,5,6,7},
        {8,9,10,11}
    };

    constexpr size_t rowCnt=2,colCnt=3;
    int iarr[rowCnt][colCnt];
    for(size_t i=0;i<rowCnt;i++)
    {
        for(size_t j=0;j<colCnt;j++)
        {
            iarr[i][j]=i*2+j*3;
        }
    }

    for(auto& row:iarr)
    {
        for(auto& col:row)
        {
            std::cout<<col<<"\t";
        }
        std::cout<<std::endl;
    }

    for(auto p=iarr;p!=iarr+1;p++)
    {
        for(auto q=*p;q!=*p+2;q++)
        {
            std::cout<<*q<<"\t";
        }
        std::cout<<std::endl;
    }

    //    for(auto beg=begin(iarr);beg!=end(iarr);beg++)
    //    {
    //        for(auto colBeg=begin(*beg);colBeg!=end(*beg);colBeg++)
    //        {
    //            std::cout<<*colBeg<<"\t";
    //        }
    //        std::cout<<std::endl;
    //    }
}
void testCh3()
{
    std::cout<<"Start test chapter 3"<<std::endl;

    ch3_1();
    ch3_2();
    ch3_3();
    ch3_4();
    ch3_5();
    ch3_6();
}
