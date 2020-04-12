#include "EventTimerFd.h"
#include "Timestamp.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>

namespace
{

TEST(EventTimerFdTest, resetEventTimer_cleanEvents_eventCleared)
{
   cbee::EventTimerFd timer;
   EXPECT_GE(timer.getFd(), 0);

   const int64_t delayMicroSeconds = 50000;
   cbee::Timestamp expiredTime
   (
      cbee::Timestamp::now().getMicroSeconds() + delayMicroSeconds
   );
   timer.resetEventTimer(expiredTime);

   // The timer hasn't expired.
   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");

   // Delay for timer expiration.
   std::this_thread::sleep_for(std::chrono::microseconds(delayMicroSeconds));

   timer.cleanEvent();
   // The timer event has been cleared.
   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");
}

TEST(EventTimerFdTest, resetEventTimer_resetTwice_twoEventsHappen)
{
   cbee::EventTimerFd timer;

   const int64_t delayMicroSeconds = 50000;

   cbee::Timestamp firstExpiredTime
   (
      cbee::Timestamp::now().getMicroSeconds() + delayMicroSeconds
   );
   timer.resetEventTimer(firstExpiredTime);

   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");
   std::this_thread::sleep_for(std::chrono::microseconds(delayMicroSeconds));
   timer.cleanEvent();
   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");

   cbee::Timestamp secondExpiredTime
   (
      cbee::Timestamp::now().getMicroSeconds() + delayMicroSeconds
   );
   timer.resetEventTimer(secondExpiredTime);

   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");
   std::this_thread::sleep_for(std::chrono::microseconds(delayMicroSeconds));
   timer.cleanEvent();
   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");
}

}
