#include "TimerHandler.h"
#include "internal/system/Timestamp.h"

#include <gtest/gtest.h>

namespace
{

struct TimerHandlerTest : public ::testing::Test
{
   TimerHandlerTest() :
      invokeTimes(0)
   {}

   void callback()
   {
      invokeTimes++;
   }

   int invokeTimes;
};

TEST_F(TimerHandlerTest, run_callSeveralTimes_callbackInvokeExpected)
{
   cbee::TimerHandler timer(cbee::Timestamp::now(), 0, [this](){callback();});

   timer.run();
   EXPECT_EQ(1, invokeTimes);

   timer.run();
   EXPECT_EQ(2, invokeTimes);
}

struct TimerHandlerExpiredTimeTestCase
{
   int64_t initialExpiredTime;
   double interval;
   double expectedUpdateReturn;
   int64_t expectedExpiredTimeAfterUpdateOnce;
   int64_t expectedExpiredTimeAfterUpdateTwice;
};

TimerHandlerExpiredTimeTestCase timerHandlerExpiredTimeTestCases[] =
{
   {1000000,   0, false, 1000000, 1000000},
   {1000000, 0.5, true,  1500000, 2000000},
   {0,       0.5, true,  500000,  1000000},
   {0,       0,   false, 0,       0      }
};

struct TimerHandlerExpiredTimeTest : public ::testing::TestWithParam<TimerHandlerExpiredTimeTestCase>
{
};

TEST_P(TimerHandlerExpiredTimeTest, updateExpiredTime_getExpectedExpiredTime)
{
   auto testCase = GetParam();

   cbee::TimerHandler timer(testCase.initialExpiredTime, testCase.interval, nullptr);

   EXPECT_EQ(testCase.expectedUpdateReturn, timer.updateExpiredTime(timer.getExpiredTime()));
   EXPECT_EQ(testCase.expectedExpiredTimeAfterUpdateOnce, timer.getExpiredTime());

   EXPECT_EQ(testCase.expectedUpdateReturn, timer.updateExpiredTime(timer.getExpiredTime()));
   EXPECT_EQ(testCase.expectedExpiredTimeAfterUpdateTwice, timer.getExpiredTime());
}

INSTANTIATE_TEST_CASE_P(TimerHandlerExpiredTimeTest, TimerHandlerExpiredTimeTest,
                        ::testing::ValuesIn(timerHandlerExpiredTimeTestCases));

}
