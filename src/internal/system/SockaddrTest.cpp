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
   auto tempAddr = std::make_unique<cbee::Sockaddr>("10.11.12.13", 8080);

   cbee::Sockaddr copyAddr(*tempAddr);
   tempAddr.reset();

   EXPECT_EQ(nullptr, tempAddr.get());
   EXPECT_STREQ("10.11.12.13", copyAddr.getIp().c_str());
   EXPECT_STREQ("10.11.12.13:8080", copyAddr.getIpPort().c_str());
}

TEST(SockaddrTest, assignOperation_assignNewAddr_expectedValue)
{
   cbee::Sockaddr expectedAddr("10.11.12.13", 8080);
   cbee::Sockaddr addr("101.111.112.113", 8181);

   EXPECT_STRNE("10.11.12.13", addr.getIp().c_str());
   EXPECT_STRNE("10.11.12.13:8080", addr.getIpPort().c_str());

   addr = expectedAddr;
   EXPECT_STREQ("10.11.12.13", addr.getIp().c_str());
   EXPECT_STREQ("10.11.12.13:8080", addr.getIpPort().c_str());
}

struct SockaddrSetIpTestCase
{
   bool loopbackOnly;
   std::string expectedIp;
};

SockaddrSetIpTestCase sockaddrSetIpTestCases[] = {{true, "127.0.0.1"}, {false, "0.0.0.0"}};

class SockaddrSetIpTest : public ::testing::TestWithParam<SockaddrSetIpTestCase>
{
};

TEST_P(SockaddrSetIpTest, setIp_loopback_getExpectedIp)
{
   auto testCase = GetParam();

   cbee::Sockaddr addr(8080, testCase.loopbackOnly);

   EXPECT_STREQ(testCase.expectedIp.c_str(), addr.getIp().c_str());
}

INSTANTIATE_TEST_CASE_P(SockaddrSetIpTest, SockaddrSetIpTest, ::testing::ValuesIn(sockaddrSetIpTestCases));

TEST(SockaddrErrorTest, setIp_inValidIp_exit)
{
   EXPECT_EXIT(cbee::Sockaddr addr("266.0.0.290", 8080), ::testing::ExitedWithCode(EXIT_FAILURE), "Sockaddr::setIp failure");
}

} // namespace
