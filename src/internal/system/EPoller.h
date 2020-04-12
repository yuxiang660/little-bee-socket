#pragma once

#include "EventModifierInterface.h"
#include "Event.h"

#include <sys/epoll.h>
#include <vector>

namespace cbee
{

class EPoller : public EventModifierInterface
{
public:
   EPoller();
   ~EPoller();

   void updateEvent(int fd, EventHandler event) const override;
   void deleteEvent(int fd) const override;

   std::vector<EventHandler> poll(int timeoutMs);

private:
   EPoller(const EPoller&) = delete;
   EPoller& operator=(const EPoller&) = delete;

private:
   const int epollFd;
   std::vector<struct epoll_event> pollContainer;
};

} // namespace cbee
