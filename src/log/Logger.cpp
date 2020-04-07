#include "Logger.h"

namespace cbee
{

void Logger::config()
{
   el::Configurations defaultConf;
   defaultConf.setToDefault();
   defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime [%level] %msg");
   defaultConf.setGlobally(el::ConfigurationType::Filename, "/tmp/logs/cbee.log");
   defaultConf.set(el::Level::Error, el::ConfigurationType::Filename, "/tmp/logs/cbee-error.log");
   defaultConf.set(el::Level::Fatal, el::ConfigurationType::Filename, "/tmp/logs/cbee-fatal.log");
   el::Loggers::reconfigureLogger("default", defaultConf);
}

} // namespace cbee
