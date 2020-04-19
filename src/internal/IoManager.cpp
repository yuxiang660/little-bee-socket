#include "IoManager.h"
#include "internal/system/Macros.h"

#include <assert.h>

namespace cbee
{

IoManager::IoManager() :
   ioThreadId(std::this_thread::get_id()),
   quitFlag(false),
   poller(),
   eventTrigger(poller),
   timerHandlers(poller)
{
}

void IoManager::loop()
{
   assert(isInIoThread() == true);

   quitFlag = false;
   const int kPollTimeoutMs = 10000;

   while(!quitFlag)
   {
      auto activeEvents = poller.poll(kPollTimeoutMs);
      for (auto& activeEvent : activeEvents)
      {
         activeEvent->handleEvent();
      }
      runPendingFunctors();
   }
}

void IoManager::quit()
{
   quitFlag = true;
   eventTrigger.triggerOnce();
}

void IoManager::updatePollerEvent(int fd, EventHandler event) const
{
   poller.updateEvent(fd, event);
}

void IoManager::deletePollerEvent(int fd) const
{
   poller.deleteEvent(fd);
}

bool IoManager::isInIoThread() const
{
   return std::this_thread::get_id() == ioThreadId;
}

void IoManager::runInIoThread(Functor cb)
{
   assert(cb != nullptr);

   if (isInIoThread())
   {
      cb();
      return;
   }

   {
      std::lock_guard<std::mutex> lock(mtx);
      pendingFunctors.push_back(cb);
   }
   eventTrigger.triggerOnce();
}

void IoManager::runPendingFunctors()
{
   std::vector<Functor> functors;
   {
      std::lock_guard<std::mutex> lock(mtx);
      functors.swap(pendingFunctors);
   }

   for (auto& functor : functors)
   {
      assert(functor != nullptr);
      functor();
   }
}

int IoManager::runAfter(double delay, TimerHandler::TimerFunc cb)
{
   assert(nullptr != cb);

   Timestamp time = Timestamp::now().addSeconds(delay);
   TimerHandler handler(time, 0, cb);
   runInIoThread([this, handler](){timerHandlers.addHandler(handler);});
   return handler.getId();
}

int IoManager::runEvery(double interval, TimerHandler::TimerFunc cb)
{
   assert(cb != nullptr);

   Timestamp time = Timestamp::now().addSeconds(interval);
   TimerHandler handler(time, interval, cb);
   runInIoThread([this, handler](){timerHandlers.addHandler(handler);});
   return handler.getId();
}

void IoManager::cancelTimerHandler(int handlerId)
{
   runInIoThread([this, handlerId](){timerHandlers.cancelHandler(handlerId);});
}

}
