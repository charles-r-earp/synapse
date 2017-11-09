#include "lambda.hpp"
#include <memory>

namespace synapse {
    
  template<typename T, template<typename...> class Vector>
  class tensor {
    Vector<T> p;
  };
    
  template<typename T, template<typename...> class Vector>
  class network;
    
  template<typename T, template<typename...> class Vector>
  class variable {
    network<T, Vector>* net;
    tensor val, grad;
  public:
    friend class network<T, Vector>;
  };
    
  template<typename T, template<typename...> class Vector>
  class network {
    Vector<T> params;
  public:
    auto fc(int n, variable<T, Vector>& in, variable<T, Vector>& out) { 
      params.insert(params.end(), n, 0);
      return [&]{ 
        XASSERT(!in.net or in.net == this, 
                "network.fc() input variable has been used in another network!");
        out = in;
      };
    }
  };
    
  
  
};