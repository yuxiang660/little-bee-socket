#pragma once

#include <sys/time.h>
#include <stdint.h>

namespace cbee
{

class Timestamp
{
public:
   static const int kMicroSecondsPerSecond = 1000 * 1000;

   Timestamp();
   Timestamp(int64_t microSeconds);

   static Timestamp now();

   int64_t getMicroSeconds() const;

private:
   int64_t microSecondsSinceEpoch;
};

} // namespace cbee
