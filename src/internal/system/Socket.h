#pragma once

#include "Sockaddr.h"

#include <netinet/in.h>
#include <sys/socket.h>

namespace cbee
{

class Socket
{
public:
   Socket();
   Socket(const int socketFd);

   void bind(const Sockaddr& serverAddr);
   void listen();
   Socket accept(Sockaddr* connectAddr);
   void connect(const Sockaddr& serverAddr);

private:
   struct sockaddr* sockaddrCast(Sockaddr* addr);
   const struct sockaddr* sockaddrCast(const Sockaddr* addr);

private:
   const int fd;
};

}
