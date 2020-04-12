#pragma once

#include "Timestamp.h"

namespace cbee
{

class EventTimerFd
{
public:
   EventTimerFd();
   ~EventTimerFd();

   void cleanEvent() const;
   void resetEventTimer(Timestamp expiredTime) const;

   int getFd() const;

private:
   struct timespec getRelativeTimeFromNow(Timestamp expiredTime) const;

private:
   const int fd;
};

}
