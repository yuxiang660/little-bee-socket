#pragma once

#include "Sockaddr.h"

#include <netinet/in.h>
#include <sys/socket.h>

namespace cbee
{

class SocketFd
{
public:
   SocketFd();
   explicit SocketFd(int socketFd);
   ~SocketFd();

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

   Sockaddr getLocalAddr();
   Sockaddr getPeerAddr();

private:
   SocketFd(const SocketFd&) = delete;
   SocketFd& operator=(const SocketFd&) = delete;

private:
   struct sockaddr* sockaddrCast(Sockaddr* addr);
   const struct sockaddr* sockaddrCast(const Sockaddr* addr);

private:
   const int fd;
};

}
