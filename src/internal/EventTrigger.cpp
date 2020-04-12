#include "EventTrigger.h"

namespace cbee
{

EventTrigger::EventTrigger(const EventModifierInterface& modifier) :
   eventModifier(modifier),
   fd(),
   event(fd.getFd(), [this](){handleRead();}, nullptr, nullptr, nullptr)
{
   event.enableReadEvent();
   eventModifier.updateEvent(fd.getFd(), &event);
}

EventTrigger::~EventTrigger()
{
   eventModifier.deleteEvent(fd.getFd());
}

void EventTrigger::triggerOnce() const
{
   fd.triggerEvent();
}

void EventTrigger::handleRead() const
{
   fd.cleanEvents();
}

}
