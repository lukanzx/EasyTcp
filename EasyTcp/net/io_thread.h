#pragma once


#include <pthread.h>
#include <semaphore.h>
#include "EasyTcp/net/eventloop.h"

namespace EasyTcp
{

  class IOThread
  {
  public:
    IOThread();

    ~IOThread();

    EventLoop *getEventLoop();

    void start();

    void join();

  public:
    static void *Main(void *arg);

  private:
    pid_t m_thread_id{-1}; // 线程号
    pthread_t m_thread{0}; // 线程句柄

    EventLoop *m_event_loop{NULL}; // 当前 io 线程的 loop 对象

    sem_t m_init_semaphore;

    sem_t m_start_semaphore;
  };

}
