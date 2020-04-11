#include "SocketFd.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>

namespace cbee
{

SocketFd::SocketFd() : fd(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
   assert(fd >= 0);
}

SocketFd::SocketFd(const int socketFd) : fd(socketFd)
{
   assert(fd >= 0);
}

SocketFd::~SocketFd()
{
   __attribute__((unused)) auto ret = ::close(fd);
   assert(ret == 0);
}

void SocketFd::shutdownRead()
{
   if (::shutdown(fd, SHUT_RD) < 0)
   {
      perror("socket shutdown read failure");
      exit(EXIT_FAILURE);
   }
}

void SocketFd::shutdownWrite()
{
   if (::shutdown(fd, SHUT_WR) < 0)
   {
      perror("socket shutdown write failure");
      exit(EXIT_FAILURE);
   }
}

void SocketFd::shutdown()
{
   if (::shutdown(fd, SHUT_RDWR) < 0)
   {
      perror("socket shutdown read and write failure");
      exit(EXIT_FAILURE);
   }
}

void SocketFd::setNonBlock()
{
   int flags = ::fcntl(fd, F_GETFL, 0);
   flags |= O_NONBLOCK;
   if(::fcntl(fd, F_SETFL, flags))
   {
      perror("socket config failure");
      exit(EXIT_FAILURE);
   }

   flags = ::fcntl(fd, F_GETFD, 0);
   flags |= FD_CLOEXEC;
   if(::fcntl(fd, F_SETFD, flags))
   {
      perror("socket config failure");
      exit(EXIT_FAILURE);
   }
}

void SocketFd::bind(const Sockaddr& serverAddr)
{
   if (::bind(fd, sockaddrCast(&serverAddr), sizeof(Sockaddr)) < 0)
   {
      perror("bind failure");
      exit(EXIT_FAILURE);
   }
}

void SocketFd::listen()
{
   if (::listen(fd, SOMAXCONN) < 0)
   {
      perror("listen failure");
      exit(EXIT_FAILURE);
   }
}

int SocketFd::accept(Sockaddr* connectAddr)
{
   assert(connectAddr != nullptr);
   socklen_t addrlen = static_cast<socklen_t>(sizeof(Sockaddr));
   int connfd = ::accept(fd, sockaddrCast(connectAddr), &addrlen);
   if (connfd < 0)
   {
      perror("accept failure");
      exit(EXIT_FAILURE);
   }
   return connfd;
}

void SocketFd::connect(const Sockaddr& serverAddr)
{
   if (::connect(fd, sockaddrCast(&serverAddr), static_cast<socklen_t>(sizeof(Sockaddr))) < 0)
   {
      perror("connect failure");
      exit(EXIT_FAILURE);
   }
}

int SocketFd::getFd() const
{
   return fd;
}

int SocketFd::read(void* buf, int count)
{
   return static_cast<int>(::read(fd, buf, count));
}

int SocketFd::write(const void* buf, int count)
{
   return static_cast<int>(::write(fd, buf, count));
}

Sockaddr SocketFd::getLocalAddr()
{
   Sockaddr localaddr;
   socklen_t addrlen = static_cast<socklen_t>(sizeof(Sockaddr));
   if (::getsockname(fd, sockaddrCast(&localaddr), &addrlen) < 0)
   {
      perror("getLocalAddr failure");
      exit(EXIT_FAILURE);
   }
   return localaddr;
}

Sockaddr SocketFd::getPeerAddr()
{
   Sockaddr peeraddr;
   socklen_t addrlen = static_cast<socklen_t>(sizeof(Sockaddr));
   if (::getpeername(fd, sockaddrCast(&peeraddr), &addrlen) < 0)
   {
      perror("getPeerAddr failure");
      exit(EXIT_FAILURE);
   }
   return peeraddr;
}

struct sockaddr* SocketFd::sockaddrCast(Sockaddr* addr)
{
   return reinterpret_cast<struct sockaddr*>(addr);
}

const struct sockaddr* SocketFd::sockaddrCast(const Sockaddr* addr)
{
   return reinterpret_cast<const struct sockaddr*>(addr);
}

} // namespace cbee
