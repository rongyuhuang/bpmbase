#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<functional>
#include<memory>

#include"quote.h"

void ch15_12()
{
    Quote q1;
    BulkQuote bq1;
    Quote* pq1 = &q1;
    pq1 = &bq1;
    Quote& rq1 = bq1;

    Quote q2(std::string("125-1-23-9"),25);
    std::cout<<q2.net_price(4)<<std::endl;
    print_total(std::cout,q2,4);
    BulkQuote bq2(std::string("125-1-23-9"),25,10,0.8);
    std::cout<<bq2.net_price(15)<<std::endl;

    print_total(std::cout,bq2,15);
    /*1.每个类控制自己的成员初始化过程
     * 2.每个类负责定义各自的接口
     * 3.派生类的声明，不包含它的派生列表
     * 4.一个类想做基类，必须已经定义
     * 5.组织一个类派生，可以在类名后跟 final
     * 6.不存在从基类向派生类的隐式类型转换，在对象之间不存在类型转换
    */

}

void ch15_34()
{
    /*虚函数
     * 1.对虚函数的调用可能在运行时才被解析
     * 2.动态绑定只有当我们通过指针或引用调用虚函数才会发生
     * 3.如果虚函数使用默认实参，则基类和派生中定义的默认实参最好一致
     * 4.只有成员函数或者友元中的代码才需要使用作用域运算符来回避虚函数的机制
     * 5.
    */

    /*抽象基类
     * 1.在函数体的位置书写=0就可以将一个虚函数说明为纯虚函数，可为纯虚函数提供定义但必须在类的外部；
     * 2.含有纯虚函数的泪是抽象基类；
     * 3.派生类构造函数值初始化它的直接基类
    */
    //QuoteDisc qd1;不能创建抽象基类的对象
    BulkQuote bq1;

}

class Pal;
class Base{
    friend class Pal;
public:
    int pub_mem=0;

protected:
    int prot_mem=1;

private:
    int pri_mem=2;
public:
     int fcn();

};

class Sneaky:public Base{
    friend void clobber(Sneaky&);
    friend void clobber(Base&);
    int j; //默认为private
};

struct PubDev:public Base{
public:
    using Base::prot_mem;
    //using Base::pri_mem;  //派生类只能为那些他可以访问的名字提供using声明
    int f(){return prot_mem;}

    //private成员对于派生类来说是不可访问的
    //int g(){return pri_mem;}
};

struct PriDev:private Base{
    int f1()const{return prot_mem;}

};

class Pal{
public:
    int f(Base b){return b.prot_mem;} //Pal是Base的友元
    //int f2(Sneaky s){return s.j;} //友元不具有传递性
    int f3(Sneaky s){return s.prot_mem;} //Pal是Base的友元，对基类的访问权限由基类本身控制，即便对于派生类的基类部分也是如此

};


class D2:public Pal{
public :
    int mem(Base b)
    {
        //return b.prot_mem; //友元关系不能继承
        return b.pub_mem;
    }
};
void clobber(Sneaky& s)
{
    s.j = s.prot_mem =0;
}
void clobber(Base& b)
{
//    b.prot_mem=0; // 不能访问Base的protected成员
    b.pub_mem=0;
}

class D1:public Base{
public:

    int fcn(int); //隐藏了基类的 fcn(),形参列表与Base中的fcn不一致
    virtual void f2(); //新的虚函数
};

class DD1:public D1{
public:
    int fcn(int); //非虚函数，隐藏了 D1的fcn(int)
    int fcn() ; //覆盖了Base的fcn()
    void f2() ; //覆盖了D1的f2
};

