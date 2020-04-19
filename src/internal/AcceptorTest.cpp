#include "Acceptor.h"
#include "internal/system/Sockaddr.h"

#include <gtest/gtest.h>
#include <memory>
#include <thread>

namespace
{

struct AcceptorTest : public ::testing::Test
{
   AcceptorTest() :
      io(),
      listenAddr(1234),
      accept(io, [this](int fd){newConnection(fd);}, listenAddr)
   {
   }

   void newConnection(int fd)
   {
      connectionSocket = std::make_unique<cbee::SocketFd>(fd);
   }

   cbee::IoManager io;
   cbee::Sockaddr listenAddr;
   cbee::Acceptor accept;
   std::unique_ptr<cbee::SocketFd> connectionSocket;
};

TEST_F(AcceptorTest, constructor_registerAcceptorInLoop_createNewConnection)
{
   auto test = [&]()
   {
      cbee::SocketFd clientSocket;
      clientSocket.connect(listenAddr);
      // Delay for the creation of new connection
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      io.quit();
   };
   std::thread testThread(test);

   io.loop();

   testThread.join();

   EXPECT_NE(connectionSocket.get(), nullptr);
   EXPECT_STREQ("127.0.0.1:1234", connectionSocket->getLocalAddr().getIpPort().c_str());
}

}
