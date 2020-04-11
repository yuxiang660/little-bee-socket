#include "Socket.h"
#include "Sockaddr.h"

#include <gtest/gtest.h>
#include <chrono>
#include <thread>

namespace
{

class SocketTest : public testing::Test
{
public:
   void serve()
   {
      serverAddr.setInetFamily();
      serverAddr.setIp(false);
      serverAddr.setPort(8080);

      serverSocket.bind(serverAddr);
      serverSocket.listen();
      cbee::Sockaddr connectAddr;
      cbee::Socket connectionSocket (serverSocket.accept(&connectAddr));
      
      EXPECT_STREQ("127.0.0.1", connectionSocket.getLocalAddr().getIp().c_str());
      EXPECT_EQ(serverSocket.getLocalAddr().getPort(), connectionSocket.getLocalAddr().getPort());

      EXPECT_STREQ(connectAddr.getIp().c_str(), connectionSocket.getPeerAddr().getIp().c_str());
      EXPECT_EQ(connectAddr.getPort(), connectionSocket.getPeerAddr().getPort());

      EXPECT_STREQ(clientSocket.getLocalAddr().getIp().c_str(), connectionSocket.getPeerAddr().getIp().c_str());
      EXPECT_EQ(clientSocket.getLocalAddr().getPort(), connectionSocket.getPeerAddr().getPort());
   }

   void connect()
   {
      clientSocket.connect(serverAddr);
   }

   cbee::Socket serverSocket;
   cbee::Socket clientSocket;
   cbee::Sockaddr serverAddr;
};

TEST_F(SocketTest, serveInSubThread_connectInMainThread_getExpectedConnectAddress)
{
   std::thread serveThread(&SocketTest::serve, this);

   // Delay for the connection to make sure server is ready.
   std::this_thread::sleep_for(std::chrono::milliseconds(50));
   connect();

   serveThread.join();
}

} // namespace
