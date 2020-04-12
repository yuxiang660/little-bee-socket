#include "EventTriggerFd.h"

#include <sys/eventfd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace cbee
{

EventTriggerFd::EventTriggerFd() : fd(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC))
{
   assert(fd >= 0);
}

EventTriggerFd::~EventTriggerFd()
{
   __attribute__((unused)) auto ret = ::close(fd);
   assert(ret == 0);
}

int EventTriggerFd::cleanEvents()
{
   uint64_t number = 0;

   if (::read(fd, &number, sizeof(uint64_t)) < 0)
   {
      perror("EventTriggerFd::cleanEvent failure");
      exit(EXIT_FAILURE);
   }

   return static_cast<int>(number);
}

void EventTriggerFd::triggerEvent()
{
   uint64_t number = 1;

   if (::write(fd, &number, sizeof(uint64_t)) < 0)
   {
      perror("EventTriggerFd::triggerEvent failure");
      exit(EXIT_FAILURE);
   }
}

int EventTriggerFd::getFd() const
{
   return fd;
}

} // namespace cbee

