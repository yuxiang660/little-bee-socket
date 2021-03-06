#include "Timestamp.h"

namespace cbee
{

Timestamp::Timestamp() : microSecondsSinceEpoch(0)
{
}

Timestamp::Timestamp(int64_t microSeconds) :
   microSecondsSinceEpoch(microSeconds)
{
}

Timestamp Timestamp::now()
{
   struct timeval now;
   gettimeofday(&now, nullptr);
   int64_t seconds = now.tv_sec;
   return Timestamp(seconds * kMicroSecondsPerSecond + now.tv_usec);
}

int64_t Timestamp::getMicroSeconds() const
{
   return microSecondsSinceEpoch;
}

Timestamp Timestamp::addSeconds(double seconds) const
{
   int64_t delta = static_cast<int64_t>(seconds * kMicroSecondsPerSecond);
   return Timestamp(microSecondsSinceEpoch + delta);
}

}
