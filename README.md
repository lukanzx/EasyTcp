## EasyTcp

### 1. Overview
EasyTcp is a multi-threaded asynchronous RPC framework developed based on C++11, 
designed to be efficient and concise while maintaining extremely high performance.

EasyTcp is also based on the master-slave Reactor architecture, 
and the underlying layer uses epoll to implement IO multiplexing.
The application layer is based on protobuf to customize the rpc communication protocol, 
and will also support simple HTTP protocols.

#### 2. Technical Stack Keywords
 - C++11
 - Protobuf
 - Rpc
 - Reactor
 - HTTP