#include <thrust/host_vector>
#include <thrust/tupple>

#include <vector>
#include <cassert>
#include <ostream>
#include <initializer_list>

namespace synapse {
    
  template<std::size_t dims...>
  class tensor_shape : public decltype(thrust::make_tuple(dims...())>) {
  public:
    tensor_shape(std::initializer_list<std::size_t>&& list) : std::vector<std::size_t>(std::move(list)) {} 
    auto tensor_size() const {
      std::size_t s = 1;
      for(const auto& dim : *this)
        s *= dim;
      return s;
    }
    friend auto& operator<<(std::ostream& os, const tensor_shape& shape) {
      bool first = true;
      os << "{";
      for(auto& dim : shape) {
        if(first) {
          first = false;
          os << dim;
        }
        else {
          os << ", " << dim;
        }
      }
      os <<"}";
      return os;
    }
  };
    
  
  
  template<class Container = thrust::host_vector<double>, class ExecutionPolicy = thrust::host>
  class tensor : public Container {
    tensor_shape dims;
  public:
    tensor() {}
    tensor(const tensor_shape& shape) : Container(shape.tensor_size()), dims(shape) {}
    const auto& shape() const { return dims; }
    auto operator+(const tensor& t) {
      assert(t.shape() == dims);
      tensor result(dims);
      transform(ExecutionPolicy::policy(), this->cbegin(), this->cend(), t.cbegin(), result.begin(), [](const auto& l, const auto& r){ 
        return r+l; 
      });
      return result;
    }
    friend auto& operator<<(std::ostream& os, tensor& t) {
      bool first = true;
      os << "(" << 
      first = true;
      os <<"}){";
      for(auto& val : t) {
        if(first) {
          first = false;
          os << val;
        }
        else {
          os << ", " << val;
        }
      }
      os << "}";
      return os;
    }
  };
};