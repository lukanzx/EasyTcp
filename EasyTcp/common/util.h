#pragma once

#include <sys/types.h>
#include <unistd.h>

namespace EasyTcp
{

    pid_t getPid();

    pid_t getThreadId();

    int64_t getNowMs();

    int32_t getInt32FromNetByte(const char *buf);

}
