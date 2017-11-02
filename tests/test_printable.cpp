#include <synapse/printable.hpp>
#include <vector>
#include <iostream>

int main() {
  using synapse::printable;
  printable<std::vector<float>> x{1, 2, 3, 4};
  std::cout << x << std::endl;
  std::cout << "test_tensor pass" << std::endl;
  return 0;
}