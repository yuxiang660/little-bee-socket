#pragma once

#include "SocketEvent.h"
#include "SocketFd.h"

#include <sys/epoll.h>
#include <vector>

namespace cbee
{

class EPoller
{
public:
   EPoller();
   ~EPoller();

   void updateSocketEvent(const SocketFd& socket, SocketEventHandler event) const;
   void deleteSocketEvent(const SocketFd& socket) const;

   std::vector<SocketEventHandler> poll(int timeoutMs);

private:
   EPoller(const EPoller&) = delete;
   EPoller& operator=(const EPoller&) = delete;

private:
   int epollFd;
   std::vector<struct epoll_event> pollContainer;
};

} // namespace cbee
