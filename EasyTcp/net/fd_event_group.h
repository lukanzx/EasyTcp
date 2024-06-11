#pragma once

#include <vector>
#include "EasyTcp/common/mutex.h"
#include "EasyTcp/net/fd_event.h"

namespace EasyTcp
{

  class FdEventGroup
  {

  public:
    FdEventGroup(int size);

    ~FdEventGroup();
    FdEvent *getFdEvent(int fd);

  public:
    static FdEventGroup *GetFdEventGroup();

  private:
    int m_size{0};
    std::vector<FdEvent *> m_fd_group;
    Mutex m_mutex;
  };

}
