#include "Acceptor.h"

#include <assert.h>

namespace cbee
{

Acceptor::Acceptor
(
   const IoManager& io,
   const NewConnectionFunc& newConn,
   const Sockaddr& listenAddr
) :
   ioManager(io),
   newConnectionCallback(newConn),
   fd(),
   event(fd.getFd(), [this](){handleRead();}, nullptr, nullptr, nullptr)
{
   assert(ioManager.isInIoThread() == true);

   fd.setNonBlock();
   fd.setReuseAddr();
   fd.bind(listenAddr);
   fd.listen();
   event.enableReadEvent();
   ioManager.updatePollerEvent(fd.getFd(), &event);
}

Acceptor::~Acceptor()
{
   ioManager.deletePollerEvent(fd.getFd());
}

void Acceptor::handleRead()
{
   assert(ioManager.isInIoThread() == true);

   Sockaddr connectAddr;
   int connFd = fd.accept(&connectAddr);

   assert(newConnectionCallback != nullptr);
   newConnectionCallback(connFd);
}

}
