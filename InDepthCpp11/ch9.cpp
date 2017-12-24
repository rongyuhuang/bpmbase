#include<list>
#include<mutex>
#include<thread>
#include<atomic>
#include<functional>
#include<condition_variable>
#include<iostream>

#include"syncqueue.h"

const int MaxTaskCount =100;

class ThreadPool{
public:
    using Task = std::function<void()>;
    ThreadPool(int numThreads=std::thread::hardware_concurrency()):m_queue(MaxTaskCount)
    {
        Start(numThreads);
    }

    ~ThreadPool()
    {
        Stop();
    }

    void Stop()
    {
        std::call_once(m_flag,[this]{StopThreadGroup();});
    }

    void AddTask(Task&& t)
    {
        m_queue.Put(std::forward<Task>(t));
    }

    void AddTask(const Task& t)
    {
        m_queue.Put(t);
    }
private:
    void Start(int numThreads)
    {
        m_running = true;
        //创建线程组
        for(int i=0;i<numThreads;++i)
        {
            m_threadGroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread,this));
        }
    }

    void RunInThread()
    {
        while(m_running)
        {
            //取任务分别执行
            std::list<Task> list;
            m_queue.Take(list);
            for(auto& task:list)
            {
                if(!m_running)
                {
                    return;
                }
                task();
            }
        }
    }

    void StopThreadGroup()
    {
        m_queue.Stop();
        m_running =false;
        for(auto t :m_threadGroup)
        {
            if(t)
            {
                t->join();
            }
        }
        m_threadGroup.clear();
    }
private:
    std::list<std::shared_ptr<std::thread>> m_threadGroup;
    SyncQueue<Task> m_queue;
    std::atomic_bool m_running;
    std::once_flag m_flag;
};

void testCh9()
{
    std::cout<<"Start to test chapter 9"<<std::endl;
    //std::cout.imbue(std::locale("chs")); //初始化cout为中文输出
    ThreadPool pool;
    std::thread t1([&pool]{
        for(int i=0;i<10;++i)
        {
            auto tid = std::this_thread::get_id();
            pool.AddTask([tid]{std::cout<<"同步底层线程1的ID："<<tid<<std::endl;});
        }
    });
    std::thread t2([&pool]{
        for(int i=0;i<10;++i)
        {
            auto tid = std::this_thread::get_id();
            pool.AddTask([tid]{std::cout<<"同步底层线程2的ID："<<tid<<std::endl;});
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::getchar();
    pool.Stop();;
    t1.join();
    t2.join();
}
