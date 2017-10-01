#include <synapse/adiff.hpp>
#include <cassert>
#include <iostream>

int main() {
  {
    synapse::adiff<float> x = {2, { {0, 1} }};
    auto y = 3*x;
    assert(y.d(0) == 3);
  }
  {
    synapse::adiff<float> x = {2, { {0, 1} }};
    auto y = x*x;
    assert(y.d(0) == 4);
  }
  std::cout << "test_adiff pass" << std::endl;
  return 0;
}