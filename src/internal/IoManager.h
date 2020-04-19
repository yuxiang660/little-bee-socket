#pragma once

#include "internal/system/EPoller.h"
#include "internal/system/Event.h"
#include "internal/EventTrigger.h"
#include "internal/EventTimerHandlerQueue.h"
#include "internal/TimerHandler.h"
#include "internal/system/Timestamp.h"

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

namespace cbee
{

// IoManager member functions must be thread safe.
class IoManager
{
public:
   typedef std::function<void()> Functor;

   IoManager();

   void loop();
   void quit();

   bool isInIoThread() const;
   void runInIoThread(Functor cb);

   void updatePollerEvent(int fd, EventHandler event) const;
   void deletePollerEvent(int fd) const;

   int runAfter(double delay, TimerHandler::TimerFunc cb);
   int runEvery(double interval, TimerHandler::TimerFunc cb);
   void cancelTimerHandler(int handlerId);

private:
   IoManager(const IoManager&) = delete;
   IoManager& operator=(const IoManager&) = delete;

   void runPendingFunctors();

private:
   const std::thread::id ioThreadId;
   std::atomic<bool> quitFlag;
   EPoller poller;
   EventTrigger eventTrigger;
   EventTimerHandlerQueue timerHandlers;

   std::mutex mtx;
   std::vector<Functor> pendingFunctors;
};

} // namespace cbe
