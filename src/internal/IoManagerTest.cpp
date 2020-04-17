#include "IoManager.h"

#include <gtest/gtest.h>
#include <thread>

namespace
{

TEST(IoManagerTest, isInIoThread_loopNotStarted_exception)
{
   cbee::IoManager io;

   EXPECT_DEATH(io.isInLoopThread(), "IoManager::isInLoopThread failure");
}

TEST(IoManagerTest, isInIoThread_inDifferentThreadAfterLoopStarted_returnFalse)
{
   cbee::IoManager io;

   auto startLoop = [&]()
   {
      io.loop();
   };

   std::thread loopThread(startLoop);

   std::this_thread::sleep_for(std::chrono::milliseconds(50));
   EXPECT_EQ(false, io.isInLoopThread());
   io.quit();

   loopThread.join();
}

struct IoManagerLoopTest : public ::testing::Test
{
   IoManagerLoopTest() :
      io(),
      invokeTimes(0)
   {}

   void callback()
   {
      EXPECT_EQ(true, io.isInLoopThread());
      invokeTimes++;
   }

   cbee::IoManager io;
   int invokeTimes;
};

TEST_F(IoManagerLoopTest, runAfter_inDifferentThread_expectedLoopIterationTimes)
{
   auto addHandlers = [this](){
      EXPECT_EQ(false, io.isInLoopThread());

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
