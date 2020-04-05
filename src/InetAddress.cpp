#include "InetAddress.h"

namespace cbee
{

InetAddress::InetAddress(uint16_t port, bool loopbackOnly)
{
   address.setInetFamily();
   address.setIp(loopbackOnly);
   address.setPort(port);
}

InetAddress::InetAddress(const char* ip, uint16_t port)
{
   address.setInetFamily();
   address.setIp(ip);
   address.setPort(port);
}

std::string InetAddress::getIp() const
{
   return address.getIp();
}

std::string InetAddress::getIpPort() const
{
   return address.getIp() + ":" + std::to_string(address.getPort());
}

} // namespace cbee