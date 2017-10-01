#include <synapse/matrix.hpp>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
  synapse::matrix<float, 1, 3> m = {0, 4, 1};
  std::vector<int> v = {0, 1, 2};
  auto view = synapse::make_view(v.begin(), v.end());
  auto view_mat = synapse::make_view_matrix<3, 1>(view);
  auto result = m * view_mat;
  std::cout << "test_matrix pass" << std::endl;
  return 0;
}