void ch15_56()
{
    /*受保护的成员
     * 1.受保护的成员对于类的用户来说是不可访问的
     * 2.受保护的成员对于派生类的成员和友元来说是可访问的
     * 3.派生类的成员或友元只能通过派生类对象来访问基类的受保护成员，
     * 4.派生类向基类的转换 是否可访问由使用该转换的代码决定，同时派生类的派生访问说明符也会有影响
     * 5.基类应该将其接口成员声明为共有；同时将属于其实现的部分分成两组：一组可供派生类访问，另一组只能由基类及基类的友元访问。
     * 对于前者应该声明为受保护的这样派生类就能在实现自己的功能时使用基类的这些操作和数据，后者应该声明为私有的
    */

    PubDev pd1;
    PriDev pd2;
    std::cout<<pd1.f()<<std::endl;
    std::cout<<pd2.f1()<<std::endl;
    std::cout<<pd1.pub_mem<<std::endl;
    //std::cout<<pd2.pub_mem<<std::endl; //继承自Base的成员在派生类中都是 private

    /*友元与继承
     * 1.友元关系不能传递、也不能继承
     * 2.每个类负责控制各自成员的访问权限
     * 3.可使用using声明来改变派生类继承的某个名字的访问级别：派生类只能为那些他可以访问的名字提供using声明
     * 4.默认的继承保护级别：类为private,struct为public
    */

    /*继承中的类作用域
     * 1.每个类定义自己的作用域，派生类的作用域嵌套在其基类的作用域之内
     * 2.派生类的成员将隐藏同名的基类成员
     * 3.通过作用于运算符来使用隐藏的成员
     * 4.名字查找先于类型检查：声明在内层作用域的函数并不会重载声明在外层作用域的函数：
     * 如果派生类的成员与基类的某个成员同名，则派生类将在其作用域内隐藏该基类成员，及时两者形参列表不一致。
    */

    /*虚函数与作用域
     * 1.积累与派生类中的虚函数必须有相同的形参列表：如果不同，则无法通过基类的引用或者指针调用派生类的虚函数
     *
    */

//    Base bobj;
//    D1 d1Obj;
//    DD1 dd1Obj;
//    Base *pb1 = &bobj,*pb2=&d1Obj,*pb3 = &dd1Obj;
//    pb1->fcn(); // 虚调用，将在运行时调用Base::fcn()
//    pb2->fcn(); // 虚调用，将在运行时调用Base::fcn()
//    pb3->fcn(); // 虚调用，将在运行时调用DD1::fcn()

//    D1 *pd1=&d1Obj,*pd2=&dd1Obj;
//    //dd1Obj.fcn()
//    pd1->f2(); //虚调用，将在运行时调用D1:f2()
//    pd2->f2(); //虚调用，将在运行时调用DD1:f2()
}

void ch15_7()
{
    /*虚析构函数
     * 1.虚析构函数将阻止该类合成移动操作
     * 2.如果基类中默认构造函数、拷贝构造函数、拷贝赋值运算符或析构函数的函数是不可访问或者被删除的，那么其派生类也是如此
     * 2.如果基类中有一个不可访问或者删除掉的析构函数，则派生类中合成的默认和拷贝构造函数将被删，因为编译器无法销毁派生类对象的基类部分
     * 3.基类定义了虚析构函数，派生想执行移动操作，则首先在基类中进行定义
     * 4.当派生类定义了拷贝或者移动操作时，该操作负责拷贝或移动包括基类在内的整个对象
     * 5.默认情况下，基类默认构造函数初始化派生类对象的基类部分：
     * 如想拷贝或移动基类部分，须在派生类的构造函数初始值列表中显示的使用基类的拷贝或移动构造函数
     * 6.对象的销毁顺序与创建的顺序相反
     * 7.在构造函数或西沟函数调用某个虚函数，应该执行与构造函数或析构函数所属类型相对应的虚函数版本
    */
    Quote* q1 =  new Quote;
    delete q1;
    q1= new BulkQuote;
    delete q1; //动态调用BulkQuote的析构函数
}

void ch15_8()
{
    Basket b1;
    b1.addItem(std::make_shared<Quote>("123",45));
    b1.addItem(std::make_shared<BulkQuote>("334",12,4,0.2));
    b1.totalReceipt(std::cout);

    Quote q1("123",45);
    b1.addItem(q1);
    BulkQuote bq1("1112",23,34,0.5);
    b1.addItem(std::move(bq1));
    b1.totalReceipt(std::cout);
}
void testCh15()
{
    std::cout<<"Start to test chapter 15"<<std::endl;
//    ch15_12();
//    ch15_34();
    //ch15_56();
    ch15_8();
}
