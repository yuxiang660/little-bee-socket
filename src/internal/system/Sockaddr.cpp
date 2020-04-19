#include "Sockaddr.h"
#include "Macros.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <endian.h>
#include <string.h>

namespace cbee
{

Sockaddr::Sockaddr()
{
   memset(&address, 0, sizeof(address));
}

Sockaddr::Sockaddr(uint16_t port, bool loopbackOnly)
{
   memset(&address, 0, sizeof(address));
   setInetFamily();
   setIp(loopbackOnly);
   setPort(port);
}

Sockaddr::Sockaddr(const char* ip, uint16_t port)
{
   memset(&address, 0, sizeof(address));
   setInetFamily();
   setIp(ip);
   setPort(port);
}

std::string Sockaddr::getIp() const
{
   char buf[INET_ADDRSTRLEN];
   __attribute__((unused)) auto ret = ::inet_ntop(AF_INET, &address.sin_addr, buf, INET_ADDRSTRLEN);
   assert(ret != nullptr);
   return std::string(buf);
}

std::string Sockaddr::getIpPort() const
{
   return getIp() + ":" + std::to_string(be16toh(address.sin_port));
}

void Sockaddr::setInetFamily()
{
   address.sin_family = AF_INET;
}

void Sockaddr::setIp(const char* ip)
{
   if (::inet_pton(AF_INET, ip, &address.sin_addr) <= 0) HANDLE_ERROR("Sockaddr::setIp failure");
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

} // namespace cbee
