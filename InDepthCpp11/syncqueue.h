#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H
#include<list>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<iostream>

template<typename T>
class SyncQueue{
public:
    SyncQueue(int maxSize):m_maxSize(maxSize),m_needStop(false){}
    void Put(const T& x)
    {
        Add(x);
    }
    void Put(T&& x)
    {
        Add(std::forward<T>(x));
    }

    void Take(std::list<T>& list)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_notEmpty.wait(lock,[this]{return m_needStop|| NotEmpty();});
        if(m_needStop)
        {
            return;
        }
        list = std::move(m_queue);
        m_notFull.notify_one();
    }

    void Take(T& t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_notEmpty.wait(m_mutex,[this]{return m_needStop|| NotEmpty();});
        if(m_needStop)
        {
            return;
        }
        t= m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
    }

    void Stop()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_needStop = true;
        }
        m_notFull.notify_all();
        m_notEmpty.notify_all();
    }

    bool Empty()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    bool Full()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue==m_maxSize;
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

private:
    bool NotFull()const
    {
        bool full = m_queue.size()>=m_maxSize;
        if(full)
        {
            std::cout<<"缓冲区已满，需要等待..."<<std::endl;
        }
        return !full;
    }

    bool NotEmpty()const
    {
        bool empty = m_queue.empty();
        if(empty)
        {
            std::cout<<"缓冲区已空，需要等待...，异步层的线程ID:"<<std::this_thread::get_id()<<std::endl;
        }
        return !empty;
    }

    void Add(const T& x)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_notFull.wait(lock,[this]{return m_needStop || NotFull();});
        if(m_needStop)
        {
            return;
        }
        m_queue.push_back(x);
        m_notEmpty.notify_one();
    }
private:
    int m_maxSize;
    bool m_needStop;
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
    std::condition_variable m_notFull;
};

#endif // SYNCQUEUE_H
