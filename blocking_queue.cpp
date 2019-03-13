
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <stdio.h>
#include <math.h>

#include<queue>
#include<unistd.h>
#include<numeric>

/* some class for tests */
class Task
{
  public:
    Task( const Task& ) = delete;
    Task& operator=( const Task& ) = delete;

    Task(int val, bool sent = false): m_val(val), sent(sent) {}
    ~Task(){}

    const int m_val;
    bool sent ;
};

/* blocking queue class */
template<typename T>
class BlockingQueue {
  private:
    std::mutex mutex;
    std::condition_variable condvar;
    std::queue<std::unique_ptr<T>> queue;
  public:
    BlockingQueue() = default;
    ~BlockingQueue() {}

    template<typename U>
      void push(U &&a) {
        {
          std::unique_lock<std::mutex> lock(mutex);
          usleep(rand()%100); // simulate some random delays
          queue.push(std::forward<std::unique_ptr<T>>(a)) ;
        }
        condvar.notify_one();
      }

    std::unique_ptr<Task> pop(){
      std::unique_lock<std::mutex> lock(mutex);
      condvar.wait(lock, [&]{return !queue.empty();});
            
      std::unique_ptr<Task> ptr = std::move(queue.front()) ;
      queue.pop() ;
      return ptr;
    }
};

void poper_func(BlockingQueue<Task> &bq, std::vector<int>& vec, int threshold)
{
  while(true)
  {
    std::unique_ptr<Task> t = bq.pop();
    if(t->m_val >= threshold)
            vec.push_back(t->m_val) ;

    if (t->sent)
      break;
  }
}

void pusher_func(BlockingQueue<Task> &bq, int n_tasks) 
{
  for (int i = 0; i < n_tasks; i++)
  {
    auto task = make_unique<Task>(i+1, i == n_tasks-1);
    bq.push(std::move(task));
  }
}

int main() {
  BlockingQueue<Task> q; 
  std::vector<int> t;
  
  int threshold = 5;
  
  std::thread push_thr(pusher_func, std::ref(q), 10000);
  std::thread pop_thr(poper_func, std::ref(q), std::ref(t), threshold);
  
  pop_thr.join();
  usleep(1000); // start to pop thread before push, just another check
  push_thr.join();
  
  auto lambda = [threshold](int i){return i%threshold ==0;} ;

  int n_vals = std::count_if(t.begin(), t.end(), lambda);
  std::vector<int> tmp(n_vals) ;
  std::copy_if(t.begin(), t.end(), tmp.begin(), lambda);
  
  std::cout << " sum: " << std::accumulate(m.begin(), m.end(), 0) << std::endl;
  
  return 0;
}
