#include "Connection.h"

#include "assert.h"

namespace cbee
{

Connection::Connection
(
   const IoManager& io,
   const int connFd,
   Event::RemoveFunc removeCallback
):
   ioManager(io),
   fd(connFd),
   event
   (
      connFd,
      [this](){handleRead();},
      [this](){handleWrite();},
      removeCallback,
      [this](){handleError();}
   )
{
   event.enableReadEvent();
   event.enableWriteEvent();
   ioManager.updatePollerEvent(fd.getFd(), &event);
}

Connection::~Connection()
{
   ioManager.deletePollerEvent(fd.getFd());
}

void Connection::handleRead()
{
   assert(true == ioManager.isInIoThread());
}

void Connection::handleWrite()
{}

void Connection::handleError()
{}

}
