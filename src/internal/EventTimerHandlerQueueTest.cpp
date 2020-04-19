#include "EventTimerHandlerQueue.h"
#include "internal/system/EPoller.h"
#include "internal/system/Timestamp.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>

namespace
{

struct EventTimerHandlerQueueEventTest : public ::testing::Test
{
   EventTimerHandlerQueueEventTest():
      poller(),
      queue(poller),
      invokeTimes(0)
   {}

   void loop(int times)
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
      }
   }

   void callback()
   {
      invokeTimes++;
   }

   cbee::EPoller poller;
   cbee::EventTimerHandlerQueue queue;
   int invokeTimes;
};

TEST_F(EventTimerHandlerQueueEventTest, addHandler_addDifferentHandlers_expectCallbackTimes)
{
   const int numberOfHandlers = 3;

   std::thread loopThread(&EventTimerHandlerQueueEventTest::loop, this, numberOfHandlers);

   for (int i = 0; i < numberOfHandlers; i++)
   {
      const cbee::TimerHandler handler(cbee::Timestamp::now().addSeconds(i * 0.01), 0, [this](){callback();});
      queue.addHandler(handler);
   }

   loopThread.join();

   EXPECT_EQ(numberOfHandlers, invokeTimes);
}

TEST_F(EventTimerHandlerQueueEventTest, addHandler_addSameHandlers_expectCallbackTimes)
{
   std::thread loopThread(&EventTimerHandlerQueueEventTest::loop, this, 1);

   for (int i = 0; i < 3; i++)
   {
      const cbee::TimerHandler handler(cbee::Timestamp::now(), 0, [this](){callback();});
      queue.addHandler(handler);
   }

   loopThread.join();

   EXPECT_EQ(3, invokeTimes);
}

TEST_F(EventTimerHandlerQueueEventTest, addHandler_inAnotherThread_expectCallback)
{
   auto test = [this](){
      const cbee::TimerHandler handler(cbee::Timestamp::now(), 0, [this](){callback();});
      queue.addHandler(handler);
   };
   std::thread testThread(test);

   loop(1);

   testThread.join();

   EXPECT_EQ(1, invokeTimes);
}

TEST_F(EventTimerHandlerQueueEventTest, addHandler_withInterval_expectCallbackTimes)
{
   const int numberOfHandlers = 3;

   std::thread loopThread(&EventTimerHandlerQueueEventTest::loop, this, numberOfHandlers);

   const cbee::TimerHandler handler(cbee::Timestamp::now(), 0.01, [this](){callback();});
   queue.addHandler(handler);

   loopThread.join();

   EXPECT_EQ(numberOfHandlers, invokeTimes);
}

TEST_F(EventTimerHandlerQueueEventTest, cancelHandler_withInterval_expectCallbackTimes)
{
   const cbee::TimerHandler handler(cbee::Timestamp::now(), 0.01, [this](){callback();});
   queue.addHandler(handler);

   auto activeEvents = poller.poll(20);
   EXPECT_EQ(1, activeEvents.size());
   activeEvents.front()->handleEvent();

   queue.cancelHandler(handler.getId());

   activeEvents = poller.poll(20);
   EXPECT_EQ(1, activeEvents.size());
   activeEvents.front()->handleEvent();

   activeEvents = poller.poll(20);
   EXPECT_EQ(0, activeEvents.size());

   EXPECT_EQ(1, invokeTimes);
}

} // namespace

