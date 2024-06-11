#pragma once

#include <vector>
#include "EasyTcp/common/log.h"
#include "EasyTcp/net/io_thread.h"

namespace EasyTcp
{

  class IOThreadGroup
  {

  public:
    IOThreadGroup(int size);

    ~IOThreadGroup();

    void start();

    void join();

    IOThread *getIOThread();

  private:
    int m_size{0};
    std::vector<IOThread *> m_io_thread_groups;

    int m_index{0};
  };

}
