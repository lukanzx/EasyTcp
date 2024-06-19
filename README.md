### EasyTcp

#### 1. Overview

EasyTcp is a multi-threaded asynchronous RPC framework developed based on C++11,
It aims to be efficient and concise while maintaining extremely high performance.

EasyTcp is also based on the master-slave Reactor architecture, and the underlying layer uses epoll to implement IO multiplexing.
The application layer is based on protobuf to customize the rpc communication protocol, and will also support simple HTTP protocols.


#### 2. Framework advantages
 - High performance: master-slave Reactor architecture, multi-threaded concurrent processing The underlying layer is multiplexed through epoll
 - Asynchronous: supports asynchronous RPC calls, and the calling party needs to wait for the return synchronously
 - Serialization: Based on Prtobuf serialization of data, lightweight protocol, high transmission efficiency
 - Framing: Implement a Generator code generator to generate an RPC framework with just one click, improving development efficiency
 - Business decoupling: Business developers can use it out of the box without worrying about any underlying framework logic
 - Asynchronous logging: Logs support scrolling by date and size; And there are asynchronous threads responsible for output, improving efficiency
 - Configuration: Provides an XML configuration module that automatically loads configuration files upon startup


#### 3. Technology stack keywords
 - C++11
 - Protobuf
 - Rpc
 - Reactor
 - HTTP


#### 4. Code development

##### 4.1 flow
 - Development of logs and configuration classes
 - Reactor Core Module
 - Tcp module packaging
 - Serialization and encoding/decoding modules
 - Rpc module packaging
 - Scaffolding construction
 - Simple performance testing

```
1. Environmental installation and project development
    1.1 Environment setup and dependency library installation
    1.2 Log module development
    1.3 Development of configuration modules

2. EventLoop module encapsulation
    2.1 EventLoop Core Class Construction
    2.2 FdEvent encapsulation and testing
    2.3 Timer
    2.4 Master Slave Reactors
    2.5 Overall testing of EventLoop module

3. Tcp module packaging
    3.1 TcpBuffer
    3.2 TcpConnection
    3.3 TcpServer (1)
    3.4 TcpServer (II)
    3.4 TcpClient
    3.5 Tcp module testing (1)
    3.5 Tcp module testing (II)

4. RPC protocol encapsulation
    4.1 TinyPB protocol encoding
    4.2 TinyPB protocol decoding
    4.3 Encoding and decoding module testing

5. RPC communication module packaging
    5.1 Basic classes such as RpcController and RcpClosure
    5.2 RpcDispatcher distributor
    5.3 RpcChannel
    5.4 RpcAsyncChannel
    5.5 Rpc module integration testing

6. RPC scaffold encapsulation
    6.1 Code Generator Development
    6.2 Project Construction and Testing
```

##### 4.1 目录
```
EasyTcp
    -Bin
        --Store test programs, executable programs
    -Conf
        --Store XML configuration files for testing purposes
    -Lib
        --Store the compiled static library libEasyTcp.a
    -Obj
        --Store all compiled primary key files, *. o
    -EasyTcp
        --Store all source code
    -Testcases
        --Store test code
```


#### 5. Introduction to Core Development

##### 5.1 Log module development

Log module:
```
1. Log level
2. Print to file, support date naming, and scrolling of logs.
3. C formatting risk control
4. Thread safety
```

LogLevel:
```
Debug
Info
Error
```

LogEvent:
```
File name, line number
MsgNo
Process number
Thread id
Date and time. Accurate to ms
Custom message
```

Log format
```
[Level] [% y -% m -% d% H:% M:% s.% ms] \ t [pid: thread_id] \ t [filename: line] [% msg]
```
Logger Logger    
   1. Provide a method for printing logs    
   2. Set the path for log output    


##### 5.2 Reactor
Reactor， Also known as EventLoop, its essence is an event loop model    
The core logic of Rtracker (also known as EventLoop) is a loop loop,     
described in pseudocode as follows:    

```
void loop() {
    while(!stop) {
        foreach (task in tasks) {
            task();
        }

        // 1. Obtain the time for the next scheduled task and subtract the larger value from the set time_out,
        // If the next scheduled task time exceeds 1 second, 
        // take the next scheduled task time as the timeout time; otherwise, take 1 second
        int time_out = Max(1000, getNextTimerCallback());

        // 2. Call Epoll to wait for an event to occur, 
        // with a timeout of the timeouts mentioned above
        int rt = epoll_wait(epfd, fds, ...., time_out); 

        if(rt < 0) {
            // The epoll call failed..
        } 
        else {
            if (rt > 0 ) {
                foreach (fd in fds) {

                    // Add pending tasks to the execution queue
                    tasks.push(fd);
                }
            }
        }
    }
}
```
    
In EasyTcp, the master-slave Reactor model is used    
The server has one mainReactor and multiple subReactors.    

The mainReactor is run by the main thread and its function is as follows:    
Listening to listenfd's readable events through epoll,    
When a readable event occurs,     
call the accept function to get clientfd, and then randomly get a subReactor,    
Register the read and write events of cliednfd to the epoll of this subReactor.    

