#include "EventTriggerFd.h"

#include <gtest/gtest.h>

namespace
{

TEST(EventTriggerFdTest, triggerEvent_cleanEvents_expectedEventNumber)
{
   const int expectedNumberOfEvents = 3;

   cbee::EventTriggerFd trigger;

   for (int i = 0; i < expectedNumberOfEvents; i++)
   {
      trigger.triggerEvent();
   }

   int numberOfEvents = trigger.cleanEvents();

   EXPECT_EQ(expectedNumberOfEvents, numberOfEvents);
   ASSERT_DEATH(trigger.cleanEvents(), "EventTriggerFd::cleanEvent failure");
}

}
