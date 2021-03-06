#include "EventTimerFd.h"
#include "Macros.h"

#include <sys/timerfd.h>
#include <algorithm>
#include <assert.h>
#include <string.h>
#include <unistd.h>

namespace cbee
{

EventTimerFd::EventTimerFd() : fd(::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC))
{
   assert(fd >= 0);
}

EventTimerFd::~EventTimerFd()
{
   __attribute__((unused)) auto ret = ::close(fd);
   assert(ret == 0);
}

void EventTimerFd::cleanEvent() const
{
   uint64_t numberOfEvents = 0;

   if (::read(fd, &numberOfEvents, sizeof(uint64_t)) < 0)
      HANDLE_ERROR("EventTimerFd::cleanEvent failure");

   assert(numberOfEvents == 1);
}

void EventTimerFd::triggerEventAt(Timestamp expiredTime) const
{
   struct itimerspec newValue;
   memset(&newValue, 0, sizeof(newValue));
   newValue.it_value = getRelativeTimeFromNow(expiredTime);
   if(::timerfd_settime(fd, 0 /* relative time */, &newValue, nullptr) < 0)
      HANDLE_ERROR("EventTimerFd::triggerEventAt failure");
}

int EventTimerFd::getFd() const
{
   return fd;
}

struct timespec EventTimerFd::getRelativeTimeFromNow(Timestamp expiredTime) const
{
   const int64_t kMinimalTimerIntervalUs = 100;
   int64_t microSeconds = std::max
   (
      kMinimalTimerIntervalUs,
      expiredTime.getMicroSeconds() - Timestamp::now().getMicroSeconds()
   );

   struct timespec ret;
   ret.tv_sec = static_cast<time_t>(microSeconds / Timestamp::kMicroSecondsPerSecond);
   ret.tv_nsec = static_cast<long>((microSeconds % Timestamp::kMicroSecondsPerSecond) * 1000);
   return ret;
}

}
