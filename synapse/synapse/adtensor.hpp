#include "tensor.hpp"

#ifndef SYNAPSE_ADTENSOR
#define SYNAPSE_ADTENSOR

namespace synapse {
  template<typename T, template<typename...> class Container = thrust::host_vector>
  class adtensor : public tensor_base<T, Container> {
    tensor<T, Container> _grad;
  public:
    adtensor() : tensor_base<T, Container>() {}
    adtensor(const shape_t& shape) : tensor_base<T, Container>(shape) {}
    adtensor(std::initializer_list<T>&& list) : tensor_base<T, Container>(std::move(list)) {}
    void reshape(shape_t& s) {
      this->shape = s;
      this->resize(this->shape.size());
      if(_grad.size()) {
        shape_t gs(s.size()+1);
        thrust::copy(s.cbegin(), s.cend(), gs.begin());
        *(gs.end()-1) = *(_grad.shape().cend()-1);
        _grad.reshape(gs);
      }
    }
    void resize_gradient(std::size_t s) {
      shape_t gs(s+1);
      thrust::copy(_grad.size().cbegin(), _grad.size().cend(), gs.begin());
      _grad.reshape(gs);
    }
    friend auto& operator<<(std::ostream& os, adtensor& t) {
      os << '(' << t.shape() << '[';
      if(t._grad.shape().ndims())
        os << '[' << *(t._grad.shape().cend()-1) << ']';
      os << "){";
      for(auto it=t.cbegin(); it!=t.cend(); ++it) {
        if(it==t.cbegin())
          os << *it;
        else 
          os << ", " << *it;
      }
      os << " [";
      for(auto it=t._grad.cbegin(); it!=t._grad.cend(); ++it) {
        if(it==t.cbegin())
          os << *it;
        else 
          os << ", " << *it;
      }
      os << ']';
      return os << '}';
    }
  }; 
    
  /*template<typename T, template<typename...> class Container>
  auto operator+(const tensor<T, Container>& t1, const tensor<T, Container>& t2) {
    tensor<T, Container> result(t1.shape());
    thrust::transform(t1.cbegin(), t1.cend(), t2.cbegin(), result.begin(), thrust::plus<T>());
    return result;
  }*/
};

#endif // SYNAPSE_ADTENSOR