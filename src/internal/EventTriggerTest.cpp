#include "EventTrigger.h"
#include "internal/system/EPoller.h"

#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>

namespace
{

class EventTriggerTest : public ::testing::Test
{
public:
   EventTriggerTest():
      poller(),
      trigger(poller),
      isEventCleared(true)
   {}

   void poll(int times)
   {
      for (int i = 0; i < times; i++)
      {
         auto activeEvents = poller.poll(-1 /* block indefinitely */);
         EXPECT_EQ(1, activeEvents.size());
         EXPECT_EQ
         (
            true,
            (cbee::Event::kConnectedOrReadableOrCloseEvent & activeEvents.front()->getActiveEvents())
         );
         activeEvents.front()->handleEvent();
         isEventCleared = true;
      }
   }

   cbee::EPoller poller;
   cbee::EventTrigger trigger;
   std::atomic<bool> isEventCleared;
};

TEST_F(EventTriggerTest, triggerOnce_invokeSeveralTimes_triggerEPollExpected)
{
   const int triggerTimes = 5;

   std::thread pollThread(&EventTriggerTest::poll, this, triggerTimes);
   // Delay for the connection to make sure poll is started.
   std::this_thread::sleep_for(std::chrono::milliseconds(50));

   int times = 0;
   while(1)
   {
      if(isEventCleared.exchange(false))
      {
         trigger.triggerOnce();
         times++;
      }
      if (triggerTimes == times) break;
   }

   pollThread.join();
}

}
