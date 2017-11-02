#include <synapse/benchmark.hpp>
#include <synapse/mat.hpp>
#include <random>
#include <iostream>

template<typename T, int rows, int cols>
int bench() {
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_real_distribution<float> dist(-1, 1);
  auto rand = [&](){ return dist(gen); };
  const int sets = 100, reps = 1000000, N = sets*reps;
  using synapse::mat;
  using synapse::vec;
  mat<float, rows, cols> m;
  vec<float, cols> v, r;
  double t = synapse::stopwatch([&](){
    std::for_each(m.begin(), m.end(), [&rand](vec<float, cols>& row){ 
      std::generate(row.begin(), row.end(), rand);
    });
    std::generate(v.begin(), v.end(), rand);
    for(int rep=0; rep<reps; ++rep)
      r = m*v; 
  }, sets);
  return N/t;
}

int main() {
  std::cout << bench<float, 2, 2>() << " float 2x2 * 2x1 per second" << std::endl;
  std::cout << bench<float, 3, 3>() << " float 3x3 * 3x1 per second" << std::endl;
  return 0;
}