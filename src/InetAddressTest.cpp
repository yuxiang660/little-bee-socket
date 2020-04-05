#include "InetAddress.h"

#include <gtest/gtest.h>
#include <string>

namespace
{

TEST(InetAddressTest, constructor_withIpAndPort_getExpectedIpAndPort)
{
   const char* expectedIp = "10.255.255.123";
   uint16_t port = 81;
   const char* expectedIpPort = "10.255.255.123:81";
   cbee::InetAddress addr(expectedIp, port);

   EXPECT_STREQ(expectedIp, addr.getIp().c_str());
   EXPECT_STREQ(expectedIpPort, addr.getIpPort().c_str());
}

struct InetAddressOnlyPortTestCase
{
   uint16_t port;
   bool loopbackOnly;
   std::string expectedIp;
   std::string expectedIpPort;
};

InetAddressOnlyPortTestCase inetAddressOnlyPortTestCase[] = {{8181, true, "127.0.0.1", "127.0.0.1:8181"},
                                                             {8181, false, "0.0.0.0", "0.0.0.0:8181"}};

class InetAddressOnlyPortTest : public ::testing::TestWithParam<InetAddressOnlyPortTestCase>
{
};

TEST_P(InetAddressOnlyPortTest, constructor_getExpectedIpAndPort)
{
   auto testCase = GetParam();

   cbee::InetAddress addr(testCase.port, testCase.loopbackOnly);
   EXPECT_STREQ(testCase.expectedIp.c_str(), addr.getIp().c_str());
   EXPECT_STREQ(testCase.expectedIpPort.c_str(), addr.getIpPort().c_str());
}

INSTANTIATE_TEST_CASE_P(InetAddressOnlyPortTest, InetAddressOnlyPortTest,
                        ::testing::ValuesIn(inetAddressOnlyPortTestCase));

} // namespace
