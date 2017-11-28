
#include<iostream>
#include<string>
#include<vector>

void ch4_1()
{
    std::cout<<"5+10*20/4="<<5+10*20/4<<std::endl;
    std::cout<<"6+3*4/2+4="<<6+3*4/2+4<<std::endl;
    std::cout<<"(6+3)*4/2+4="<<(6+3)*4/2+4<<std::endl;
    std::cout<<"6+3*4/(2+4)="<<6+3*4/(2+4)<<std::endl;

    //优先级与结合律有何影响
    int ia[]={0,2,3,6,8};
    int last = *(ia+4);
    std::cout<<"*(ia+4)="<<last<<std::endl;
    last = *ia+4;
    std::cout<<"*ia+4="<<last<<std::endl;

    //对于没有指定执行顺序的运算符来说，如果表达式指向并修改了同一个对象，将会引发错误并产生未定义的行为。如下
    int i=0;
    std::cout<<i<<"\t"<<++i<<std::endl;
}

void ch4_2()
{
    //算术运算符
    int i=1024;
    int k= -i;
    bool b=true;
    bool b2 = -b;
    std::cout<<"bool b =true;bool b2=-b="<<b2<<std::endl;

    int ival =42;
    double dval=3.14;
    std::cout<<"42%12="<<ival%12<<std::endl;
    //std::cout<<"42%3.14="<<ival%dval<<std::endl; //%仅针对整数
    //m%(-n) 等于 m%n,(-m)%n 等于-(m%n)
    int m=13,n=5;
    std::cout<<"13%(-5)="<<m%(-n)<<std::endl;
    std::cout<<"(-13)%5="<<(-m)%n<<std::endl;
}

void ch4_345()
{
    //逻辑和关系运算符
    std::vector<std::string> text{"Tao is a coder","learning C++ 11."};
    for(const auto &s:text)
    {
        std::cout<<s;
        if(s.empty() || s[s.size()-1]=='.')
        {
            std::cout<<std::endl;
        }
        else
        {
            std::cout<<",";
        }
    }
    //赋值运算符
    //赋值运算符的左侧运算对象必须是一个可修改的左值
    int i=0,j=0,k=0;//初始化
    const int ci = i;//初始化
    //赋值运算满足右结合律


    //递增递减运算符,除非必要，建议使用前置版本
    i=0;
    j=0;
    j=++i;  //前置版本得到递增之后的值
    std::cout<<"i=0,j=0;j=++i,j="<<j<<",i="<<i<<std::endl;
    j=i++;  //后置版本得到递增之前的值
    std::cout<<"j=i++,j="<<j<<",i="<<i<<std::endl;

    std::vector<int> ivec{1,2,3,454};
    auto beg = ivec.begin();
    while(beg !=ivec.end() && *beg>=0)
    {
        std::cout<<*beg++<<std::endl;
    }
}

void ch4_6789()
{
    //4.6 成员访问运算符
    std::string s1="a string",*p=&s1;
    auto n= s1.size();
    std::cout<<n<<std::endl;
    n= p->size();    //等价于 (*p).size()
    std::cout<<n<<std::endl;

    //4.7 条件运算符
    int grade;
    std::cout<<" input grade"<<std::endl;
    std::cin>>grade;
    auto gradeResult = (grade>90) ? "High pass!" :( grade>=60 ? "pass" :"fail");
    std::cout<<gradeResult<<std::endl;

    //4.8 位运算符
    grade =40;
    std::cout<<(40>>3)<<std::endl;

    //4.9 sizeof运算符
    std::string sTxt{"CppPrimer 5th"},*ptxt=&sTxt;
    std::cout<<"sizeof(sTxt)="<<sizeof(sTxt)<<std::endl;
    std::cout<<"sizeof(ptxt)="<<sizeof(ptxt)<<std::endl;

}

void ch4_10()
{
    //逗号运算符
    std::vector<int> ivec{1,2,3,454};
    std::vector<int>::size_type cnt = ivec.size();
    for(std::vector<int>::size_type idx=0;idx!=ivec.size();++idx,--cnt)
    {
        ivec[idx]=cnt;
    }
    while(beg !=ivec.end() && *beg>=0)
    {
        std::cout<<*beg++<<std::endl;
    }
}
void ch4_11()
{
    //类型转换
}
void testCh4()
{
    std::cout<<"Start test chapter 4"<<std::endl;
    ch4_1();
    ch4_2();
    ch4_345();
    ch4_6789();
}
