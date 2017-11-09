#include "xassert.hpp"

#include <thrust/iterator/transform_iterator.h>

#include <tuple>
#include <type_traits>
#include <functional>
#include <iostream>
#include <iterator>
#include <array>

namespace synapse {
    
  template<typename E>
  class tensor_expr {
  public:
    //using iterator = typename E::iterator;
    //using const_iterator = typename E::const_iterator;
    auto operator[](int i) const { return static_cast<E const&>(*this)[i]; }
    static constexpr int size() { return E::size(); }
    //iterator begin() { return static_cast<E const&>(*this).begin(); }
    auto cbegin() const { return static_cast<E const&>(*this).cbegin(); }
    //iterator end() { return static_cast<E const&>(*this).end(); }
    auto cend() const { return static_cast<E const&>(*this).cbegin(); }
  };
    
  template<typename It1, typename It2, typename Op>
  class binary_op_iterator : public std::random_access_iterator_tag {
    It1 it1;
    It2 it2;
    Op const& op;
  public:
    binary_op_iterator(const It1& it1, const It2& it2, const Op& op) : it1(it1), it2(it2), op(op) {}
    const auto *operator->() const { return op(*it1, *it2); }
    auto operator++(int) { auto tmp = *this; ++it1; ++it2; return tmp; }
    //auto operator() { ++it1; ++it2; return this; }
    auto operator!=(const binary_op_iterator& it) const { 
      return it.it1 != it1 or it.it2 != it2; 
    }
    auto operator==(const binary_op_iterator& it) const { 
      return !(*this != it);
    }
  };
   
  template<typename It1, typename It2, typename Op>
  auto make_binary_op_iterator(const It1& it1, const It2& it2, const Op& op) {
    return binary_op_iterator<It1, It2, Op>(it1, it2, op);
  }
    
  template<typename E1, typename E2, typename Op>
  class array_op : public tensor_expr<array_op<E1, E2, Op>> {
    E1 const& e1;
    E2 const& e2;
    const Op op;
  public:
    array_op(const E1& e1, const E2& e2, const Op& op) : e1(e1), e2(e2), op(op) {}
    static constexpr int size() { return E1::size(); }
    auto cbegin() const { 
      return make_binary_op_iterator(e1.cbegin(), e2.cbegin(), op); 
    }
    auto cend() const {
      return make_binary_op_iterator(e1.cend(), e2.cend(), op); 
    }
  };
    
  template<typename E1, typename E2, typename Op>
  auto make_array_op(const E1& e1, const E2& e2, const Op& op) {
    return array_op<E1, E2, Op>(e1, e2, op);
  }
    
  template<int... dims>
  struct shape_t;
    
  template<>
  struct shape_t<> {
    static const int ndims = 0;
    template<int i>
    constexpr typename std::enable_if<i>=0, int>::type dim() const { 
      return 1;
    }
    static const int size = 1;
  };
    
  template<int dim1, int... dims>
  struct shape_t<dim1, dims...> {
    static const int ndims = sizeof...(dims);
    template<int i>
    static constexpr typename std::enable_if<i<ndims, int>::type dim() { 
      return std::get<i>(std::make_tuple(dims...));
    }
    template<int i>
    static constexpr typename std::enable_if<i>=ndims, int>::type dim() { 
      return 1;
    }
    static const int size = dim1 * shape_t<dims...>::size;
  };
    
  template<typename Vector, int... dims> 
  class tensor : public tensor_expr<tensor<Vector, dims...>> {
    Vector vec;
  public:
    using value_type = typename Vector::value_type;
    //using iterator = typename Vector::iterator;
    //using const_iterator = typename Vector::const_iterator;
    static const shape_t<dims...> shape;
    tensor() : vec(shape.size) {}
    /*template<typename... Args>
    tensor(Args... args) : vec{args...} {
      static_assert(sizeof...(Args)==shape.size, "Invalid number of arguments to tensor{} constructer!");
    }*/
    template<typename E>
    tensor(const tensor_expr<E>& e) {
      for(const auto& val : e);
        //vec.push_back(val);
      static_assert(e.size() == shape.size, "Attempt to assign tensor to container of different size()!");
    }
    static constexpr int size() { return shape.size; }
    auto begin() { return vec.begin(); }
    auto cbegin() const { return vec.cbegin(); }
    auto end() { return vec.end(); }
    auto cend() const { return vec.cend(); }
    auto operator+(const tensor& t) {
      return make_array_op(*this, t, std::plus<value_type>());
    }
    /*auto operator=(const value_type& val) {
      XASSERT(it!=end(), "Attempt to comma-initialize tensor after it has been fully initialized.");
      std::cout << "comma init: " << std::distance(begin(), it) << " -> " << val << std::endl;
      *it++ = val;
      return *this;
    }*/
  };
    
};