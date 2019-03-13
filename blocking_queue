
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <stdio.h>
#include <math.h>

#include<queue>
#include<unistd.h>
#include<numeric>

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
          usleep(rand()%100);
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


int main() {

}
