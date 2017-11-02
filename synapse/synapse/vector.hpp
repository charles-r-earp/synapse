//#include <thrust/host_vector.h>

#ifndef SYNAPSE_VECTOR
#define SYNAPSE_VECTOR

namespace synapse {
    
  template<typename T, template<typename...> class Container>
  using vector = Container<T>;
    
};

#endif // SYNAPSE_VECTOR