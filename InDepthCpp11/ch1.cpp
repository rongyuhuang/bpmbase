#include<iostream>
#include<functional>
#include<set>
#include<vector>
#include<string>

void ch1_1()
{
    std::cout<<"1.1类型推导"<<std::endl;
    /*使用auto声明的变量必须马上初始化，以让编译器推导出它的实际类型
     * 1.当不声明为指针或引用时，auto的推导结果和初始化表达式抛弃引用和cv限定符后类型一致
     * 2.当生命为指针或引用时，auto的推导结果将保持初始化表达式的cv属性
    */
    auto x=7; //x推导为 int
    auto pi = new auto(1); //pi推导为 int*
    const auto *v=&x,u=6;//v推导为 const int*,u推导为const int
    static auto y = 0.0; //y推导为double
    //auto int r;
    //auto s;

    x=0;
    auto *a=&x; //a推导为 int*,auto为int
    auto b= &x;//b推导为int*,auto 为int*
    auto&c = x; //c推导为int&,auto 为int
    auto d = c; //d推导为int,auto为int
    const auto e = x;// e推导为const int,auto 为int
    auto f =e; //f推导为int
    const auto& g = x; //g推导为const int&,auto为int
    auto& h=g; // h推导为const int&,auto为const int

}
void testCh1()
{
    std::cout<<"Start test chapter 1"<<std::endl;
    ch1_1();
}
