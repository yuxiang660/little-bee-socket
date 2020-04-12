#include "EventTriggerFd.h"
#include "Macros.h"

#include <sys/eventfd.h>
#include <assert.h>
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

int EventTriggerFd::cleanEvents() const
{
   uint64_t numberOfEvents = 0;

   if (::read(fd, &numberOfEvents, sizeof(uint64_t)) < 0) HANDLE_ERROR("EventTriggerFd::cleanEvent failure");

   return static_cast<int>(numberOfEvents);
}

void EventTriggerFd::triggerEvent() const
{
   uint64_t number = 1;

   if (::write(fd, &number, sizeof(uint64_t)) < 0) HANDLE_ERROR("EventTriggerFd::triggerEvent failure");
}

int EventTriggerFd::getFd() const
{
   return fd;
}

} // namespace cbee

