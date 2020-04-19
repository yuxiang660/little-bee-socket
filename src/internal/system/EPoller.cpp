#include "EPoller.h"
#include "Macros.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

namespace cbee
{

static const int kInitialActiveEventsSize = 16;

EPoller::EPoller() : epollFd(::epoll_create1(EPOLL_CLOEXEC)), pollContainer(kInitialActiveEventsSize)
{
   assert(epollFd >= 0);
}

EPoller::~EPoller()
{
   __attribute__((unused)) auto ret = ::close(epollFd);
   assert(ret == 0);
}

std::vector<EventHandler> EPoller::poll(int timeoutMs)
{
   int sizeOfPollContainer = static_cast<int>(pollContainer.size());
   int numberOfActiveEvents = ::epoll_wait
   (
      epollFd,
      pollContainer.data(),
      sizeOfPollContainer,
      timeoutMs
   );

   if (numberOfActiveEvents < 0) HANDLE_ERROR("Epoller::poll failure");

   assert(numberOfActiveEvents <= sizeOfPollContainer);
   std::vector<EventHandler> activeEvents;
   for (int i = 0; i < numberOfActiveEvents; i++)
   {
      EventHandler handle = static_cast<EventHandler>(pollContainer[i].data.ptr);
      handle->setActiveEvents(pollContainer[i].events);
      activeEvents.push_back(handle);
   }

   if (numberOfActiveEvents == sizeOfPollContainer)
   {
      pollContainer.resize(2 * sizeOfPollContainer);
   }

   return activeEvents;
}

void EPoller::updateEvent(int fd, EventHandler event) const
{
   struct epoll_event e;
   memset(&e, 0, sizeof(epoll_event));
   e.events = event->getAllEvents();
   e.data.ptr = event;
   if (::epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &e) < 0)
   {
      if (::epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &e) < 0) HANDLE_ERROR("EPoller::updateEvent failure");
   }
}

void EPoller::deleteEvent(int fd) const
{
   if (::epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr) < 0) HANDLE_ERROR("EPoller::deleteEvent failure");
}

} // namespace cbee
