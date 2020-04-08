#include "Logger.h"

#include <gtest/gtest.h>

namespace
{

TEST(LoggerBasicTest, log_levels_expectedConsoleOut)
{
   LOG(WARNING) << "Logger WARNING";
   LOG(TRACE) << "Logger TRACE";
   LOG(DEBUG) << "Logger DEBUG";
   LOG(ERROR) << "Logger ERROR";
   ASSERT_DEATH((LOG(FATAL) << "Logger FATAL"), "");
}

struct LoggerTest : public testing::Test
{
   ~LoggerTest()
   {
      cbee::Logger::config();
   }
};

TEST_F(LoggerTest, log_messageFormats_expectedConsoleOut)
{
   el::Configurations testConf;
   testConf.setToDefault();
   testConf.set(el::Level::Trace, el::ConfigurationType::Format, "%loc, %fbase, %line - %msg");
   el::Loggers::reconfigureLogger("default", testConf);
   LOG(TRACE) << "Message Format: %loc, %fbase, %line - %msg";
}

TEST_F(LoggerTest, log_disableLog_noConsoleOut)
{
   el::Configurations testConf;
   testConf.setToDefault();
   testConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
   el::Loggers::reconfigureLogger("default", testConf);
   LOG(TRACE) << "No Console out";
}

} // namespace
