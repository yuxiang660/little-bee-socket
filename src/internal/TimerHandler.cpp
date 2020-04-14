#include "TimerHandler.h"

namespace cbee
{

std::atomic<int> TimerHandler::s_numberOfCreated = 0;

TimerHandler::TimerHandler(Timestamp time, double interval, TimerFunc cb) :
   expiredTime(time),
   intervalSeconds(interval),
   callback(cb),
   id(s_numberOfCreated.fetch_add(1))
{
}

void TimerHandler::run() const
{
   if (callback) callback();
}

bool TimerHandler::updateExpiredTime(Timestamp handleTime)
{
   if (intervalSeconds == 0) return false;

   expiredTime = handleTime.addSeconds(intervalSeconds);
   return true;
}

Timestamp TimerHandler::getExpiredTime() const
{
   return expiredTime;
}

int TimerHandler::getId() const
{
   return id;
}

} // namespace cbee
