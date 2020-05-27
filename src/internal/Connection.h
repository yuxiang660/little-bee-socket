#pragma once

#include "IoManager.h"
#include "internal/system/Event.h"
#include "internal/system/SocketFd.h"

namespace cbee
{

class Connection
{
public:
   Connection
   (
      const IoManager& io,
      const int connFd,
      Event::RemoveFunc removeCallback
   );
   ~Connection();

private:
   Connection(const Connection&) = delete;
   Connection& operator=(const Connection&) = delete;

   void handleRead();
   void handleWrite();
   void handleError();

private:
   const IoManager& ioManager;
   const SocketFd fd;
   Event event;
};

}
