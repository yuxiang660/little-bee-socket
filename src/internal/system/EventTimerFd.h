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
   EventTimerFd(const EventTimerFd&) = delete;
   EventTimerFd& operator=(const EventTimerFd&) = delete;

private:
   struct timespec getRelativeTimeFromNow(Timestamp expiredTime) const;

private:
   const int fd;
};

}
