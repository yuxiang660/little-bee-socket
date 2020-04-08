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
   SocketTest()
   {
      serverAddr.setInetFamily();
      serverAddr.setIp(false);
      serverAddr.setPort(8080);
   }

   void serve()
   {
      serverSocket.bind(&serverAddr);
      serverSocket.listen();
      serverSocket.accept(&connectAddr);
   }

   void connect()
   {
      clientSocket.connect(&serverAddr);
   }

   cbee::Socket serverSocket;
   cbee::Socket clientSocket;
   cbee::Sockaddr serverAddr;
   cbee::Sockaddr connectAddr;
};

TEST_F(SocketTest, serveInSubThread_connectInMainThread_getExpectedConnectAddress)
{
   std::thread serveThread(&SocketTest::serve, this);

   // Delay for the connection to make sure server is ready.
   std::this_thread::sleep_for(std::chrono::milliseconds(50));
   connect();

   serveThread.join();

   EXPECT_STREQ("127.0.0.1", connectAddr.getIp().c_str());
   EXPECT_NE(0, connectAddr.getPort());
}

} // namespace
