#include <synapse/function.hpp>

#include <iostream>

int main() {
  using synapse::variable;
  variable<int> x;
  auto y = x + x;
  std::cout << y(1) << std::endl;
  std::cout << "test_function pass" << std::endl;
  return 0;
}