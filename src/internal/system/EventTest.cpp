#include "Event.h"
#include "Socket.h"

#include <gtest/gtest.h>
#include <sys/epoll.h>

namespace
{

class EventTest : public ::testing::Test
{
public:
   EventTest() :
      event(socket),
      writeDone(false),
      readDone(false),
      removeDone(false),
      errorDone(false)
   {
      event.setWriteCallback([&]() { writeDone = true; });
      event.setReadCallback([&]() { readDone = true; });
      event.setRemoveCallback([&]() { removeDone = true; });
      event.setErrorCallback([&]() { errorDone = true; });
   }

   ~EventTest()
   {
   }

   cbee::Socket socket;
   cbee::Event event;
   bool writeDone;
   bool readDone;
   bool removeDone;
   bool errorDone;
};

TEST_F(EventTest, readEvent_expectedCallBack)
{
   event.setActiveEvents(cbee::Event::kConnectedOrReadableOrCloseEvent);
   event.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(true, readDone);
   EXPECT_EQ(false, removeDone);
   EXPECT_EQ(false, errorDone);
}

TEST_F(EventTest, writeEvent_expectedCallBack)
{
   event.setActiveEvents(cbee::Event::kWritableEvent);
   event.handleEvent();
   EXPECT_EQ(true, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(false, removeDone);
   EXPECT_EQ(false, errorDone);
}

TEST_F(EventTest, errorEvent_expectedCallBack)
{
   event.setActiveEvents(cbee::Event::kErrorEvent);
   event.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(false, removeDone);
   EXPECT_EQ(true, errorDone);
}

TEST_F(EventTest, destructEvent_expectedCallBack)
{
   event.setActiveEvents(cbee::Event::kRemoveEvent);
   event.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(true, removeDone);
   EXPECT_EQ(false, errorDone);
}

} // namespace
