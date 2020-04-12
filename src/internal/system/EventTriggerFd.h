#pragma once

namespace cbee
{

class EventTriggerFd
{
public:
   EventTriggerFd();
   ~EventTriggerFd();

   int cleanEvents();
   void triggerEvent();

   int getFd() const;

private:
   const int fd;
};

}
