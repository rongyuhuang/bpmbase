/*C++11多线程开发
*/
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<set>
#include<list>

#include<functional>
#include<thread>
#include<condition_variable>
#include<mutex>
#include<atomic>
#include<future>
#include<utility>
#include<chrono>

void fcn1(int x,const std::string& msg)
{
    //https://item.jd.com/17874261795.html#none
    std::cout<<x<<":"<<msg<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout<<"this thread has sleeped 3 s"<<std::endl;
}
void ch5_1()
{
    /*1.线程的创建
     * join()阻塞线程，detach()分离线程
     * 线程不能被复制，但能被移动
    */
    std::thread t1(fcn1,1,"fcn1");
    //std::thread t2(std::move(t1)); //t1被移动之后不再代表任何线程
    std::cout<<t1.get_id()<<std::endl; //获取当前线程ID
    std::cout<<std::thread::hardware_concurrency()<<std::endl;//获得计算机核数
    t1.join();
    //t2.join();
    //t1.detach();
}


std::mutex g_lock;
void fcn2()
{
    g_lock.lock();
    std::cout<<"Entered thread "<<std::this_thread::get_id()<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<<"Leaved thread "<<std::this_thread::get_id()<<std::endl;
    g_lock.unlock();
}

void fcn2_lock_guard()
{
    std::lock_guard<std::mutex> locker(g_lock);
    std::cout<<"Entered thread "<<std::this_thread::get_id()<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<<"Leaved thread "<<std::this_thread::get_id()<<std::endl;
}

struct Complex{
    std::recursive_mutex mutex;
    int i;
    Complex():i(0){}

    void mul(int x)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);
        i*=x;
    }
    void div(int x)
    {
        if(x==0)
        {
            std::cout<<"x can\'t be 0"<<std::endl;
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mutex);
        i/=x;
    }

    void both(int x,int y)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);
        mul(x);
        div(y);
    }
};

std::timed_mutex t_mutex;

void work(){
    std::chrono::milliseconds timeout(150);

    while(true)
    {
        if(t_mutex.try_lock_for(timeout))
        {
            std::cout<<std::this_thread::get_id()<< " do work with mutex"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            t_mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        else
        {
            std::cout<<std::this_thread::get_id()<<"do work without mutex"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
void ch5_2()
{
    /*互斥量是一种同步原语，是一种线程同步的手段，用来保护多线程同时访问的共享数据
     * C++11提供如下4种语义的互斥量
     * std::mutex
     * std::timed_mutex
     * std::recursive_mutex
     * std::recursive_timed_mutex
    */
    std::thread t1(fcn2);
    std::thread t2(fcn2);
    std::thread t3(fcn2_lock_guard);
    t1.join();
    t2.join();
    t3.join();

    Complex c1;
    c1.both(12,5);

//    std::thread t5(work);
//    std::thread t6(work);
//    t5.join();
//    t6.join();
}


template <typename T>
class SyncQueue{
private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable_any m_notEmpty;
    std::condition_variable_any m_notFull;
    int m_maxSize;
private:
    bool isFull()const
    {
        return m_queue.size()==m_maxSize;
    }
    bool isEmpty()const
    {
        return m_queue.empty();
    }

public:
    SyncQueue(int maxSize):m_maxSize(maxSize){}

    void Put(const T& x)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        while(isFull())
        {
            std::cout<<"缓冲区已满，请等待..."<<std::endl;
            m_notFull.wait(m_mutex);
        }
        m_queue.push_back(x);
        m_notEmpty.notify_one();
    }

    void Take(T& x)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        while(isEmpty())
        {
            std::cout<<"缓冲区已空，请等待..."<<std::endl;
            m_notEmpty.wait(m_mutex);
        }
        x= m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
    }

    bool Empty()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    bool Full()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size() == m_maxSize;
    }

    std::size_t Size()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    int Count()
    {
       return m_queue.size();
    }
};

template <typename T>
class SimpleSyncQueue{
public:
    SimpleSyncQueue(){}
    void Put(const T& x)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_back(x);
        m_notEmpty.notify_one();
    }

    void Take(T& x)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_notEmpty.wait(lock,[this]{return !m_queue.empty();});
        x=m_queue.front();
        m_queue.pop_front();
    }

    bool Empty()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    std::size_t Size()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
};
void ch5_3()
{
    SyncQueue<int> sq1(1024);
    sq1.Put(12);
    sq1.Put(23);
    std::cout<<sq1.Size()<<std::endl;
    int x;
    sq1.Take(x);
    std::cout<<sq1.Size()<<"\tx="<<x<<std::endl;

    SimpleSyncQueue<int> ssq1;
    ssq1.Put(34);
    ssq1.Put(52);

    std::cout<<ssq1.Size()<<std::endl;
    ssq1.Take(x);
    std::cout<<ssq1.Size()<<"\tx="<<x<<std::endl;

}


