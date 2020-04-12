#include "SocketFd.h"
#include "Macros.h"

#include <sys/socket.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

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

void SocketFd::shutdownRead() const
{
   if (::shutdown(fd, SHUT_RD) < 0) HANDLE_ERROR("SocketFd::shutdownRead failure");
}

void SocketFd::shutdownWrite() const
{
   if (::shutdown(fd, SHUT_WR) < 0) HANDLE_ERROR("SocketFd::shutdownWrite failure");
}

void SocketFd::shutdown() const
{
   if (::shutdown(fd, SHUT_RDWR) < 0) HANDLE_ERROR("SocketFd::shutdown failure");
}

void SocketFd::setNonBlock() const
{
   int flags = ::fcntl(fd, F_GETFL, 0);
   flags |= O_NONBLOCK;
   if(::fcntl(fd, F_SETFL, flags)) HANDLE_ERROR("SocketFd::setNonBlock O_NONBLOCK failure");

   flags = ::fcntl(fd, F_GETFD, 0);
   flags |= FD_CLOEXEC;
   if(::fcntl(fd, F_SETFD, flags)) HANDLE_ERROR("SocketFd::setNonBlock FD_CLOEXEC failure");
}

void SocketFd::bind(const Sockaddr& serverAddr) const
{
   if (::bind(fd, sockaddrCast(&serverAddr), sizeof(Sockaddr)) < 0) HANDLE_ERROR("SocketFd::bind failure");
}

void SocketFd::listen() const
{
   if (::listen(fd, SOMAXCONN) < 0) HANDLE_ERROR("SocketFd::listen failure");
}

int SocketFd::accept(Sockaddr* connectAddr) const
{
   assert(connectAddr != nullptr);
   socklen_t addrlen = static_cast<socklen_t>(sizeof(Sockaddr));
   int connfd = ::accept(fd, sockaddrCast(connectAddr), &addrlen);

   if (connfd < 0) HANDLE_ERROR("SocketFd::accept failure");

   return connfd;
}

void SocketFd::connect(const Sockaddr& serverAddr) const
{
   if (::connect(fd, sockaddrCast(&serverAddr), static_cast<socklen_t>(sizeof(Sockaddr))) < 0)
      HANDLE_ERROR("SocketFd::connect failure");
}

int SocketFd::getFd() const
{
   return fd;
}

int SocketFd::read(void* buf, int count) const
{
   return static_cast<int>(::read(fd, buf, count));
}

int SocketFd::write(const void* buf, int count) const
{
   return static_cast<int>(::write(fd, buf, count));
}

Sockaddr SocketFd::getLocalAddr() const
{
   Sockaddr localaddr;
   socklen_t addrlen = static_cast<socklen_t>(sizeof(Sockaddr));

   if (::getsockname(fd, sockaddrCast(&localaddr), &addrlen) < 0) HANDLE_ERROR("SocketFd::getLocalAddr failure");

   return localaddr;
}

Sockaddr SocketFd::getPeerAddr() const
{
   Sockaddr peeraddr;
   socklen_t addrlen = static_cast<socklen_t>(sizeof(Sockaddr));

   if (::getpeername(fd, sockaddrCast(&peeraddr), &addrlen) < 0) HANDLE_ERROR("SocketFd::getPeerAddr failure");

   return peeraddr;
}

struct sockaddr* SocketFd::sockaddrCast(Sockaddr* addr) const
{
   return reinterpret_cast<struct sockaddr*>(addr);
}

const struct sockaddr* SocketFd::sockaddrCast(const Sockaddr* addr) const
{
   return reinterpret_cast<const struct sockaddr*>(addr);
}

} // namespace cbee
