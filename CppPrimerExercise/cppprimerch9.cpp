#include<iostream>
#include<vector>
#include<string>
#include<deque>
#include<list>
#include<forward_list>
#include<array>
#include<stack>

#include"sales_data.h"

void ch9_2()
{

    std::list<std::string>::iterator iter;
    std::vector<int>::difference_type count;

    std::list<std::string> a={"Milton","Shakespece","Austen"};
    auto it1 = a.begin();
    std::cout<<*it1<<std::endl;
    auto it2 = a.cbegin();
    std::cout<<*it2<<std::endl;
    auto it3 = a.rbegin();
    std::cout<<*it3<<std::endl;
    auto it4 = a.crbegin();
    std::cout<<*it4<<std::endl;

    //显示指定类型
    std::list<std::string>::iterator it5 = a.begin();
    std::list<std::string>::const_iterator it6 = a.begin();

    std::vector<const char*> articles = {"a","an","the"};  //列表初始化
    std::list<std::string> authors(a);
//    std::deque<std::string> authDeq(a);
//    std::vector<std::string> words(articles);

    std::forward_list<std::string> wordForward (articles.begin(),articles.end());


    std::vector<int> ivec(10,-2);
    std::list<std::string> svec(15,"ji");
    std::forward_list<int> iforward(10);
    std::deque<std::string> sdeq(10);

    std::array<int,23> iarr;
    std::array<std::string,10> sarr;
    std::array<int,32>::size_type size;
    std::cout<<size<<std::endl;

    std::array<int,10> a1={0,1,2,3,4,5,6,7,8,9};
    for(auto x:a1)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    std::array<int,10> a2={0};//所有元素被初始化为0
    for(auto x:a2)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;
    a1=a2;
    for(auto x:a1)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    //可以使用assign对 顺序容器赋值
    std::list<std::string> names;
    std::vector<const char*> oldStyles{"blue","red"};
    names.assign(oldStyles.cbegin(),oldStyles.cend());
    names.assign(5,"hiya");
    //等价于 names.clear();names.insert(names.begin(),10,"hiya")
    for(const auto x:names)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    std::vector<std::string> svec1(10);
    std::vector<std::string> svec2(24);
    swap(svec1,svec2);
    //除 array外，swap不对任何元素进行拷贝、删除或插入操作

    std::cout<<svec1.size()<<std::endl;
    //forward_list支持max_size,empty,但不支持 size

    std::vector<int> v1={1,2,3,4,8};
    std::vector<int> v2={1,3};
    std::cout<<(v1<v2)<<std::endl;
    //容器的关系运算符，等价于 各自进行元素的逐对比较
}

void ch9_3()
{
   std::vector<std::string> para;
   std::string word;
   std::cout<<"begin push back"<<std::endl;
    while(std::cin>>word)
    {
        para.push_back(word);
    }
    std::cout<<"begin push front"<<std::endl;
    while(std::cin>>word)
    {
        para.insert(para.begin(),word);
    }
    for(const auto x:para)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;
    para.insert(para.end(),10,"haha");
    para.insert(para.begin(),{"From","long","long","ago"});

    std::vector<Sales_data> sales;
    sales.emplace(sales.begin(),"1-990090-9-99");
    sales.emplace_back(Sales_data("1-990090-9-99",23,52.5));

    if(sales.empty()==false)
    {
        auto val=*sales.begin(),val2=sales.front();
        auto last = sales.end();
        auto val3 = *(--last);
        auto &val4= sales.begin();
       // val4=Sales_data("1-2323-90-1",2,20.2);
        print(std::cout,sales[0]);
        print(std::cout,sales.at(1));
    }

    sales.pop_back();
    sales.clear();

    std::vector<int> ivec={0,1,2,3,4,5,6,7,8,9};
    auto beg=ivec.begin();
    while(beg!=ivec.end())
    {
        if(*beg%2==0)
        {
            beg=ivec.erase(beg);
        }
        else
        {
            ++beg;
        }
    }
    ivec.clear();
    ivec.erase(ivec.begin(),ivec.end());

    std::forward_list<int> flist = {0,1,2,3,4,5,6,9,7,8};
    auto prev = flist.before_begin();
    auto curr = flist.begin();
    while(curr !=flist.end())
    {
        if(*curr%2==0)
        {
            curr = flist.erase_after(prev);
        }
        else
        {
            prev = curr++;
        }
    }

    std::list<int> ilist(10,42);
    ilist.resize(15);
    ilist.resize(25,-1);
    ilist.resize(5);
}

