#include <assert.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <memory>
#include <unistd.h>
#include <google/protobuf/service.h>
#include "EasyTcp/common/log.h"
#include "EasyTcp/common/config.h"
#include "EasyTcp/common/log.h"
#include "EasyTcp/net/tcp/tcp_client.h"
#include "EasyTcp/net/tcp/net_addr.h"
#include "EasyTcp/net/coder/string_coder.h"
#include "EasyTcp/net/coder/abstract_protocol.h"
#include "EasyTcp/net/coder/tinypb_coder.h"
#include "EasyTcp/net/coder/tinypb_protocol.h"
#include "EasyTcp/net/tcp/net_addr.h"
#include "EasyTcp/net/tcp/tcp_server.h"
#include "EasyTcp/net/rpc/rpc_dispatcher.h"

#include "order.pb.h"

class OrderImpl : public Order
{
public:
  void makeOrder(google::protobuf::RpcController *controller,
                 const ::makeOrderRequest *request,
                 ::makeOrderResponse *response,
                 ::google::protobuf::Closure *done)
  {
    APPDEBUGLOG("start sleep 5s");
    sleep(5);
    APPDEBUGLOG("end sleep 5s");
    if (request->price() < 10)
    {
      response->set_ret_code(-1);
      response->set_res_info("short balance");
      return;
    }
    response->set_order_id("20230514");
    APPDEBUGLOG("call makeOrder success");
    if (done)
    {
      done->Run();
      delete done;
      done = NULL;
    }
  }
};

int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    printf("Start test_rpc_server error, argc not 2 \n");
    printf("Start like this: \n");
    printf("./test_rpc_server ../conf/EasyTcp.xml \n");
    return 0;
  }

  EasyTcp::Config::SetGlobalConfig(argv[1]);

  EasyTcp::Logger::InitGlobalLogger();

  std::shared_ptr<OrderImpl> service = std::make_shared<OrderImpl>();
  EasyTcp::RpcDispatcher::GetRpcDispatcher()->registerService(service);

  EasyTcp::IPNetAddr::s_ptr addr = std::make_shared<EasyTcp::IPNetAddr>("127.0.0.1", EasyTcp::Config::GetGlobalConfig()->m_port);

  EasyTcp::TcpServer tcp_server(addr);

  tcp_server.start();

  return 0;
}