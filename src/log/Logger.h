#pragma once

#include "internal/lib/easylogging++.h"

#include <limits.h>
#include <stdlib.h>

namespace cbee
{

inline void configLogger()
{
   char path[PATH_MAX];
   // use relative path from "build" folder to get fullpath for "default-logger.conf"
   realpath("../src/log/default-logger.conf", path);
   el::Configurations conf(path);
   el::Loggers::reconfigureAllLoggers(conf);
}

} // namespace cbee