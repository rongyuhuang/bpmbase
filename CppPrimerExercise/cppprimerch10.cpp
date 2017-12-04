#include<iostream>
#include<vector>
#include<stack>
#include<list>

#include<iterator> //back_inserter
#include<algorithm>
#include<numeric>
#include<functional>



void ch10_1()
{
    std::vector<int> ivec={12,12,11,23,43,42,67};
    int val=42;
    auto result= find(ivec.cbegin(),ivec.cend(),val) ;
    std::cout<<"The value "<<val
            <<(result==ivec.end()? " is not present":" is present.")<<std::endl;

    int ia[]={23,12,210,23,109,80,32,43};
    val=109;
    int* iaResult = std::find(std::begin(ia),std::end(ia),val);
    std::cout<<"The value "<<val
            <<(iaResult==std::end(ia)? " is not present":" is present.")<<std::endl;

    //迭代器令算法不依赖于容器，但算法依赖于元素类型的操作

}

void elimDups(std::vector<std::string>& words)
{
    std::sort(words.begin(),words.end());
    auto end_unique = std::unique(words.begin(),words.end());
    words.erase(end_unique,words.end());
}

void ch10_2()
{
    //只读算法
    std::vector<int> ivec={12,23,12,43,546,64,342};
    int isum=std::accumulate(ivec.cbegin(),ivec.cend(),0);
    std::cout<<isum<<std::endl;

    std::vector<std::string> svec={"Hi,","C++ Primer ","5th Ed."};
    auto sSum = std::accumulate(svec.cbegin(),svec.cend(),std::string(""));
    std::cout<<sSum<<std::endl;

    std::vector<std::string> svec2={"Hi,","C++ Primer"};
    //auto eqResult = equal(svec.cbegin(),svec.cend(),svec2.cbegin());
    //std::cout<<eqResult<<std::endl;
    fill(ivec.begin(),ivec.end(),0);
    std::cout<<"fill 0 to ivec"<<std::endl;
    for(const auto x:ivec)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    fill_n(ivec.begin(),ivec.size()/2,20);
    std::cout<<"fill 20 to half of ivec"<<std::endl;
    for(const auto x:ivec)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    //fill_n 注意不越界

    ivec.clear();
    auto it = std::back_inserter(ivec);
    *it=45;
    fill_n(std::back_inserter(ivec),10,0);
    std::cout<<"clear ivec,then using back_inserter fill 45,(10,0) to ivec"<<std::endl;
    for(const auto x:ivec)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    int a1[]={0,1,2,3,4,5,6,7,8,9};
    int a2[sizeof(a1)/sizeof(*a1)];
    //目的序列至少要包含与输入序列一样多的元素
    auto ret = std::copy(std::begin(a1),std::end(a1),a2);
    //ret指向a2尾部

    std::replace(ivec.begin(),ivec.end(),45,20);

    std::vector<std::string> words ={"the","quick","red","fox","jumps","over","the","red","slow","read","turtle"};
    for(const auto &x:words)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;
    elimDups(words);
    for(const auto &x:words)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;
}

//bool isShorter(const std::string & s1,const std::string& s2)
//{
//    return s1.size()<s2.size();
//}

void biggies(std::vector<std::string>& words,std::vector<std::string>::size_type sz)
{
    elimDups(words);
    std::stable_sort(words.begin(),words.end(),isShorter);
    auto wc = std::find_if(words.begin(),words.end(),
                           [sz](const std::string&s){return s.size()>sz;});
    //计算 满足 size>=sz的元素数量
    auto count = words.end()-wc;
    std::cout<<"There are "<<count<<" words\' size >="<<sz<<" in words"<<std::endl;
    std::for_each(wc,words.end(),
                  [](const std::string& s){std::cout<<s<<"\t";});
    std::cout<<std::endl;
}

