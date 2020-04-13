#include "TimerHandler.h"

namespace cbee
{

TimerHandler::TimerHandler(Timestamp time, double interval, TimerFunc cb) :
   expiredTime(time),
   intervalSeconds(interval),
   callback(cb)
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

} // namespace cbee
