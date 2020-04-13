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
   const double delay = 0.1;
   timer.triggerEventAt(cbee::Timestamp::now().addSeconds(delay));

   // The timer hasn't expired.
   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");
   // Delay for timer expiration.
   const int64_t delayUs = delay * cbee::Timestamp::kMicroSecondsPerSecond;
   std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
   ASSERT_NO_THROW(timer.cleanEvent());
   // The timer event has been cleared.
   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");
}

TEST(EventTimerFdTest, resetEventTimer_resetTwice_twoEventsHappen)
{
   cbee::EventTimerFd timer;
   const double delay = 0.1;
   timer.triggerEventAt(cbee::Timestamp::now().addSeconds(delay));

   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");
   const int64_t delayUs = delay * cbee::Timestamp::kMicroSecondsPerSecond;
   std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
   ASSERT_NO_THROW(timer.cleanEvent());

   timer.triggerEventAt(cbee::Timestamp::now().addSeconds(delay));

   EXPECT_DEATH(timer.cleanEvent(), "EventTimerFd::cleanEvent failure");
   std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
   ASSERT_NO_THROW(timer.cleanEvent());
}

TEST(EventTimerFdTest, triggerEventRightNow_cleanEvents_noException)
{
   cbee::EventTimerFd timer;
   timer.triggerEventRightNow();

   const int64_t minDelay = cbee::EventTimerFd::kMinimalTimerIntervalUs * 10 /* add some jitter */;
   std::this_thread::sleep_for(std::chrono::microseconds(minDelay));
   ASSERT_NO_THROW(timer.cleanEvent());
}

}
