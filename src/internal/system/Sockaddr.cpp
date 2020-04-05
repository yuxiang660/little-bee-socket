#include "Sockaddr.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace cbee
{

Sockaddr::Sockaddr()
{
   memset(&address, 0, sizeof(address));
}

void Sockaddr::setInetFamily()
{
   address.sin_family = AF_INET;
}

void Sockaddr::setIp(const char* ip)
{
   if (::inet_pton(AF_INET, ip, &address.sin_addr) <= 0)
   {
      perror("ip address is invalid");
      exit(EXIT_FAILURE);
   }
}

void Sockaddr::setIp(bool loopbackOnly)
{
   in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
   address.sin_addr.s_addr = htobe32(ip);
}

void Sockaddr::setPort(uint16_t port)
{
   address.sin_port = htobe16(port);
}

std::string Sockaddr::getIp() const
{
   char buf[INET_ADDRSTRLEN];
   auto ret = ::inet_ntop(AF_INET, &address.sin_addr, buf, INET_ADDRSTRLEN);
   assert(ret != nullptr);
   return std::string(buf);
}

uint16_t Sockaddr::getPort() const
{
   return be16toh(address.sin_port);
}

} // namespace cbee
