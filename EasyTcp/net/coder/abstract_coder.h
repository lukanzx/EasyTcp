#pragma once

#include <vector>
#include "EasyTcp/net/tcp/tcp_buffer.h"
#include "EasyTcp/net/coder/abstract_protocol.h"

namespace EasyTcp
{

  class AbstractCoder
  {
  public:
    // 将 message 对象转化为字节流，写入到 buffer
    virtual void encode(std::vector<AbstractProtocol::s_ptr> &messages, TcpBuffer::s_ptr out_buffer) = 0;

    // 将 buffer 里面的字节流转换为 message 对象
    virtual void decode(std::vector<AbstractProtocol::s_ptr> &out_messages, TcpBuffer::s_ptr buffer) = 0;

    virtual ~AbstractCoder() {}
  };

}
