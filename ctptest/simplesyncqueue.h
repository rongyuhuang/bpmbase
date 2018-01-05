#ifndef SIMPLESYNCQUEUE_H
#define SIMPLESYNCQUEUE_H

#include<thread>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<list>

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

    void Take(std::list<T>& list)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_notEmpty.wait(lock,[this]{return m_queue.empty();});
        list = std::move(m_queue);
        //m_notFull.notify_one();
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
#endif // SIMPLESYNCQUEUE_H
