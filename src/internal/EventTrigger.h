#pragma once

#include "internal/system/EventModifierInterface.h"
#include "internal/system/EventTriggerFd.h"
#include "internal/system/Event.h"

namespace cbee
{

class EventTrigger
{
public:
   EventTrigger(const EventModifierInterface& modifier);
   ~EventTrigger();

   void triggerOnce() const;

private:
   EventTrigger(const EventTrigger&) = delete;
   EventTrigger& operator=(const EventTrigger&) = delete;

   void handleRead() const;

private:
   const EventModifierInterface& eventModifier;
   const EventTriggerFd fd;
   Event event;
};

}
