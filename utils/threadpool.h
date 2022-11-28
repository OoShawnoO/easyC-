#ifndef THREADPOOL_H
#define THREADPOOL_H

/***
 *
 * 使用方式:
 * 创建一个类,该类需要拥有一个process方法.
 * 需要进行process时 使用ThreadPool的append方法将该类对象的指针加入执行队列
 *
 * */

#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <semaphore>
#include <exception>
#include <cassert>

template <typename T>
class ThreadPool{
private:
    static void* work(void* arg);
    void run();
    int thread_number;
    int max_requests;
    std::thread* threads;
    std::list<T*> queue;
    std::mutex mutex;
    std::counting_semaphore<0> sem{0};
    bool stop;
public:
    explicit ThreadPool(int _thread_number = 8,int _max_reuqests = 10000);
    ~ThreadPool();
    bool append(T* request);
};

template <typename T>
ThreadPool<T>::ThreadPool(int _thread_number,int _max_reuqests):
thread_number(_thread_number),max_requests(_max_reuqests)
{
    if(thread_number <= 0 || max_requests <= 0){
        throw std::exception();
    }
    threads = new std::thread[thread_number];
    assert(threads);
    for(int i=0;i<thread_number;i++){
        threads[i] = std::thread(work,this);
        assert(&threads[i]);
        threads[i].detach();
    }
}

template<typename T>
ThreadPool<T>::ThreadPool<T>::~ThreadPool() {
    delete []threads;
    stop = true;
}

template<typename T>
void *ThreadPool<T>::work(void *arg) {
    auto pool = (ThreadPool*)arg;
    pool->run();
    return pool;
}

template<typename T>
void ThreadPool<T>::run() {
    while(!stop){
        sem.acquire();
        mutex.lock();
        if(queue.empty()){
            mutex.unlock();
            continue;
        }
        T* request = queue.front();
        queue.pop_front();
        mutex.unlock();
        if(!request){
            continue;
        }
        request->process();
    }
}

template<typename T>
bool ThreadPool<T>::append(T *request) {
    mutex.lock();
    if(queue.size() > max_requests){
        mutex.unlock();
        return false;
    }
    queue.push_back(request);
    mutex.unlock();
    sem.release();
    return true;
}

#endif
