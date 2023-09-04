//
// Created by Hestar on 2023/9/4.
//

#ifndef SIMPLEFTP_THREADPOOL_H
#define SIMPLEFTP_THREADPOOL_H

#include <list>
#include "locker.h"

template<typename T>
class threadpool{
public:
    // 参数thread_number是线程池中线程的数量，max_requests是请求队列中最多允许的、等待处理的请求的数量
    threadpool(int thread_number = 8,int max_requests = 1000);
    ~threadpool();
    bool append(T* request);
    static void* worker(void* arg);
    void run();
private:
    int m_thread_number; // 线程池中的线程数
    int m_max_requests; // 请求队列中允许的最大请求数
    pthread_t* m_threads; // 描述线程池的数组，其大小为m_thread_number
    std::list<T*> m_workqueue; // 请求队列
    locker m_queuelocker; // 保护请求队列的互斥锁
    sem m_queuestat; //是否有任务需要处理
    bool m_stop; // 是否结束线程
};


#endif //SIMPLEFTP_THREADPOOL_H
