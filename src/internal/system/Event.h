#pragma once

#include <sys/epoll.h>
#include <functional>

namespace cbee
{

class Event
{
public:
   typedef std::function<void()> EventCallback;
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

   Event();
   int getAllEvents() const;
   int getActiveEvents() const;
   void setActiveEvents(int events);

   void setReadCallback(EventCallback cb);
   void setWriteCallback(EventCallback cb);
   void setRemoveCallback(EventCallback cb);
   void setErrorCallback(EventCallback cb);

   void enableReadEvent();
   void disableReadEvent();
   void enableWriteEvent();
   void disableWriteEvent();
   void disableAllEvents();

   void handleEvent();

private:
   int allEvents;    // Events configured to epoll
   int activeEvents; // Events from epoll polling
   EventCallback writeCallback;
   EventCallback readCallback;
   EventCallback removeCallback;
   EventCallback errorCallback;
};

typedef Event* EventHandler;

} // namespace cbee