void ch9_4()
{
    std::vector<int> ivec;
    std::cout<<"ivec size:"<<ivec.size()<<",ivec capacity:"<<ivec.capacity()<<std::endl;

    for(std::vector<int>::size_type i=0;i<20;i++)
    {
        ivec.push_back(i);
    }
    std::cout<<"ivec size:"<<ivec.size()<<",ivec capacity:"<<ivec.capacity()<<std::endl;
    ivec.resize(50);
    std::cout<<"ivec size:"<<ivec.size()<<",ivec capacity:"<<ivec.capacity()<<std::endl;
    while(ivec.size()!=50)
    {
        ivec.push_back(3);
    }
    std::cout<<"ivec size:"<<ivec.size()<<",ivec capacity:"<<ivec.capacity()<<std::endl;
    ivec.push_back(10);
    std::cout<<"ivec size:"<<ivec.size()<<",ivec capacity:"<<ivec.capacity()<<std::endl;
}

void ch9_5()
{
    const char* cp="Hello,world!";
    char notNull[]={'h','i'};
    std::string s1(cp);
    std::string s2(notNull,2);
    std::string s3(notNull);
    std::string s4(cp+2,5);
    std::string s5(s1,2,5);
    std::string s6(s1,2);

    std::cout<<"cp=\""<<cp<<"\",notNull=\""<<notNull<<"\""<<std::endl;
    std::cout<<"s1(cp)="<<s1<<",s2(notNull,2)="<<s2<<",s3(notNull)="<<s3
            <<",s4(cp+2,5)="<<s4<<",s5(s1,2,5)="<<s5<<",s6(s1,2)="<<s6<<std::endl;

    std::string s7 = s1.substr(2,5);

    std::string bookName("C++ Primer"),version=bookName;
    version.insert(bookName.size()," 5th Ed.");
    bookName.append(" 5th Ed.");

    bookName.erase(11,3);
    bookName.insert(11,"4th");
    version.replace(11,3,"4th");


    //string 搜索操作
    std::string name("AnnaBelle");
    auto pos1= name.find("Anna");
    std::string lowName("annabelle");
    pos1 = lowName.find("Anna");

    std::string numbers("0123456789");
    name="rd21saws2";
    pos1 = name.find_first_of(numbers);
    pos1 = name.find_first_not_of(numbers);

    pos1=0;

    while((pos1=name.find_first_of(numbers,pos1))<name.size())
    {
        std::cout<<"find number at index "<<pos1<<",element is "<<name[pos1]<<std::endl;
        ++pos1;
    }
    auto firstPos = name.find('2');
    auto lastPos = name.rfind('2');

    int i=40;
    std::string numberStr = std::to_string(i);
    double numberD= std::stod(numberStr);

    std::string piStr = "pi=3.14159";
    //piStr = piStr.substr(piStr.find_first_of(""))
    double pi = std::stod(piStr.substr(piStr.find_first_of("+-.123456789")));
    std::cout<<"find number from \"pi=3.14159\":"<<pi<<std::endl;
}

void ch9_6()
{
    std::stack<int> intStack;
    for(size_t i=0;i<12;++i)
    {
        intStack.push(i+2);
    }
    while(!intStack.empty())
    {
        int val = intStack.top();
        std::cout<<val<<"\t";
        intStack.pop();
    }
    std::cout<<std::endl;
}
void testCh9()
{
    std::cout<<"Start test chapter 9"<<std::endl;
    //ch9_2();
    //ch9_3();
    ch9_4();
    ch9_5();
    ch9_6();
}
