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
      socket(),
      event
      (
         socket,
         [this]() { readDone = true; },
         [this]() { writeDone = true; },
         [this](int fdKey) { removedFd = fdKey; removeDone = true; },
         [this]() { errorDone = true; }
      ),
      readDone(false),
      writeDone(false),
      removeDone(false),
      errorDone(false),
      removedFd(-1)
   {
   }

   ~EventTest()
   {
   }

   cbee::Socket socket;
   cbee::Event event;
   bool readDone;
   bool writeDone;
   bool removeDone;
   bool errorDone;
   int removedFd;
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

TEST_F(EventTest, removeEvent_expectedCallBack)
{
   event.setActiveEvents(cbee::Event::kRemoveEvent);
   event.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(true, removeDone);
   EXPECT_EQ(false, errorDone);
   EXPECT_EQ(socket.getFd(), removedFd);
}

} // namespace
