#include "EPoller.h"
#include "Event.h"
#include "Sockaddr.h"
#include "Socket.h"

#include <gtest/gtest.h>
#include <thread>

namespace
{

static const char expectedBuffer[] = "hello";

class EPollerTest : public ::testing::Test
{
public:
   EPollerTest()
   {
      serverSocket.open();
      serverSocket.setNonBlock();

      setConnectionEvent();
      setServerEvent();
   }

   ~EPollerTest()
   {
      serverSocket.close();
   }

   void serve()
   {
      serverAddr.setInetFamily();
      serverAddr.setIp(false);
      serverAddr.setPort(8181);

      serverSocket.bind(serverAddr);
      serverSocket.listen();
      epoll.updateEvent(serverSocket, &serverEvent);

      auto connenctEvent = epoll.poll(-1).front();
      EXPECT_EQ(EPOLLIN, connenctEvent->getActiveEvents());
      connenctEvent->handleEvent();

      auto readEvent = epoll.poll(-1).front();
      EXPECT_EQ(EPOLLIN, readEvent->getActiveEvents());
      readEvent->handleEvent();

      auto closeEvent = epoll.poll(-1).front();
      // peer connection was closed, which causes EPOLLIN + EPOLLRDHUP
      EXPECT_EQ(EPOLLIN | EPOLLRDHUP, closeEvent->getActiveEvents());
      closeEvent->handleEvent();
   }

   cbee::EPoller epoll;

   cbee::Socket serverSocket;
   cbee::Event serverEvent;

   cbee::Socket connectionSocket;
   cbee::Event coneectionEvent;

   cbee::Sockaddr serverAddr;
   cbee::Sockaddr connectAddr;

private:
   void setConnectionEvent()
   {
      coneectionEvent.setReadCallback([&]() {
         char actualBuffer[1024] = {0}; 
         int actualSize = connectionSocket.read(actualBuffer, sizeof(actualBuffer));
         if (actualSize == 0)
         {
            connectionSocket.close();
            return;
         }
         EXPECT_EQ(sizeof(expectedBuffer), actualSize);
         EXPECT_STREQ(expectedBuffer, actualBuffer);
      });
      coneectionEvent.enableReadEvent();
   }

   void setServerEvent()
   {
      serverEvent.setReadCallback([&]() {
         connectionSocket = serverSocket.accept(&connectAddr);
         connectionSocket.setNonBlock();
         EXPECT_STREQ("127.0.0.1", connectAddr.getIp().c_str());
         epoll.updateEvent(connectionSocket, &coneectionEvent);
      });
      serverEvent.enableReadEvent();
   }
};

TEST_F(EPollerTest, constructor)
{
   std::thread serveThread(&EPollerTest::serve, this);

   // Delay for the connection to make sure server is ready.
   std::this_thread::sleep_for(std::chrono::milliseconds(50));
   cbee::Socket clientSocket;
   clientSocket.open();
   clientSocket.connect(serverAddr);
   
   clientSocket.write(expectedBuffer, sizeof(expectedBuffer));

   // Delay to make sure data has been read by the server.
   std::this_thread::sleep_for(std::chrono::milliseconds(50));
   clientSocket.close();

   serveThread.join();
}

} // namespace
