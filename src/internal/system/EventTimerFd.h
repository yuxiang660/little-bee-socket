#pragma once

#include "Timestamp.h"

namespace cbee
{

class EventTimerFd
{
public:
   static const int64_t kMinimalTimerIntervalUs = 100;

   EventTimerFd();
   ~EventTimerFd();

   void cleanEvent() const;
   void triggerEventAt(Timestamp expiredTime) const;
   void triggerEventRightNow() const;

   int getFd() const;

private:
   EventTimerFd(const EventTimerFd&) = delete;
   EventTimerFd& operator=(const EventTimerFd&) = delete;

private:
   struct timespec getRelativeTimeFromNow(Timestamp expiredTime = 0) const;

private:
   const int fd;
};

}
