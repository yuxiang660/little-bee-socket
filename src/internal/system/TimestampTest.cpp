#include "Timestamp.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>

namespace
{

TEST(TimestampTest, constructor_noArg_expectedZero)
{
   cbee::Timestamp time;
   EXPECT_EQ(0, time.getMicroSeconds());
}

TEST(TimestampTest, constructor_microSecondsArg_expectedTime)
{
   const int64_t expectedMicroSeconds = 10;
   cbee::Timestamp time(expectedMicroSeconds);
   EXPECT_EQ(expectedMicroSeconds, time.getMicroSeconds());
}

TEST(TimestampTest, now_timeIsNotZero)
{
   cbee::Timestamp time = cbee::Timestamp::now();
   EXPECT_NE(0, time.getMicroSeconds());
}

TEST(TimestampTest, compareOperations_work)
{
   cbee::Timestamp time1 = cbee::Timestamp::now();
   std::this_thread::sleep_for(std::chrono::milliseconds(1));
   cbee::Timestamp time2 = cbee::Timestamp::now();
   EXPECT_EQ(true, time1 < time2);
   EXPECT_EQ(true, time2 > time1);
   EXPECT_EQ(true, time2 != time1);
   EXPECT_EQ(false, time1 == time2);
}

TEST(TimestampTest, addSeconds_expectedNewTimestamp)
{
   double seconds = 1.5;
   cbee::Timestamp now = cbee::Timestamp::now();
   cbee::Timestamp future = now.addSeconds(seconds);

   EXPECT_EQ
   (
      seconds * cbee::Timestamp::kMicroSecondsPerSecond,
      future.getMicroSeconds() - now.getMicroSeconds()
   );
}

}
