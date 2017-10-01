#include <ctime>

namespace synapse {
  template<typename F>
  double stopwatch(F f, int n=1) {
    auto start = time(NULL);
    for(int i=0; i<n; ++i)
      f();
    auto end = time(NULL);
    return difftime(end, start);
  }
  
  /*template<typename Setup, typename F>
  double bench(Setup s, F f, int n) {
    double t = 0;
    for(int i=0; i<n; ++i) {
      s();
      t += stopwatch(f);
    }
    return t;
  }*/
};