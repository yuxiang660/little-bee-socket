#include "Socket.h"

#include <sys/socket.h>
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

Socket Socket::accept(Sockaddr* connectAddr)
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

struct sockaddr* Socket::sockaddrCast(Sockaddr* addr)
{
   return reinterpret_cast<struct sockaddr*>(addr);
}

const struct sockaddr* Socket::sockaddrCast(const Sockaddr* addr)
{
   return reinterpret_cast<const struct sockaddr*>(addr);
}

} // namespace cbee
