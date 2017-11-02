//#include "lambda.hpp"
#include "algorithm.hpp"

#include <tuple>
#include <vector>
#include <omp.h>
#include <ostream>
#include <initializer_list>
#include <iostream>
#include <cassert>

namespace synapse {
    
  template<typename T, std::size_t rows = 1, std::size_t cols = 1>
  class tensor {
    std::array<T, rows*cols> vals;
  public:
    tensor() {}
    template<typename ... Values>
    tensor(Values ... values) : vals{values...} {}
    const T& operator[](std::size_t i) const { return vals[i]; }
    T& operator[](std::size_t i) { return vals[i]; }
    auto cbegin() const { return vals.cbegin(); }
    auto cend() const { return vals.cend(); }
    auto begin() { return vals.begin(); }
    auto end() { return vals.end(); }
      
    friend auto& operator<<(std::ostream& os, tensor& t) {
      os << "<" << rows << ", " << cols << ">{";
      bool first = true;
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
    
  template<typename T>
  class parameter_ref;
    
  template<typename T, std::size_t rows = 1, std::size_t cols = 1>
  class parameter : public tensor<T, rows, cols> {
    std::size_t index;
    std::size_t *nparams = NULL;
  public:
    friend class parameter_ref<T>;
    using tensor<T, rows, cols>::tensor;
    virtual ~parameter() { if(nparams) *nparams = 0; }
  };
    
  template<typename T, std::size_t rows = 1>
  class variable : public tensor<T, rows> {
    std::vector<T> grads;
    void grads_resize(std::size_t s) {
      if(s > grads.size())
        grads.resize(s);
    }
  public:
    using tensor<T, rows>::tensor;
    variable operator+(variable& var) {
      grads_resize(var.grads.size());
      var.grads_resize(grads.size());
      variable result;
      transform(this->cbegin(), this->cend(), var.cbegin(), result.begin(), [](const T& l, const T& r){ return l+r; });
      return result;
    }
  };
    
  template<typename T>
  class network;
    
  template<typename T>
  class parameter_ref {
    T* vals;
    std::size_t* index;
    std::size_t** nparams;
    std::size_t nrows, ncols;
  public:
    template<std::size_t rows, std::size_t cols>
    parameter_ref(parameter<T, rows, cols>& param) {
      vals = param.begin();
      index = &param.index;
      nparams = &param.nparams;
      nrows = rows;
      ncols = cols;
    }
    friend class network<T>;
    friend auto& operator<<(std::ostream& os, parameter_ref& param) {
      os << "<" << param.nrows << ", " << param.ncols << ">[" 
           << *param.index << "/" << **param.nparams << "] ";
      return os;
    }
    auto size() const { return nrows * ncols; }
    auto cbegin() const { return vals; }
    auto cend() const { return vals+size(); }
    auto begin() { return vals; }
    auto end() { return vals+size(); }
  };
    
  template<typename T>
  class network {
    std::vector<parameter_ref<T>> params;
    std::size_t nparams;
    void check_nparams() {
      if(!nparams) {
        std::cout << "ERROR: network.nparams == 0. This may occur if parameters to the network are deleted" 
            << "or go out of scope. This is a safety feature to prevent invalid memory access. " << std::endl;
        std::terminate();
      }
    }
  public:
    network(std::initializer_list<parameter_ref<T>> params) : params(std::move(params)) {
      assert(this->params.size());
      nparams = 0;
      for(auto& param : this->params) {
        assert(*param.nparams == NULL);
        *param.nparams = &nparams;
        *param.index = **param.nparams;
        nparams += param.nrows * param.ncols;
      }
    } 
    template<typename F>
    void init(const F& f) {
      check_nparams();
      for(auto& param : params) {
        for(auto& val : param) {
          val = f();
        }
      }
    }
    void init() { init([]{ return T(2)*rand()/RAND_MAX - T(1); }); };
    friend auto& operator<<(std::ostream& os, network& net) {
      net.check_nparams();
      for(auto& param : net.params) {
        os << param;
      }
      return os;
    }
  };
    
};