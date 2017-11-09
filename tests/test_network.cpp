//#include <synapse/network.hpp>
#include <iostream>
#include <vector>

template<typename T>
class vec_iterator {
  T* p;  
public:
  vec_iterator(T* p) : p(p) {}
  auto operator!=(const vec_iterator& it) const { return it.p != p; }
  auto& operator++() { ++p; return *this; }
  auto& operator*() { return *p; }
};

template<typename Vector>
class vec {
  Vector v;
public:
  using value_type = typename Vector::value_type;
  vec() {}
  auto begin() { return vec_iterator<value_type>(v.data()); }
  auto end() { return vec_iterator<value_type>(v.data()+v.size()); }
};

int main() {
  vec<std::vector<int>> x;
  for(auto& val : x);
  std::cout << "test_network pass" << std::endl;
  return 0;
}