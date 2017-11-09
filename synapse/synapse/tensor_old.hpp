#include "xassert.hpp"

#include <thrust/functional.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>
#include <thrust/logical.h>
#include <thrust/host_vector.h>

#include <ostream>
#include <initializer_list>
#include <numeric>

#ifndef SYNAPSE_TENSOR
#define SYNAPSE_TENSOR

namespace synapse {
    
  class indices_t : public thrust::host_vector<int> {};
    
  class dimensions_t {
    thrust::host_vector<int> _dims;
  public:
    dimensions_t(int ndims = 0, int init = 1) : _dims(ndims, init) {
      XASSERT(init >= 0, "Dimensions must be positive and non-zero!");
    }
    dimensions_t(const std::initializer_list<int>& list) : _dims(list.size()) {
      XASSERT(thrust::all_of(list.begin(), list.end(), [](const auto& i){ return i>0; }), "Dimensions must be positive and non-zero!");
      thrust::copy(list.begin(), list.end(), _dims.begin());
    }
    auto& operator[](int i) { return i >= 0 ? _dims[i] : _dims[_dims.size()-i]; }
    const auto operator[](int i) const { return i >= 0 ? _dims[i] : _dims[_dims.size()-i]; }
    auto begin() { return _dims.begin(); }
    auto cbegin() const { return _dims.cbegin(); }
    auto end() { return _dims.end(); }
    auto cend() const { return _dims.cend(); }
    auto size() const { return _dims.size(); }
    auto empty() const { return _dims.empty(); }
    void resize(int s) { _dims.resize(s); }
  };
    
  class shape_t {
   dimensions_t _dims;
   mutable dimensions_t _sizes;
  public:
    shape_t(int ndims = 0, int init = 1) : _dims(ndims, init) {}
    shape_t(const std::initializer_list<int>& list) : _dims(list.size()) {}
    const auto& sizes() const { 
        if(_sizes.empty()) {
          if(!_dims.empty()) {
            _sizes.resize(_dims.size());
            std::partial_sum(_dims.cbegin(), _dims.cend(), _sizes.begin(), std::multiplies<int>());
          }
        }
        return _sizes; 
    }
    auto ndims() const { return _dims.size(); }
    void set_dim(int i, int dim) { 
      XASSERT(dim >= 0, "Negative shape dimension!");
      _dims[i] = dim;  
    }
    const auto operator[](int i) const { return _dims[i]; }
    auto cbegin() const { return _dims.cbegin(); }
    auto cend() const { return _dims.cend(); }
    friend auto& operator<<(std::ostream& os, const shape_t& s) {
      os << "shape_t{";
      for(auto it=s.cbegin(); it!=s.cend(); ++it) {
        if(it==s.cbegin())
          os << *it;
        else 
          os << ", " << *it;
      }
      return os << '}';
    }
  };
  
  template<typename T, template<typename...> class Container = thrust::host_vector>
  class tensor {
    mutable shape_t _shape;
    Container<T> _data;
  public:
    template<typename... Types>
    using container_type = Container<Types...>;
    tensor() {}
    tensor(const shape_t& shape, T init = 0) : _shape(shape), _data(_shape.sizes()[-1], init) {}
    tensor(std::initializer_list<T>&& list) : _shape{list.size()}, _data(list.size()) {
      thrust::copy(list.begin(), list.end(), _data.begin());
    }
    auto& operator[](int i) { return _data[i]; }
    const auto& operator[](std::size_t i) const { return _data[i]; }
    auto begin() { return _data.begin(); }
    auto cbegin() const { return _data.cbegin(); }
    auto end() { return _data.end(); }
    auto cend() const { return _data.cend(); }
    const auto& shape() const { return _shape; }
    friend auto& operator<<(std::ostream& os, const tensor& t) {
      os << "tensor(" << t.shape() << ",{";
      for(auto it=t.cbegin(); it!=t.cend(); ++it) {
        if(it==t.cbegin())
          os << *it;
        else 
          os << ", " << *it;
      }
      return os << "})";
    }
  };
    
  /*template<typename T, template<typename...> class Container = thrust::host_vector>
  class tensor : public tensor<T, Container> {
  public:
    tensor() : tensor_base<T, Container>() {}
    tensor(const shape_t& shape, ) : tensor_base<T, Container>(shape) {}
    tensor(std::initializer_list<T>&& list) : tensor_base<T, Container>(std::move(list)) {}
    void reshape(const shape_t& s) {
      this->shape = s;
      this->resize(this->shape.size());
    }
    friend auto& operator<<(std::ostream& os, const tensor& t) {
      os << "tensor(" << t.shape() << ", {";
      for(auto it=t.cbegin(); it!=t.cend(); ++it) {
        if(it==t.cbegin())
          os << *it;
        else 
          os << ", " << *it;
      }
      return os << '})';
    }
  };*/
    
  /*template<typename T, template<typename...> class Container>
  auto operator+(const tensor<T, Container>& t1, const tensor<T, Container>& t2) {
    tensor<T, Container> result(t1.shape());
    thrust::transform(t1.cbegin(), t1.cend(), t2.cbegin(), result.begin(), thrust::plus<T>());
    return result;
  }*/
};

#endif // SYNAPSE_TENSOR