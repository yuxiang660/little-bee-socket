#pragma once

#include "SocketEvent.h"
#include "Socket.h"

#include <sys/epoll.h>
#include <vector>

namespace cbee
{

class EPoller
{
public:
   EPoller();
   ~EPoller();

   void updateSocketEvent(const Socket& socket, SocketEventHandler event) const;
   void deleteSocketEvent(const Socket& socket) const;

   std::vector<SocketEventHandler> poll(int timeoutMs);

private:
   EPoller(const EPoller&) = delete;
   EPoller& operator=(const EPoller&) = delete;

private:
   int epollFd;
   std::vector<struct epoll_event> pollContainer;
};

} // namespace cbee
