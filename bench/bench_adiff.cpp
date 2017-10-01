#include <synapse/benchmark.hpp>
#include <synapse/adiff.hpp>
#include <random>
#include <array>
#include <iostream>

template<typename T, int size, typename F>
int bench(const F& f) {
  // f = f(array);
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_real_distribution<float> dist(-1, 1);
  auto rand = [&](){ return dist(gen); };
  const int sets = 100, reps = 100000, N = sets*reps;
  std::array<synapse::adiff<T>, size> vars;
  double t = synapse::stopwatch([&](){
    for(int i=0; i<size; ++i)
      vars[i] = { rand(), { {i, 1} } };
    for(int rep=0; rep<reps; ++rep)
      f(vars);
  }, sets);
  return N/t;
}

int main() {
  std::cout << bench<float, 1>([](const std::array<synapse::adiff<float>, 1>& a){ return a[0] * a[0]; })
            << " x*x adiff ops per second " << std::endl;
  return 0;
}