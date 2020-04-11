#include "SocketEvent.h"
#include "Socket.h"

#include <gtest/gtest.h>
#include <sys/epoll.h>

namespace
{

class SocketEventTest : public ::testing::Test
{
public:
   SocketEventTest() :
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

   cbee::Socket socket;
   cbee::SocketEvent event;
   bool readDone;
   bool writeDone;
   bool removeDone;
   bool errorDone;
   int removedFd;
};

TEST_F(SocketEventTest, readEvent_expectedCallBack)
{
   event.setActiveEvents(cbee::SocketEvent::kConnectedOrReadableOrCloseEvent);
   event.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(true, readDone);
   EXPECT_EQ(false, removeDone);
   EXPECT_EQ(false, errorDone);
}

TEST_F(SocketEventTest, writeEvent_expectedCallBack)
{
   event.setActiveEvents(cbee::SocketEvent::kWritableEvent);
   event.handleEvent();
   EXPECT_EQ(true, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(false, removeDone);
   EXPECT_EQ(false, errorDone);
}

TEST_F(SocketEventTest, errorEvent_expectedCallBack)
{
   event.setActiveEvents(cbee::SocketEvent::kErrorEvent);
   event.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(false, removeDone);
   EXPECT_EQ(true, errorDone);
}

TEST_F(SocketEventTest, removeEvent_expectedCallBack)
{
   event.setActiveEvents(cbee::SocketEvent::kRemoveEvent);
   event.handleEvent();
   EXPECT_EQ(false, writeDone);
   EXPECT_EQ(false, readDone);
   EXPECT_EQ(true, removeDone);
   EXPECT_EQ(false, errorDone);
   EXPECT_EQ(socket.getFd(), removedFd);
}

} // namespace
