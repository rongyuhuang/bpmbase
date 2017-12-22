/*chapter 4 使用C++11解决内存泄露的问题
 * C++没有垃圾回收机制，需要自己管理内存
 * 智能指针是存储指向动态分配堆对象指针的类，用于生存期控制，能够确保在离开指针所在作用域时，自动正确的销毁动态分配的对象，防止内存泄露
*/
#include<iostream>
#include<memory>
#include<string>
#include<map>
#include<functional>
#include<algorithm>
#include<array>
void deleteIntPtr(int* p)
{
    std::cout<<"delete a int*"<<std::endl;
    delete p;
}

template<typename T>
std::shared_ptr<T> make_shared_array(std::size_t size)
{
    return std::shared_ptr<T>(new T[size],std::default_delete<T[]>());
}
void ch4_1()
{
    /*shared_ptr使用引用计数，每一个shared_ptr的拷贝都指向相同的内存，在最后一个shared_ptr析构的时候，内存才会被释放
     * 不能用一个原始指针初始化多个shared_ptr
     * 不要再函数实参中创建shared_ptr
     * 通过shared_from_this返回this指针，即让目标类派生std::enable_shared_from_this<T>，避免重复析构
     * 要避免循环引用
    */
    //1).初始化
    std::shared_ptr<int> p1(new int(2));
    std::shared_ptr<int> p2 = p1;
    std::shared_ptr<int> p3;
    p3.reset(new int(5));
    if(p3)
    {
        std::cout<<"ptr is not null"<<std::endl;
    }

    //2).获取原始指针
    int* srcp = p3.get();
    std::cout<<*srcp<<std::endl;

    //3).指定删除器: std::shared_ptr的默认删除器不支持数组对象
    {
        std::shared_ptr<int> p4(new int,deleteIntPtr);
    }
    {
        std::shared_ptr<int> p5 (new int[10],[](int*p){ std::cout<<"delete a point of array"<<std::endl;delete[]p;});
    }
    {
     std::shared_ptr<int> p6 = make_shared_array<int>(100);
    }
}

//支持普通指针
template<class T,class... Args>inline
typename std::enable_if<!std::is_array<T>::value,std::unique_ptr<T>>::type make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
//支持动态数组
template<class T>inline
typename std::enable_if<std::is_array<T>::value && std::extent<T>::value==0,std::unique_ptr<T>>::type make_unique(std::size_t size)
{
    typedef typename remove_extent<T>::type U;
    return std::unique_ptr<T>(new U[size]());
}
//过滤掉定长数组的情况
template<class T,class... Args>
typename std::enable_if<std::extent<T>::value!=0,void>::type make_unique(Args&&...)=delete;

class MyDeleter{
public:
    void operator()(int *p)
    {
        std::cout<<"deleter"<<std::endl;
        delete p;
    }
};
void ch4_2()
{
    /*unique_ptr独占的智能指针，不允许其它的智能指针共享器内部的指针，不允许通过赋值将一个unique_ptr赋值给另一个unique_ptr;
     * 可通过函数返回值返回给其他的unique_ptr,或通过std::move()转移到其他unique_ptr
     *
    */
    std::unique_ptr<int> p1(new int);
    std::unique_ptr<int> p2= std::move(p1);

    //unique_ptr可以指向一个数组
    std::unique_ptr<int[]> p3 (new int[10]);
    p3[3]=3;

    //unique_ptr指定删除器的时候需要确定删除器的类型
    std::shared_ptr<int> p4 (new int(1),[](int* p){delete p;});
    std::unique_ptr<int,void(*)(int*)> p5(new int(1),[](int* p){delete p;});

    //如果需要支持捕获变量的lambda,需要通过 std::function转换为函数指针
    std::unique_ptr<int,std::function<void(int*)>> p6(new int(6),[&](int*p){delete p;});
    //自定义删除器
    std::unique_ptr<int,MyDeleter> p7(new int(7));
}


std::weak_ptr<int> gwp1;
void fcn3()
{
    if(gwp1.expired())
    {
        std::cout<<"gwp1 is expired。"<<std::endl;
    }
    else
    {
        std::cout<<*gwp1.lock()<<std::endl;
    }
}

class A3;
class B3;
class A3:public std::enable_shared_from_this<A3>
{
public:

    std::shared_ptr<A3> GetSelf()
    {
        return shared_from_this();
    }
    ~A3()
    {
        std::cout<<"A3 is deleted"<<std::endl;
    }
    std::shared_ptr<B3> spb;
};

class B3{
public:
    std::weak_ptr<A3> wpa;
    ~B3()
    {
        std::cout<<"B3 is deleted."<<std::endl;
    }
};
void ch4_3()
{
    /*weak_ptr弱引用的智能指针
    */
    //use_count()观测资源的引用计数
    std::shared_ptr<int> sp1(new int(23));
    std::weak_ptr<int> wp1(sp1);
    std::cout<<wp1.use_count()<<std::endl;
    //expired()观察的资源是否已释放
    if(wp1.expired())
    {
        std::cout<<"weak_ptr is invalid"    <<std::endl;
    }
    else
    {
        std::cout<<"weak_ptr is still valid"<<std::endl;
    }
    {
        auto sp2=std::make_shared<int>(2);
        gwp1 = sp2;
        fcn3();
    }
    fcn3();
    {
        std::shared_ptr<A3> spa1(new A3);
        std::shared_ptr<A3> spa2= spa1->GetSelf();

        std::shared_ptr<B3> spb1 (new B3);
        spa1->spb = spb1;
        spb1->wpa = spa1;
    }
}

void ch4_4()
{
    //可以用宏定义和unique_ptr来管理第三方库分配的内存
}
void testCh4()
{
    std::cout<<"Start to test chapter 4"<<std::endl;
    ch4_1();
    ch4_2();
    ch4_3();
    ch4_4();
}
