#include "lambda.hpp"
#include "xassert.hpp"

//#include <vector>
//#include <algorithm>
#include <tuple>

namespace synapse {
      
  template<typename T>
  class variable {
    variable* id;
    const T* p;
  public:
    variable() : id(this), p(NULL) {}
    void bind(const T& val) { 
      XASSERT(this->p == NULL, "Attempt to bind variable which has already been bound!"); 
      p = &val; 
    }
    void reset() { 
      XASSERT(p, "Attempt to reset variable which has not been bound!"); 
      p = NULL;
    }
    T operator()() const {
      XASSERT(p, "Attempt access variable which has not been bound!"); 
      return *p; 
    }
    bool operator==(const variable& var) const {
      return id == var.id;
    }
  };
    
  template<typename Op, typename Vars>
  class function {
    Op op;
    Vars vars;
    template<int i>
    void bind() {}
    template<int i, typename Arg1, typename... Args>
    void bind(const Arg1& arg1, Args... args) {
      std::get<i>(vars).bind(arg1);
      bind<i+1>(args...);
    }
    template<int i>
    void reset() {}
    template<int i, typename Arg1, typename... Args>
    void reset(const Arg1& arg1, Args... args) {
      std::get<i>(vars).reset();
      reset<i+1>(args...);
    }
  public:
    function(const Op& op, Vars&& vars) : op(op), vars(std::move(vars)) {}
    template<typename... Args>
    auto operator()(Args... args) {
      static_assert(sizeof...(Args) == std::tuple_size<Vars>::value, 
                    "function operator() called with invalid number of arguments!");
      auto atuple = std::make_tuple(args...);
      bind<0>(args...);
      auto result = op();
      reset<0>(args...);
      return result;
    }
  };
    
  template<typename Op, typename Vars>
  auto make_function(const Op& op, Vars&& vars) { 
    return function<Op, Vars>(op, std::move(vars));
  }
    
  template<typename T1, typename T2>
  auto operator+(const variable<T1>& v1, const variable<T2>& v2) {
    if(v1 == v2) {
      auto vars = std::make_tuple(v1);
      return make_function([&]{ return 2*std::get<0>(vars)(); }, std::move(vars));
    }
    else {
      auto vars = std::make_tuple(v1, v2);
      return make_function([&]{ return std::get<0>(vars)() + std::get<1>(vars)(); }, std::move(vars));
    }
  }
    
};