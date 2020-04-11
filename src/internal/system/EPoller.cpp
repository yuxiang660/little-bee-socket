#include "EPoller.h"

#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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

   if (numberOfActiveEvents < 0)
   {
      perror("Epoller::poll failure");
      exit(EXIT_FAILURE);
   }

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

void EPoller::updateEvent(const Socket& socket, EventHandler event)
{
   struct epoll_event e;
   memset(&e, 0, sizeof(epoll_event));
   e.events = event->getAllEvents();
   e.data.ptr = event;
   if (::epoll_ctl(epollFd, EPOLL_CTL_ADD, socket.getFd(), &e) < 0)
   {
      perror("EPoller::updateEvent failure");
      exit(EXIT_FAILURE);
   }
}

void EPoller::deleteEvent(const Socket& socket)
{
   if (::epoll_ctl(epollFd, EPOLL_CTL_DEL, socket.getFd(), nullptr) < 0)
   {
      perror("EPoller::deleteEvent failure");
      exit(EXIT_FAILURE);
   }
}

} // namespace cbee
