#pragma once

#include "IoManager.h"
#include "internal/system/Sockaddr.h"
#include "internal/system/SocketFd.h"
#include "internal/system/Event.h"

#include <functional>

namespace cbee
{

class Acceptor
{
public:
   typedef std::function<void (int)> NewConnectionFunc;

   Acceptor
   (
      const IoManager& io,
      const NewConnectionFunc& newConn,
      const Sockaddr& listenAddr
   );
   ~Acceptor();

private:
   Acceptor(const Acceptor&) = delete;
   Acceptor& operator=(const Acceptor&) = delete;

   void handleRead();

private:
   const IoManager& ioManager;
   const NewConnectionFunc newConnectionCallback;
   const SocketFd fd;
   Event event;
};

}
