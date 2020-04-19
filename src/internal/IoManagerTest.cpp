#include "IoManager.h"

#include <gtest/gtest.h>
#include <thread>

namespace
{

TEST(IoManagerTest, isInIoThread_inSameThread_returnTrue)
{
   cbee::IoManager io;

   EXPECT_EQ(io.isInIoThread(), true);
}

TEST(IoManagerTest, isInIoThread_inDifferentThread_returnFalse)
{
   cbee::IoManager io;

   auto test = [&]()
   {
      EXPECT_EQ(io.isInIoThread(), false);
   };

   std::thread testThread(test);
   testThread.join();
}

struct IoManagerLoopTest : public ::testing::Test
{
   IoManagerLoopTest() :
      io(),
      invokeTimes(0)
   {}

   void callback()
   {
      EXPECT_EQ(true, io.isInIoThread());
      invokeTimes++;
   }

   cbee::IoManager io;
   int invokeTimes;
};

TEST_F(IoManagerLoopTest, runAfter_inDifferentThread_expectedLoopIterationTimes)
{
   auto addHandlers = [this](){
      EXPECT_EQ(false, io.isInIoThread());

      io.runAfter(0, [this](){callback();});
      io.runAfter(0.01, [this](){callback();});
      io.runAfter(0.02, [this](){callback();});

      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      io.quit();
   };
   std::thread testThread(addHandlers);

   io.loop();

   testThread.join();
   EXPECT_EQ(3, invokeTimes);
}

}
