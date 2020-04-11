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
   Connection
   (
      const cbee::EPoller& e,
      const int fd,
      cbee::Event::RemoveFunc remove
   ) :
      epoll(e),
      socket(fd),
      handleRemove(remove),
      event
      (
         socket,
         [this](){handleRead();},
         nullptr,
         handleRemove,
         nullptr
      )
   {
      socket.setNonBlock();
      event.enableReadEvent();
   }

   void updateEvent()
   {
      epoll.updateEvent(socket, &event);
   }

   void deleteEvent()
   {
      epoll.deleteEvent(socket);
   }

   const cbee::Socket& getSocket() const
   {
      return socket;
   }

private:
   Connection(const Connection&) = delete;
   Connection& operator=(const Connection&) = delete;

private:
   void handleRead()
   {
      char actualBuffer[1024] = {0};
      int actualSize = socket.read(actualBuffer, sizeof(actualBuffer));
      if (actualSize == 0)
      {
         if(handleRemove) handleRemove(socket.getFd());
         return;
      }
      EXPECT_EQ(sizeof(expectedBuffer), actualSize);
      EXPECT_STREQ(expectedBuffer, actualBuffer);
   }

private:
   const cbee::EPoller& epoll;
   cbee::Socket socket;
   cbee::Event::RemoveFunc handleRemove;
   cbee::Event event;
};

class EPollerTest : public ::testing::Test
{
public:
   EPollerTest() :
      serverEvent
      (
         serverSocket,
         [this](){handleRead();},
         nullptr,
         nullptr,
         nullptr
      )
   {
      serverSocket.setNonBlock();
      serverEvent.enableReadEvent();
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
      EXPECT_EQ(nullptr, connection.get());
   }

   cbee::EPoller epoll;
   cbee::Socket serverSocket;
   cbee::Event serverEvent;

   cbee::Sockaddr serverAddr;
   std::unique_ptr<Connection> connection;

private:
   void handleRead()
   {
      cbee::Sockaddr connectAddr;
      int connectionFd = serverSocket.accept(&connectAddr);
      EXPECT_STREQ("127.0.0.1", connectAddr.getIp().c_str());

      connection = std::make_unique<Connection>(epoll, connectionFd, [this](int fdKey){removeConnection(fdKey);});
      connection->updateEvent();
   }

   void removeConnection(int fdKey)
   {
      EXPECT_EQ(fdKey, connection->getSocket().getFd());
      connection->deleteEvent();
      connection.reset();
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
