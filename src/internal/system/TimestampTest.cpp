#include "Timestamp.h"

#include "gtest/gtest.h"

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

}
