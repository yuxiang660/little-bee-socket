#include "Event.h"

#include <gtest/gtest.h>
#include <sys/epoll.h>

namespace
{

class EventTest : public ::testing::Test
{
public:
   EventTest() : writeDone(false), readDone(false), removeDone(false), errorDone(false)
   {
      e.setWriteCallback([&]() { writeDone = true; });
      e.setReadCallback([&]() { readDone = true; });
      e.setRemoveCallback([&]() { removeDone = true; });
      e.setErrorCallback([&]() { errorDone = true; });
   }

   cbee::Event e;
   bool writeDone;
   bool readDone;
   bool removeDone;
   bool errorDone;
};

TEST_F(EventTest, readEvent_expectedCallBack)
{
   e.setActiveEvents(cbee::Event::kConnectedOrReadableOrCloseEvent);
   e.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(true, readDone);
   EXPECT_EQ(false, removeDone);
   EXPECT_EQ(false, errorDone);
}

TEST_F(EventTest, writeEvent_expectedCallBack)
{
   e.setActiveEvents(cbee::Event::kWritableEvent);
   e.handleEvent();
   EXPECT_EQ(true, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(false, removeDone);
   EXPECT_EQ(false, errorDone);
}

TEST_F(EventTest, errorEvent_expectedCallBack)
{
   e.setActiveEvents(cbee::Event::kErrorEvent);
   e.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(false, removeDone);
   EXPECT_EQ(true, errorDone);
}

TEST_F(EventTest, destructEvent_expectedCallBack)
{
   e.setActiveEvents(cbee::Event::kRemoveEvent);
   e.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(true, removeDone);
   EXPECT_EQ(false, errorDone);
}

} // namespace
