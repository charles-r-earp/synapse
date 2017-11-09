#include <synapse/xassert.hpp>

#include <iostream>
#include <string>

int main() {
  int x = 2;
  XASSERT(!x, ("x is " + std::to_string(x)).c_str());
  std::cout << "test_xassert pass" << std::endl;
  return 0;
}