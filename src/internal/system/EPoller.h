#pragma once

#include "Event.h"
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

   void updateEvent(const Socket& socket, EventHandler event);
   void deleteEvent(const Socket& socket);

   std::vector<EventHandler> poll(int timeoutMs);

private:
   int epollFd;
   std::vector<struct epoll_event> pollContainer;
};

} // namespace cbee
