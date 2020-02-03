
#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
 
template <typename T>
class ThreadSafeQueue
{
 public:
 
  void pop(T& item)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty())
    {
      cond_var_.wait(lock);
    }
    item = queue_.front();
    queue_.pop();
  }
 
  void push(const T& item)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(item);
    lock.unlock();
    cond_var_.notify_one();
  }
 
 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_var_;
};

#endif