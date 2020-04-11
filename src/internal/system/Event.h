#pragma once
#include "SocketFd.h"

#include <sys/epoll.h>
#include <functional>

namespace cbee
{

class Event
{
public:
   typedef std::function<void()> ReadFunc;
   typedef std::function<void()> WriteFunc;
   typedef std::function<void(int)> RemoveFunc;
   typedef std::function<void()> ErrorFunc;

   static const int kNoneEvent = 0;
   // 1. The listen socket receives a new connection, which triggers EPOLLIN.
   // 2. The socket receives data, which triggers EPOLLIN.
   // 3. The peer socket is closed, which triggers EPOLLIN + EPOLLRDHUP locally.
   static const int kConnectedOrReadableOrCloseEvent = EPOLLIN | EPOLLRDHUP | EPOLLPRI;
   // 1. The client socket connects with the server successfully, which triggers EPOLLOUT.
   // 2. The socket becomes writable again from full state, which triggers EPOLLOUT.
   static const int kWritableEvent = EPOLLOUT;
   // 1. The socket is shutdown(read and write), which triggers EPOLLIN + EPOLLHUP + EPOLLRDHUP (no EPOLLERR).
   static const int kRemoveEvent = EPOLLHUP;
   // 1. Write data to peer socket that has been closed. Local socke receives RST message which triggers EPOLLIN+EPOLLRDHUP+EPOLLHUP+EPOLLERR.
   static const int kErrorEvent = EPOLLERR;

   Event
   (
      const SocketFd& socket,
      ReadFunc read,
      WriteFunc write,
      RemoveFunc remove,
      ErrorFunc error
   );

   int getAllEvents() const;
   int getActiveEvents() const;
   void setActiveEvents(int events);

   void enableReadEvent();
   void disableReadEvent();
   void enableWriteEvent();
   void disableWriteEvent();
   void disableAllEvents();

   void handleEvent() const;

private:
   Event(const Event&) = delete;
   Event& operator=(const Event&) = delete;

private:
   const SocketFd& bindedSocket;
   int allEvents;    // Events configured to epoll
   int activeEvents; // Events from epoll polling
   ReadFunc readCallback;
   WriteFunc writeCallback;
   RemoveFunc removeCallback;
   ErrorFunc errorCallback;
};

typedef Event* EventHandler;

} // namespace cbee
