#pragma once

#include <netinet/in.h>
#include <string>

namespace cbee
{

class Sockaddr
{
public:
   Sockaddr();
   Sockaddr(uint16_t port, bool loopbackOnly = false);
   Sockaddr(const char* ip, uint16_t port);
   std::string getIp() const;
   std::string getIpPort() const;

private:
   void setInetFamily();
   void setIp(const char* ip);
   void setIp(bool loopbackOnly);
   void setPort(uint16_t port);

private:
   struct sockaddr_in address;
};

} // namespace cbee
