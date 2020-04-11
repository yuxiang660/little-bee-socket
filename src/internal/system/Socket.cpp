#include "Socket.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>

namespace cbee
{

Socket::Socket() : fd(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
   assert(fd >= 0);
}

Socket::Socket(const int socketFd) : fd(socketFd)
{
   assert(fd >= 0);
}

Socket::~Socket()
{
   __attribute__((unused)) auto ret = ::close(fd);
   assert(ret == 0);
}

void Socket::shutdownRead()
{
   if (::shutdown(fd, SHUT_RD) < 0)
   {
      perror("socket shutdown read failure");
      exit(EXIT_FAILURE);
   }
}

void Socket::shutdownWrite()
{
   if (::shutdown(fd, SHUT_WR) < 0)
   {
      perror("socket shutdown write failure");
      exit(EXIT_FAILURE);
   }
}

void Socket::shutdown()
{
   if (::shutdown(fd, SHUT_RDWR) < 0)
   {
      perror("socket shutdown read and write failure");
      exit(EXIT_FAILURE);
   }
}

void Socket::setNonBlock()
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

void Socket::bind(const Sockaddr& serverAddr)
{
   if (::bind(fd, sockaddrCast(&serverAddr), sizeof(Sockaddr)) < 0)
   {
      perror("bind failure");
      exit(EXIT_FAILURE);
   }
}

void Socket::listen()
{
   if (::listen(fd, SOMAXCONN) < 0)
   {
      perror("listen failure");
      exit(EXIT_FAILURE);
   }
}

int Socket::accept(Sockaddr* connectAddr)
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

void Socket::connect(const Sockaddr& serverAddr)
{
   if (::connect(fd, sockaddrCast(&serverAddr), static_cast<socklen_t>(sizeof(Sockaddr))) < 0)
   {
      perror("connect failure");
      exit(EXIT_FAILURE);
   }
}

int Socket::getFd() const
{
   return fd;
}

int Socket::read(void* buf, int count)
{
   return static_cast<int>(::read(fd, buf, count));
}

int Socket::write(const void* buf, int count)
{
   return static_cast<int>(::write(fd, buf, count));
}

Sockaddr Socket::getLocalAddr()
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

Sockaddr Socket::getPeerAddr()
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

struct sockaddr* Socket::sockaddrCast(Sockaddr* addr)
{
   return reinterpret_cast<struct sockaddr*>(addr);
}

const struct sockaddr* Socket::sockaddrCast(const Sockaddr* addr)
{
   return reinterpret_cast<const struct sockaddr*>(addr);
}

} // namespace cbee
