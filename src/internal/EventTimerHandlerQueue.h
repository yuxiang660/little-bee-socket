#pragma once

#include "internal/system/EventModifierInterface.h"
#include "internal/system/EventTimerFd.h"
#include "internal/system/Event.h"
#include "internal/system/Timestamp.h"
#include "TimerHandler.h"

#include <map>
#include <vector>

namespace cbee
{

class EventTimerHandlerQueue
{
public:
   EventTimerHandlerQueue(const EventModifierInterface& modifier);
   ~EventTimerHandlerQueue();

   void addHandler(const TimerHandler& handler);
   void cancelHandler(int id);
   void handleRead();

private:
   EventTimerHandlerQueue(const EventTimerHandlerQueue&) = delete;
   EventTimerHandlerQueue& operator=(const EventTimerHandlerQueue&) = delete;

   std::vector<TimerHandler> extractExpiredHandlers(Timestamp handleTime);
   void triggerEvent(const std::vector<TimerHandler>& expiredHandlers, Timestamp handleTime);

private:
   const EventModifierInterface& eventModifier;
   const EventTimerFd fd;
   Event event;
   std::multimap<Timestamp, TimerHandler> handlerQueue;
};

}
