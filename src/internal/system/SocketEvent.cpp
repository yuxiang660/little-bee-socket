#include "SocketEvent.h"
#include "log/Logger.h"

#include <assert.h>

namespace cbee
{

SocketEvent::SocketEvent
(
   const Socket& socket,
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

int SocketEvent::getAllEvents() const
{
   return allEvents;
}

int SocketEvent::getActiveEvents() const
{
   return activeEvents;
}

void SocketEvent::setActiveEvents(int events)
{
   activeEvents = events;
}

void SocketEvent::enableReadEvent()
{
   allEvents |= kConnectedOrReadableOrCloseEvent;
}

void SocketEvent::disableReadEvent()
{
   allEvents &= ~kConnectedOrReadableOrCloseEvent;
}

void SocketEvent::enableWriteEvent()
{
   allEvents |= kWritableEvent;
}

void SocketEvent::disableWriteEvent()
{
   allEvents &= kWritableEvent;
}

void SocketEvent::disableAllEvents()
{
   allEvents = kNoneEvent;
}

void SocketEvent::handleEvent() const
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
