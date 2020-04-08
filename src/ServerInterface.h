#pragma once

#include "Functions.h"

namespace cbee
{

class ServerInterface
{
public:
   virtual ~ServerInterface() = default;
   virtual void start() = 0;
   // Server Process:
   // 1. Connect
   // 2. In the Loop:
   //    2.1 Read
   //    2.2 Write
   // 3. DisConnect
   virtual void setConnectedFunc(const ConnectedFunc &f) = 0;
   virtual void setReadDoneFunc(const ReadDoneFunc &f) = 0;
   virtual void setWriteDoneFunc(const WriteDoneFunc &f) = 0;
   virtual void setDisConnectedFunc(const DisConnectedFunc &f) = 0;
};

} // namespace cbee
