#include <thrust/for_each.h>

#include <synapse/adiff.hpp>

#include <iostream>
#include <vector>

int main() {
  using synapse::adiff;
  using synapse::println;
  adiff<std::vector<float>> x({0, 1, 4});
  thrust::for_each(x.cbegin(), x.cend(), println());
  std::cout << "test_adiff pass" << std::endl;
  return 0;
}