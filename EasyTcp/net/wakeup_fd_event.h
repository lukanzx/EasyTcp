#pragma once

#include "EasyTcp/net/fd_event.h"

namespace EasyTcp
{

    class WakeUpFdEvent : public FdEvent
    {
    public:
        WakeUpFdEvent(int fd);

        ~WakeUpFdEvent();

        void wakeup();

    private:
    };

}
