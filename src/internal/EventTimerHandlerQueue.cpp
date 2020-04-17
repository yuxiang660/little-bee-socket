#include "EventTimerHandlerQueue.h"
#include "internal/system/Macros.h"

namespace cbee
{

EventTimerHandlerQueue::EventTimerHandlerQueue(const EventModifierInterface& modifier) :
   eventModifier(modifier),
   fd(),
   event(fd.getFd(), [this](){handleRead();}, nullptr, nullptr, nullptr)
{
   event.enableReadEvent();
   eventModifier.updateEvent(fd.getFd(), &event);
}

EventTimerHandlerQueue::~EventTimerHandlerQueue()
{
   eventModifier.deleteEvent(fd.getFd());
}

void EventTimerHandlerQueue::addHandler(const TimerHandler& handler)
{
   auto it = handlerQueue.emplace(handler.getExpiredTime(), handler);
   if (it == handlerQueue.begin())
   {
      fd.triggerEventAt(handler.getExpiredTime());
   }
}

void EventTimerHandlerQueue::cancelHandler(int id)
{
   const auto sizeBeforeCancel = handlerQueue.size();

   for (auto it = handlerQueue.begin(); it != handlerQueue.end();)
   {
      if (it->second.getId() == id)
      {
         it = handlerQueue.erase(it);
      }
      else
      {
         it++;
      }
   }

   if (sizeBeforeCancel == handlerQueue.size()) HANDLE_ERROR("EventTimerHandlerQueue::cancelHandler failure");
}

void EventTimerHandlerQueue::handleRead()
{
   Timestamp now(Timestamp::now());
   fd.cleanEvent();

   auto expiredHandlers = extractExpiredHandlers(now);

   for (auto& expiredHandler : expiredHandlers)
   {
      expiredHandler.run();
   }

   triggerEvent(expiredHandlers, now);
}

std::vector<TimerHandler> EventTimerHandlerQueue::extractExpiredHandlers(Timestamp handleTime)
{
   std::vector<TimerHandler> expiredHandlers;

   auto upperIt = handlerQueue.upper_bound(handleTime);
   for (auto it = handlerQueue.begin(); it != upperIt;)
   {
      expiredHandlers.push_back(it->second);
      it = handlerQueue.erase(it);
   }

   return expiredHandlers;
}

void EventTimerHandlerQueue::triggerEvent
(
   const std::vector<TimerHandler>& expiredHandlers,
   Timestamp handleTime
)
{
   for (auto expiredHandler : expiredHandlers)
   {
      if (expiredHandler.updateExpiredTime(handleTime))
      {
         handlerQueue.emplace(expiredHandler.getExpiredTime(), expiredHandler);
      }
   }

   if (!handlerQueue.empty())
   {
      fd.triggerEventAt(handlerQueue.begin()->first);
      return;
   }
}

}
