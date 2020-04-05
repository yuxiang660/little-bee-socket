#pragma once

#include "internal/system/Sockaddr.h"

#include <string>

namespace cbee
{

class InetAddress
{
public:
   InetAddress(uint16_t port, bool loopbackOnly = false);
   InetAddress(const char* ip, uint16_t port);
   std::string getIp() const;
   std::string getIpPort() const;

private:
   InetAddress(const InetAddress&) = delete;
   InetAddress& operator=(const InetAddress&) = delete;

private:
   Sockaddr address;
};

} // namespace cbee