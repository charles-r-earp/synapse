#include <string>

#ifndef SYNAPSE_TENSOR
#define SYNAPSE_TENSOR

namespace synapse {
    
  struct tensor_dimension {
    std::size_t value;  
    //std::size_t operator() { return value; }
  };
    
  class tensor_shape : public std::vector<tensor_dimension> {
  public:
    using std::vector<tensor_dimension>::vector;
    std::stringstream ss;
    ss << '{';
    if(p.size()) {
      auto del = p.delim();
        auto it = p.cbegin();
        os << *it++;
        for(; it!= p.cend(); ++it)
          os << del << *it;
      }
      return os << p.suffix();
  };
  /*  
  template<typename T, template<typename...> class Container, typename Shape>
  class tensor : public Container<T> {
    std::vector<std::size_t> dims;
    public:
    tensor() {}
    tensor(std::vector)
  };
  
  template<typename T, template<typename...> class Container>
  class adtensor : public tensor<T, Container> {
    tensor grad;
  public:
    tensor& 
  };*/
    
};

#endif // SYNAPSE_TENSOR