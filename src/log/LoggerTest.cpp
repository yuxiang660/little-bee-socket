#include "Logger.h"

#include <gtest/gtest.h>

namespace
{

TEST(LoggerTest, simple)
{
   LOG(WARNING) << "Logger WARNING";
   LOG(TRACE) << "Logger TRACE";
   LOG(DEBUG) << "Logger DEBUG";
   LOG(ERROR) << "Logger ERROR";
   ASSERT_DEATH((LOG(FATAL) << "Logger FATAL"), "");
}

} // namespace
