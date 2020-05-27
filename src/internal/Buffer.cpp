#include "Buffer.h"

namespace cbee
{

Buffer::Buffer() :
   buf(kCheapPrepend + kInitialSize),
   readerIndex(kCheapPrepend),
   writerIndex(kCheapPrepend)
{}

}
