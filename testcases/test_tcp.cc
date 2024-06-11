#include <memory>
#include "EasyTcp/common/log.h"
#include "EasyTcp/net/tcp/net_addr.h"
#include "EasyTcp/net/tcp/tcp_server.h"

void test_tcp_server()
{

  EasyTcp::IPNetAddr::s_ptr addr = std::make_shared<EasyTcp::IPNetAddr>("127.0.0.1", 12346);

  DEBUGLOG("create addr %s", addr->toString().c_str());

  EasyTcp::TcpServer tcp_server(addr);

  tcp_server.start();
}

int main()
{

  // EasyTcp::Config::SetGlobalConfig("../conf/EasyTcp.xml");
  // EasyTcp::Logger::InitGlobalLogger();

  EasyTcp::Config::SetGlobalConfig(NULL);

  EasyTcp::Logger::InitGlobalLogger(0);

  test_tcp_server();
}