struct AtomicCounter{
    std::atomic<int> value;
    void increament()
    {
        ++value;
    }

    void decreament()
    {
        --value;
    }
    int get()
    {
        return value.load();
    }
};
void ch5_4()
{
    AtomicCounter ac1;
    ac1.decreament();
    std::cout<<ac1.get()<<std::endl;
}


std::once_flag flag;
void do_once()
{
    std::call_once(flag,[]{std::cout<<"Called once"<<std::endl;});
}
void ch5_5()
{
    //std::call_once()保证在多线程环境中，某个函数只被调用一次
    std::thread t1(do_once);
    std::thread t2(do_once);
    t1.join();
    t2.join();
}

void ch5_6()
{
    /*异步操作类
     * std::future 作为异步结果的传输通道，可以获取线程函数的返回值
     * std::promise 用来包装一个值，将数据和future绑定起来，方便线程赋值
     * std::package_task 用来包装一个可调用对象，将函数和future绑定起来，以便异步调用
    */

    std::promise<int> pr1;
    std::thread t([](std::promise<int>& p){p.set_value_at_thread_exit(9);},std::ref(pr1));
    std::future<int>  f1=pr1.get_future();
    auto r=f1.get();
    std::cout<<r<<std::endl;


    std::packaged_task<int()> task1([](){return 7;});
    std::thread t2(std::ref(task1));
    std::future<int> f2 = task1.get_future();
    std::cout<<f2.get()<<std::endl;

//    std::vector<std::shared_future<int>> vsf;
//    auto func3 = std::async(std::launch::async,[](int a,int b){return a+b;},2,3);
//    vsf.push_back(func3);
//    std::cout<<"The shared_future result is"<<vsf[0].get()<<std::endl;
}

void ch5_7()
{
    std::future<int> f1 = std::async(std::launch::async,[](){return 1;});
    std::cout<<f1.get()<<std::endl;

    std::future<int> f2 = std::async(std::launch::async,[](){std::cout<<2<<std::endl; return 2;});
    f2.wait();

    std::future<int> f3 = std::async(std::launch::async,
                                     [](){std::this_thread::sleep_for(std::chrono::seconds(1));return 3;});
    std::cout<<"waitting ..."<<std::endl;
    std::future_status status;
    do{
        status=f3.wait_for(std::chrono::milliseconds(200));
        switch (status) {
        case std::future_status::deferred:
            std::cout<<"deferred"<<std::endl;
            break;
        case std::future_status::ready:
            std::cout<<"ready"<<std::endl;
            break;
        case std::future_status::timeout:
            std::cout<<"timeout"<<std::endl;
        default:
            break;
        }
    }while(status==std::future_status::ready);
    std::cout<<"result is "<<f3.get()<<std::endl;
}
void testCh5()
{
    std::cout<<"Start to test chapter 5"<<std::endl;
//    ch5_1();
//    ch5_2();
//    ch5_3();
//    ch5_4();
//    ch5_5();
    ch5_6();
    ch5_7();
}