That is to say, mainReactor is only responsible for establishing connection events,     
does not perform business processing, and does not care about IO events of connected sockets.    

SubReactors typically have multiple, each of which is run by a thread.    
The epoll of the subReactor has registered read-write events for clientfd.     
When an IO event occurs, business processing is required.       

##### 5.3 TimerEvent Timed tasks
```
1. Specify a time point : arrive_time
2. interval, ms。
3. is_repeated 
4. is_cancled
5. task

cancle()
cancleRepeated()
```

##### 5.4 Timer
Timer, it is a collection of TimerEvents. Timer inherits FdEvent
```
addTimerEvent();
deleteTimerEvent();

onTimer();    // The methods that need to be executed after an IO event occurs

reserArriveTime()

multimap Store TimerEvent<key (arrivetime), TimerEvent>
```

##### 5.5 IO 线程

Create an IO thread that will help us execute:
Create a new thread (pthread_create)
Create an EventLoop in a new thread and complete initialization
Enable loop  
```
class {
    pthread_t m_thread;
    pid_t m_thread_id;
    EventLoop event_loop;
}

```

RPC Server process
```
Register the OrderService object at startup.

1. Read data from buufer and decode to obtain the requested TinyPBProtocol object.
    Then obtain the methodname from the requested TinyPBProtocol,
    Find method fun from the OrderService object based on service.method_name

2. Find the corresponding request type and response type

3. Deserialize the pb_date in the request body TinyPBProtocol into an object of requesttype,
Declare an empty response type object

4. Func (request, response)

5. Sequence the reponse object as pb_data.
    Then insert it into the TinyPBProtocol structure.
    Encode it and stuff it into the buffer, and it will send the packet back
```

#### 6 使用 Generator 生成代码

Assuming the need to build an RPC service,     
the function of the service is to provide an ordering interface and implement order production logic.     

Firstly, locate the EasyTcp_generator.py file and copy its absolute path,     
For example:/data/home/ikerli/new workspace/easyTcp/generator/easyTcp_generator.py

Then, enter any directory and first create a protobuf file,     
Such as cd/data/home/ikerli/new workspace vim order. proto     
Here is the example order. proto as follows     

```
syntax = "proto3";
option cc_generic_services = true;

message makeOrderRequest {
  int32 price = 1;
  string goods = 2;
}

message makeOrderResponse {
  int32 ret_code = 1;
  string res_info = 2;
  string order_id = 3;
}

service Order {
  rpc makeOrder(makeOrderRequest) returns (makeOrderResponse);
}
```

Then execute the command: python3 EasyTcp/generator/EasyTcp generator. py - i order. proto - o/
-i: Represents the source file, which needs to be the root path of the proto file you created - o: generated file

Therefore, the meaning of this line of code is to generate RPC framework code in the current directory through the order.proto file

Note that you need to install Python 3 to execute successfully. After execution, 
you can see that an order directory has been generated in the current path:

```
order:
    -Bin: executable program path. After successful make, an executable file will be generated in this directory, which can be run directly
        -Run. sh starts the RPC script
        -Shutdown.sh stops the RPC script

    -Conf: The path to the configuration file, which stores the EasyTcp.
           XML configuration file and generally does not need to be modified

    -Lib: Library file path, storing static library files
    -Obj: Library file path, storing some compiled intermediate files (*. o)
    -Log: The log path, where log files will be generated after starting the RPC service
    -Test_client: Test file directory, providing programs for testing RPC interfaces

    -Order: Project source code path
        -Comm: Store some public files

        -Interface:
            -Make_order. h: corresponds to rpc makeOrder (makeOrderRequest) returns (makeOrderResponse),
            -Each rpc method defined under the service under proto will generate an interface in this directory
            -Makereorder.cc
            -Interface. h: Interface base class file, each RPC interface inherits this class, 
                           making it convenient to write common logic shared by multiple interfaces
            -Interface.cc

        -PB:
            -Order. pb.h: Use the protoc command to generate a file based on order. proto
            -Order.pb.cc: Use the protoc command to generate a file based on order.proto
            -Order. ptoto: The original proto file, this is just a copy of the source file

        -Service:
            -Order.cc: Responsible for distributing RPC requests to the corresponding interfaces under the corresponding interfaces
            -Order. h:

        -Main.cc: RPC startup program, where the main function is implemented
        -Makefile: The project makefile, which can be executed directly under normal circumstances
```

Note that this is only an example, the actual proto file is designed based on specific business logic,    
Simply put, it is to design corresponding interfaces,     
request parameters, and return parameters based on the business behavior of RPC services    

For example, to implement a payment service, it can be: pay. proto:
```
syntax = "proto3";
option cc_generic_services = true;

// Payment request
message payRequest {
  int32 amount = 1; // Payment amount
}

message makeOrderResponse {
  int32 ret_code = 1;
  string res_info = 2;
  string pay_result = 3; Payment results
}

service Order {
  rpc makeOrder(makeOrderRequest) returns (makeOrderResponse);
}
```
