#pragma once

#include "ServerInterface.h"
#include "Functions.h"

namespace cbee
{

class Server : public ServerInterface
{
public:
   void setConnectedFunc(const ConnectedFunc &f);
   void setReadDoneFunc(const ReadDoneFunc &f);
   void setWriteDoneFunc(const WriteDoneFunc &f);
   void setDisConnectedFunc(const DisConnectedFunc &f);

private:
   ConnectedFunc connectedFunc;
   ReadDoneFunc readDoneFunc;
   WriteDoneFunc writeDoneFunc;
   DisConnectedFunc disConnectedFunc;
};

} // namespace cbee
