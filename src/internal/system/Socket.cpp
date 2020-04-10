#include "Socket.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>

namespace cbee
{

Socket::Socket() : fd(-1)
{
}

Socket::Socket(const int socketFd) : fd(socketFd)
{
   assert(fd >= 0);
}

void Socket::open()
{
   fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (fd < 0)
   {
      perror("socket open failure");
      exit(EXIT_FAILURE);
   }
}

void Socket::close()
{
   assert(fd >= 0);
   if(::close(fd) < 0)
   {
      perror("socket close failure");
      exit(EXIT_FAILURE);
   }
}

void Socket::shutdownRead()
{
   assert(fd >= 0);
   if (::shutdown(fd, SHUT_RD) < 0)
   {
      perror("socket shutdown read failure");
      exit(EXIT_FAILURE);
   }
}

void Socket::shutdownWrite()
{
   assert(fd >= 0);
   if (::shutdown(fd, SHUT_WR) < 0)
   {
      perror("socket shutdown write failure");
      exit(EXIT_FAILURE);
   }
}

void Socket::shutdown()
{
   assert(fd >= 0);
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
   assert(fd >= 0);
   if (::bind(fd, sockaddrCast(&serverAddr), sizeof(Sockaddr)) < 0)
   {
      perror("bind failure");
      exit(EXIT_FAILURE);
   }
}

void Socket::listen()
{
   assert(fd >= 0);
   if (::listen(fd, SOMAXCONN) < 0)
   {
      perror("listen failure");
      exit(EXIT_FAILURE);
   }
}

Socket Socket::accept(Sockaddr* connectAddr)
{
   assert(fd >= 0);
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
   assert(fd >= 0);
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

struct sockaddr* Socket::sockaddrCast(Sockaddr* addr)
{
   return reinterpret_cast<struct sockaddr*>(addr);
}

const struct sockaddr* Socket::sockaddrCast(const Sockaddr* addr)
{
   return reinterpret_cast<const struct sockaddr*>(addr);
}

} // namespace cbee
