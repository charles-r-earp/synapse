#include <synapse/benchmark.hpp>
#include <synapse/matrix.hpp>
#include <random>
#include <iostream>

template<typename T, int rows, int same, int cols>
int bench() {
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_real_distribution<float> dist(-1, 1);
  auto rand = [&](){ return dist(gen); };
  const int sets = 100, reps = 100000, N = sets*reps;
  synapse::matrix<float, rows, same> a;
  synapse::matrix<float, same, cols> b, r;
  double t = synapse::stopwatch([&](){
    std::generate(a.begin(), a.end(), rand);
    std::generate(b.begin(), b.end(), rand);
    for(int rep=0; rep<reps; ++rep)
      r = a*b; 
  }, sets);
  return N/t;
}

int main() {
  std::cout << bench<float, 2, 2, 1>() << " float 2x2 * 2x1 per second" << std::endl;
  //std::cout << bench<float, 2, 2, 2>() << " float 2x2 * 2x2 per second" << std::endl;
  std::cout << bench<float, 3, 3, 1>() << " float 3x3 * 3x1 per second" << std::endl;
  //std::cout << bench<float, 3, 3, 3>() << " float 3x3 * 3x3 per second" << std::endl;
  return 0;
}