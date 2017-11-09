//#include "xassert.hpp"

#include <thrust/remove.h>
#include <thrust/host_vector.h>

//#include <ostream>
//#include <initializer_list>
//#include <numeric>

#ifndef SYNAPSE_TENSOR
#define SYNAPSE_TENSOR

namespace synapse {
    
  template<typename T, template<typename...> class Vector>
  class parameter;
    
  template<typename T, template<typename...> class Vector>
  class tensor;
    
  template<typename T, template<typename...> class Vector>
  class workspace {
    thrust::host_vector<tensor<T, Vector>*> tensors;
    thrust::host_vector<parameter<T, Vector>*> params;
    int nders;
    struct derivative_inserter {
      workspace* ws;
      bool before;
      int n;
      void operator()(tensor<T, Vector>* t) { t->insert_nders(ws, n, before); }
    };
    struct workspace_remover {
      workspace* ws;
      void operator()(tensor<T, Vector>* t) { t->remove_from_workspace(ws); }
    };
  public:
   // const int get_nders() { return nders; }
    void extend(workspace& other) {
      if(other.tensors.size()) {
        if(other.nders) {
          thrust::for_each(tensors.begin(), tensors.end(), derivative_inserter{this, true, other.nders});
        }
        auto it = tensors.end();
        tensors.insert(it, other.tensors.cbegin(), other.tensors.cend());
        other.tensors.clear();
        thrust::for_each(it, tensors.end(), derivative_inserter{this, false, nders});
      }
      else if(other.nders) {
        thrust::for_each(tensors.begin(), tensors.end(), derivative_inserter{this, true, other.nders});
      }
      nders += other.nders;
    }
    void add_tensor(tensor<T, Vector>* t) {
      if(t->ws_ptr() == this);
      else if(t->ws_ptr()) 
        extend(*t->ws_ptr());
      else 
        tensors.push_back(t);
    }
    void remove_tensor(tensor<T, Vector>* t) {
      thrust::remove(tensors.begin(), tensors.end(), t);
    }
    void add_parameter(parameter<T, Vector>* p) {
      if(p->ws_ptr() == this);
      else if(p->ws_ptr()) 
        extend(*p->ws_ptr())
      else
        params.push_back(p);
    }
    ~workspace() {
      if(tensors.size()) {
        thrust::for_each(tensors.begin(), tensors.end(), workspace_remover{this});
        tensors.clear();
      }
    }
  };
    
  template<typename T, template<typename...> class Vector>
  class parameter {
    workspace<T, Vector>* ws;
    Vector<T> _data;
    int index;
  public:
    parameter() : ws(new workspace<T, Vector>()) {}
  };
    
  template<typename T, template<typename...> class Vector>
  class tensor {
    workspace<T, Vector>* ws;
    Vector<T> _data;
    //shape_t _shape;
    int nders;
  public:
    tensor() : ws(new workspace<T, Vector>()) { ws->add_tensor(this); }
    const auto ws_ptr() { return ws; }
    void remove_from_workspace(workspace<T, Vector>* ws) { if(ws == this->ws) this->ws = NULL; }
    void insert_nders(workspace<T, Vector>* ws, int n, bool before) {
      this->ws = ws;
      if(n) {
        for(auto it = _data.begin(); it!= _data.end(); it+=nders+n) {
          _data.insert(before ? it+1 : it+1+nders, n, 0);
        }
      }
    }
    tensor operator+(tensor& t) {
      ws->add_tensor(&t);
      return *this;
    }
    ~tensor() { if(ws) ws->remove_tensor(this); }
  };
};

#endif // SYNAPSE_TENSOR