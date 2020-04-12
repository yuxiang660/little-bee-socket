#pragma once

#include "Event.h"

#include <sys/epoll.h>
#include <vector>

namespace cbee
{

class EPoller
{
public:
   EPoller();
   ~EPoller();

   void updateEvent(int fd, EventHandler event) const;
   void deleteEvent(int fd) const;

   std::vector<EventHandler> poll(int timeoutMs);

private:
   EPoller(const EPoller&) = delete;
   EPoller& operator=(const EPoller&) = delete;

private:
   const int epollFd;
   std::vector<struct epoll_event> pollContainer;
};

} // namespace cbee
