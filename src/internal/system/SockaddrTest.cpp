#include "Sockaddr.h"

#include <netinet/in.h>
#include <gtest/gtest.h>
#include <memory>
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

TEST(SockaddrTest, copyConstructor_expectedCopy)
{
   const char* expectedIp = "10.11.12.13";
   const uint16_t expectedPort = 8080;
   auto tempAddr = std::make_unique<cbee::Sockaddr>();
   tempAddr->setIp(expectedIp);
   tempAddr->setPort(expectedPort);

   cbee::Sockaddr copyAddr(*tempAddr);
   tempAddr.reset();

   EXPECT_EQ(nullptr, tempAddr.get());
   EXPECT_STREQ(expectedIp, copyAddr.getIp().c_str());
   EXPECT_EQ(expectedPort, copyAddr.getPort());
}

TEST(SockaddrTest, assignOperation_assignNewAddr_expectedValue)
{
   const char* expectedIp = "10.11.12.13";
   const uint16_t expectedPort = 8080;
   cbee::Sockaddr expectedAddr;
   expectedAddr.setIp(expectedIp);
   expectedAddr.setPort(expectedPort);

   cbee::Sockaddr addr;
   addr.setIp("101.111.112.113");
   addr.setPort(8181);

   EXPECT_STRNE(expectedIp, addr.getIp().c_str());
   EXPECT_NE(expectedPort, addr.getPort());

   addr = expectedAddr;
   EXPECT_STREQ(expectedIp, addr.getIp().c_str());
   EXPECT_EQ(expectedPort, addr.getPort());
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
   EXPECT_EXIT(addr.setIp(invalidIp), ::testing::ExitedWithCode(EXIT_FAILURE), "Sockaddr::setIp failure");
}

} // namespace
