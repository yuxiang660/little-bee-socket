#pragma once

namespace cbee
{

class EventTriggerFd
{
public:
   EventTriggerFd();
   ~EventTriggerFd();

   int cleanEvents() const;
   void triggerEvent() const;

   int getFd() const;

private:
   const int fd;
};

}
