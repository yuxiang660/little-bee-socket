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
   Socket(int socketFd);
   ~Socket();

   void shutdownRead();
   void shutdownWrite();
   void shutdown();

   void setNonBlock();
   void bind(const Sockaddr& serverAddr);
   void listen();
   int accept(Sockaddr* connectAddr);
   void connect(const Sockaddr& serverAddr);
   int getFd() const;

   int read(void *buf, int count);
   int write(const void *buf, int count);

private:
   Socket(const Socket&) = delete;
   Socket& operator=(const Socket&) = delete;

private:
   struct sockaddr* sockaddrCast(Sockaddr* addr);
   const struct sockaddr* sockaddrCast(const Sockaddr* addr);

private:
   const int fd;
};

}
