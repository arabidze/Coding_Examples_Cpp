
/* using c++11 */

#include<iostream>
#include<vector>
#include<numeric>
#include<future>

class a_class {
  public:
    int a_func(int m, std::vector<int>& mm) {
      return std::accumulate(mm.begin(), mm.end(), m);
    }                                      
};

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename F, typename... Args>
std::future<typename std::result_of<F(Args...)>::type>
Async(F&& a_func, Args&&... params) {
  return(std::async(std::launch::async, std::forward<F>(a_func),
        std::forward<Args>(params)...));
}

int main() {
  
   
  std::vector<int> vec(10000) ;
  std::srand(std::time(nullptr)); 
  std::generate(vec.begin(), vec.end(), [](){return rand()%100;});
  
  a_class a_obj ;
  auto a = Async(&a_class::a_func, &a_obj, 0, std::ref(vec)) ;
  std::cout << " sum: " << a.get() << std::endl;

  return 0;
}
