#include <synapse/expression.hpp>

#include <iostream>
#include <array>

template<typename T>
class wrapper {
  T val;
public:
  wrapper(const T& v) : val(v) {
    std::cout << "wrapper(" << &v << ") " << this << std::endl;
  }
  wrapper(T&& v) : val(std::move(v)) {
    std::cout << "wrapper(move " << &v << ") " << this << std::endl;
  }
  auto operator+(const wrapper& rhs) { return wrapper<T>(val + rhs.val); }
  operator T(){ return val; }
  const T& value() const { return val; }
};

template<typename E>
class vec_expr;

template<typename T, int rows>
struct vec {
  std::array<T, rows> data;
  auto cbegin() const { return data.cbegin(); }
  auto cend() const { return data.cend(); }
  auto begin() { return data.begin(); }
  auto end() { return data.end(); }
  auto operator+(const vec<T, rows>& v) {
   return make_vec_expr(synapse::plus, synapse::variable)
  }
};

int main() {
  using synapse::variable;
  int a, b, c;
  variable<int> x = a, y = b, z = c;
  auto f = x + y + z + x;
  a = 1, b = 2, c = 2;
  std::cout << f.eval() << std::endl;
  std::cout << "test_expression pass" << std::endl;
  return 0;
}