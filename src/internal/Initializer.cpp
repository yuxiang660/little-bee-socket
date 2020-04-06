#include "log/Logger.h"
INITIALIZE_EASYLOGGINGPP

namespace cbee
{

struct Initializer
{
   Initializer()
   {
      // Loading the library
      configLogger();
   }

   ~Initializer()
   {
      // Unloading the library
   }
};

static Initializer i;

} // namespace cbee
