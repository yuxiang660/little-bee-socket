#pragma once

#include <netinet/in.h>
#include <string>

namespace cbee
{

class Sockaddr
{
public:
   Sockaddr();
   void setInetFamily();
   void setIp(const char* ip);
   void setIp(bool loopbackOnly);
   std::string getIp() const;

   void setPort(uint16_t port);
   uint16_t getPort() const;

private:
   Sockaddr(const Sockaddr&) = delete;
   Sockaddr& operator=(const Sockaddr&) = delete;

private:
   struct sockaddr_in address;
};

} // namespace cbee
