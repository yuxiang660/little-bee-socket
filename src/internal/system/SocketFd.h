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

   void shutdownRead() const;
   void shutdownWrite() const;
   void shutdown() const;

   void setNonBlock() const;
   void bind(const Sockaddr& serverAddr) const;
   void listen() const;
   int accept(Sockaddr* connectAddr) const;
   void connect(const Sockaddr& serverAddr) const;
   int getFd() const;

   int read(void *buf, int count) const;
   int write(const void *buf, int count) const;

   Sockaddr getLocalAddr() const;
   Sockaddr getPeerAddr() const;

private:
   SocketFd(const SocketFd&) = delete;
   SocketFd& operator=(const SocketFd&) = delete;

private:
   struct sockaddr* sockaddrCast(Sockaddr* addr) const;
   const struct sockaddr* sockaddrCast(const Sockaddr* addr) const;

private:
   const int fd;
};

}
