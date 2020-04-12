#pragma once

#include "Event.h"

namespace cbee
{

class EventModifierInterface
{
public:
   virtual ~EventModifierInterface() = default;

   virtual void updateEvent(int fd, EventHandler event) const = 0;
   virtual void deleteEvent(int fd) const = 0;
};

}
