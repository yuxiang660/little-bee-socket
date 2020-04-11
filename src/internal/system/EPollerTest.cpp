#include "EPoller.h"
#include "Event.h"
#include "Sockaddr.h"
#include "Socket.h"

#include <gtest/gtest.h>
#include <memory>
#include <thread>

namespace
{

static const char expectedBuffer[] = "hello";

class Connection
{
public:
   Connection(const int fd) : socket(fd), event(socket)
   {
      setEvent();
   }

   ~Connection()
   {
   }

   cbee::EventHandler getEvent()
   {
      return &event;
   }

   const cbee::Socket& getSocket()
   {
      return socket;
   }

   void setNonBlock()
   {
      socket.setNonBlock();
   }

private:
   Connection(const Connection&) = delete;
   Connection& operator=(const Connection&) = delete;

private:
   void setEvent()
   {
      event.setReadCallback([&]() {
         char actualBuffer[1024] = {0};
         int actualSize = socket.read(actualBuffer, sizeof(actualBuffer));
         if (actualSize == 0)
         {
            // socket.close();
            return;
         }
         EXPECT_EQ(sizeof(expectedBuffer), actualSize);
         EXPECT_STREQ(expectedBuffer, actualBuffer);
      });
      event.enableReadEvent();
   }

private:
   cbee::Socket socket;
   cbee::Event event;
};

class EPollerTest : public ::testing::Test
{
public:
   EPollerTest() : serverEvent(serverSocket)
   {
      serverSocket.setNonBlock();
      setServerEvent();
   }

   ~EPollerTest()
   {
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

      auto removeEvent = epoll.poll(-1).front();
      // peer connection was closed, which causes EPOLLIN + EPOLLRDHUP
      EXPECT_EQ(EPOLLIN | EPOLLRDHUP, removeEvent->getActiveEvents());
      removeEvent->handleEvent();
   }

   cbee::EPoller epoll;

   cbee::Socket serverSocket;
   cbee::Event serverEvent;
   cbee::Sockaddr serverAddr;
   std::unique_ptr<Connection> connection;

private:
   void setServerEvent()
   {
      serverEvent.setReadCallback([&]() {
         cbee::Sockaddr connectAddr;
         int connectionFd = serverSocket.accept(&connectAddr);
         EXPECT_STREQ("127.0.0.1", connectAddr.getIp().c_str());

         connection = std::make_unique<Connection>(connectionFd);
         connection->setNonBlock();

         epoll.updateEvent(connection->getSocket(), connection->getEvent());
      });
      serverEvent.enableReadEvent();
   }
};

TEST_F(EPollerTest, constructor)
{
   std::thread serveThread(&EPollerTest::serve, this);

   {
      // Delay for the connection to make sure server is ready.
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
   
      // Move clientSocket to internal namespace, so the socket can destruct automatically.
      cbee::Socket clientSocket;
      clientSocket.connect(serverAddr);
      clientSocket.write(expectedBuffer, sizeof(expectedBuffer));

      // Delay to make sure data has been read by the server.
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
   }

   serveThread.join();
}

} // namespace
