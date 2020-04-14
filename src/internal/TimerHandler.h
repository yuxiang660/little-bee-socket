#pragma once

#include "internal/system/Timestamp.h"

#include <atomic>
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
   int getId() const;

private:
   Timestamp expiredTime;
   // Interval seconds for the callback function. Zero means it only calls once.
   const double intervalSeconds;
   const TimerFunc callback;
   const int id;

   static std::atomic<int> s_numberOfCreated;
};

}
