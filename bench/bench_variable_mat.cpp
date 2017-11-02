#include <synapse/benchmark.hpp>
#include <synapse/mat.hpp>
#include <synapse/variable.hpp>
#include <random>
#include <iostream>

template<typename T, int rows, int cols>
int bench() {
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_real_distribution<float> dist(-1, 1);
  auto rand = [&](){ return dist(gen); };
  const int sets = 1, reps = 10000, N = sets*reps;
  using synapse::mat;
  using synapse::vec;
  using synapse::variable;
  using synapse::workspace;
  workspace<float> ws;
  mat<variable<float>, rows, cols> m;
  for(auto& row : m) 
    for(auto& val : row)
      val = ws.param(rand());
  vec<variable<float>, cols> v, r;
  double t = synapse::stopwatch([&](){
    std::generate(v.begin(), v.end(), rand);
    std::fill(r.begin(), r.end(), 0);
    for(int rep=0; rep<reps; ++rep)
      r = r + m*v; 
  }, sets);
  return N/t;
}

int main() {
  //std::cout << bench<float, 2, 2>() << " float 2x2 * 2x1 per second" << std::endl;
  std::cout << bench<float, 3, 3>() << " float 3x3 * 3x1 per second" << std::endl;
  return 0;
}