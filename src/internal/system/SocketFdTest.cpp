#include "SocketFd.h"
#include "Sockaddr.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>

namespace
{

class SocketFdTest : public testing::Test
{
public:
   SocketFdTest() :
      serverAddr(8080)
   {}

   void serve()
   {
      serverSocket.bind(serverAddr);
      serverSocket.listen();
      cbee::Sockaddr connectAddr;
      cbee::SocketFd connectionSocket (serverSocket.accept(&connectAddr));
      
      EXPECT_STREQ("127.0.0.1", connectionSocket.getLocalAddr().getIp().c_str());
      EXPECT_STREQ("127.0.0.1:8080", connectionSocket.getLocalAddr().getIpPort().c_str());

      EXPECT_STREQ("127.0.0.1", connectionSocket.getPeerAddr().getIp().c_str());
      EXPECT_STREQ(connectAddr.getIpPort().c_str(), connectionSocket.getPeerAddr().getIpPort().c_str());
   }

   void connect()
   {
      clientSocket.connect(serverAddr);
   }

   cbee::SocketFd serverSocket;
   cbee::SocketFd clientSocket;
   cbee::Sockaddr serverAddr;
};

TEST_F(SocketFdTest, serveInSubThread_connectInMainThread_getExpectedConnectAddress)
{
   std::thread serveThread(&SocketFdTest::serve, this);

   // Delay for the connection to make sure server is ready.
   std::this_thread::sleep_for(std::chrono::milliseconds(50));
   connect();

   serveThread.join();
}

} // namespace
