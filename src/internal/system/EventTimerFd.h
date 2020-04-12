#pragma once

#include "Timestamp.h"

namespace cbee
{

class EventTimerFd
{
public:
   EventTimerFd();
   ~EventTimerFd();

   void cleanEvent();
   void resetEventTimer(Timestamp expiredTime);

   int getFd() const;

private:
   struct timespec getRelativeTimeFromNow(Timestamp expiredTime) const;

private:
   const int fd;
};

}