void biggies(std::vector<std::string>& words,std::vector<std::string>::size_type sz,
             std::ostream& os,char c=' ')
{
    elimDups(words);
    std::stable_sort(words.begin(),words.end(),isShorter);
    auto wc = std::find_if(words.begin(),words.end(),
                           [sz](const std::string&s){return s.size()>sz;});
    //计算 满足 size>=sz的元素数量
    auto count = words.end()-wc;
    std::cout<<"There are "<<count<<" words\' size >="<<sz<<" in words"<<std::endl;
    //隐式捕获和显式捕获混用时捕获列表中的第一个元素必须是一个&或者=（此符号指定了默认捕获方式为引用或值）
    std::for_each(wc,words.end(),
                  [&,c](const std::string& s){os<<s<<c;});

//    std::for_each(wc,words.end(),
//                  [=,&os]{os<<s<<c;});

    std::cout<<std::endl;
}

void fcn3()
{
    size_t v1=42;
    //f可以改变他所捕获的变量的值
    auto f=[v1]()mutable{return ++v1;};
    v1=0;
    auto j=f();
    std::cout<<j<<std::endl;

    auto f1 =[&j]{return ++j;};
    j=0;
    j=f1();
    std::cout<<j<<std::endl;
}

bool check_size(const std::string &s,std::string::size_type sz)
{
    return s.size()>=sz;
}
void ch10_3()
{
    std::vector<std::string> words ={"the","quick","red","fox","jumps","over","the","red","slow","read","turtle"};
    for(const auto &x:words)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;
    elimDups(words);
    std::stable_sort(words.begin(),words.end(),isShorter);
    for(const auto &x:words)
    {
        std::cout<<x<<"\t";
    }
    std::cout<<std::endl;

    auto f=[]{return 42;};
    std::cout<<f()<<std::endl;

    int sz=4;
    auto f1=[sz](const std::string& s){return s.size()>=sz;};
    auto wc = std::find_if(words.begin(),words.end(),f1);
    auto count = words.end()-wc;
    std::cout<<count<<std::endl;
    std::for_each(wc,words.end(),[](const std::string &s){std::cout<<s<<"\t";});
    std::cout<<std::endl;

    std::vector<std::string> wordes ={"the","quick","red","fox","jumps","over","the","red","slow","read","turtle"};
    biggies(wordes,sz);

    //值捕获

    size_t v1=42;
    auto fval=[v1]{return v1;};
    v1=0;
    std::cout<<fval()<<std::endl;

    size_t v2=25;
    auto fref=[&v2]{return v2;};
    v2=0;
    std::cout<<fref()<<std::endl;

    //可变 lambda
    fcn3();

    //参数绑定
    auto check1 = bind(check_size,std::placeholders::_1,6);
    std::string shi("Hello");
    bool b1 = check1(shi);
    std::cout<<b1<<std::endl;
}


void ch10_4()
{
    //插入迭代器
    std::list<int> lst ={1,2,3,4,5};
    std::list<int> lst2,lst3;
    copy(lst.cbegin(),lst.cend(),std::front_inserter(lst2));
    //copy(lst.crend(),lst.crbegin(),std::front_inserter(lst3));

    //iostream 迭代器

    std::istream_iterator<int> int_it(std::cin);
    std::istream_iterator<int> int_eof;
    std::ifstream in("afile");
    std::istream_iterator<std::string> str_it(in);

    std::vector<int> ivec;
    while(int_it!=int_eof)
    {
        ivec.push_back(*int_it++);
    }
    std::cout<<std::accumulate(int_it,int_eof,0)<<std::endl;

    std::ostream_iterator<int> out_iter(std::cout," ");
    for(auto e:ivec)
    {
        *out_iter++=e;
    }
    std::cout<<std::endl;

    copy(ivec.begin(),ivec.end(),out_iter);
    std::cout<<std::endl;

    //反向迭代器
    ivec.clear();
    ivec={0,1,2,3,4,4,5,6,7,8,9};
    for(auto r_iter=ivec.crbegin();r_iter!=ivec.crend();++r_iter)
    {
        std::cout<<*r_iter<<"\t";
    }
    std::cout<<std::endl;


}

void ch10_5()
{
    //泛型算法结构

}
void testCh10()
{
    std::cout<<"Start test chapter 10"<<std::endl;
//    ch10_1();
//    ch10_2();
    //ch10_3();
    ch10_4();
}
