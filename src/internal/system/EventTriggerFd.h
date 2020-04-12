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
   EventTriggerFd(const EventTriggerFd&) = delete;
   EventTriggerFd& operator=(const EventTriggerFd&) = delete;

private:
   const int fd;
};

}
