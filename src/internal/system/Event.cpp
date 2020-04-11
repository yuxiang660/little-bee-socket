#include "Event.h"
#include "log/Logger.h"

#include <assert.h>

namespace cbee
{

Event::Event
(
   const SocketFd& socket,
   ReadFunc read,
   WriteFunc write,
   RemoveFunc remove,
   ErrorFunc error
) :
   bindedSocket(socket),
   allEvents(kNoneEvent),
   activeEvents(kNoneEvent),
   readCallback(read),
   writeCallback(write),
   removeCallback(remove),
   errorCallback(error)
{
   assert(bindedSocket.getFd() >= 0);
}

int Event::getAllEvents() const
{
   return allEvents;
}

int Event::getActiveEvents() const
{
   return activeEvents;
}

void Event::setActiveEvents(int events)
{
   activeEvents = events;
}

void Event::enableReadEvent()
{
   allEvents |= kConnectedOrReadableOrCloseEvent;
}

void Event::disableReadEvent()
{
   allEvents &= ~kConnectedOrReadableOrCloseEvent;
}

void Event::enableWriteEvent()
{
   allEvents |= kWritableEvent;
}

void Event::disableWriteEvent()
{
   allEvents &= kWritableEvent;
}

void Event::disableAllEvents()
{
   allEvents = kNoneEvent;
}

void Event::handleEvent() const
{
   if (activeEvents & kErrorEvent)
   {
      LOG(DEBUG) << "kErrorEvent event: " << activeEvents;
      if (errorCallback) errorCallback();
      return;
   }

   if (activeEvents & kRemoveEvent)
   {
      LOG(DEBUG) << "kRemoveEvent event: " << activeEvents;
      if (removeCallback) removeCallback(bindedSocket.getFd());
      return;
   }

   if (activeEvents & kConnectedOrReadableOrCloseEvent)
   {
      LOG(DEBUG) << "kConnectedOrReadableOrCloseEvent event: " << activeEvents;
      if (readCallback) readCallback();
      return;
   }

   if (activeEvents & kWritableEvent)
   {
      LOG(DEBUG) << "EPOLLOUT event: " << activeEvents;
      if (writeCallback) writeCallback();
      return;
   }
}

} // namespace cbee
