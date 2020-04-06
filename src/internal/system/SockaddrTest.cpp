#include "Sockaddr.h"

#include <netinet/in.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <string>

namespace
{

TEST(SockaddrTest, constructor_memoryZeroOut)
{
   constexpr int memoryLength = sizeof(struct sockaddr_in);
   unsigned char tester[memoryLength] = {};
   cbee::Sockaddr addr;

   EXPECT_EQ(memcmp(&addr, tester, memoryLength), 0);
}

TEST(SockaddrTest, setIp_validIp_getExpectedIp)
{
   const char* expectedIp = "10.11.12.13";

   cbee::Sockaddr addr;
   addr.setIp(expectedIp);

   EXPECT_STREQ(expectedIp, addr.getIp().c_str());
}

TEST(SockaddrTest, setPort_validPort_getExpectedPort)
{
   uint16_t expectedPort = 8181;
   cbee::Sockaddr addr;
   addr.setPort(expectedPort);
   EXPECT_EQ(expectedPort, addr.getPort());
}

struct SockaddrSetIpTestCase
{
   bool loopbackOnly;
   std::string expectedIp;
};

SockaddrSetIpTestCase sockaddrSetIpTestCases[] = {{true, "127.0.0.1"}, {false, "0.0.0.0"}};

class SockaddrSetIpTest : public ::testing::TestWithParam<SockaddrSetIpTestCase>
{
protected:
   cbee::Sockaddr addr;
};

TEST_P(SockaddrSetIpTest, setIp_loopback_getExpectedIp)
{
   auto testCase = GetParam();
   addr.setIp(testCase.loopbackOnly);
   EXPECT_STREQ(testCase.expectedIp.c_str(), addr.getIp().c_str());
}

INSTANTIATE_TEST_CASE_P(SockaddrSetIpTest, SockaddrSetIpTest, ::testing::ValuesIn(sockaddrSetIpTestCases));

TEST(SockaddrErrorTest, setIp_inValidIp_exit)
{
   const char* invalidIp = "266.0.0.290";

   cbee::Sockaddr addr;
   EXPECT_EXIT(addr.setIp(invalidIp), ::testing::ExitedWithCode(EXIT_FAILURE), "ip address is invalid");
}

} // namespace
