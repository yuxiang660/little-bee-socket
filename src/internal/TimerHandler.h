#pragma once

#include "internal/system/Timestamp.h"

#include <functional>

namespace cbee
{

class TimerHandler
{
public:
   typedef std::function<void()> TimerFunc;

   TimerHandler(Timestamp time, double interval, TimerFunc cb);

   void run() const;
   bool updateExpiredTime(Timestamp handleTime);
   Timestamp getExpiredTime() const;

private:
   TimerHandler(const TimerHandler&) = delete;
   TimerHandler& operator=(const TimerHandler&) = delete;

private:
   Timestamp expiredTime;
   // Interval seconds for the callback function. Zero means it only calls once.
   const double intervalSeconds;
   const TimerFunc callback;
};

}
