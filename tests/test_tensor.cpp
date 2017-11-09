#include <synapse/tensor.hpp>
#include <iostream>

int main() {
  using synapse::tensor;
  tensor<float, std::vector> x, y;
  auto z = x + y;
  std::cout << "test_tensor pass" << std::endl;
  return 0;
}