#include <thrust/iterator/permutation_iterator.h>
#include <thrust/copy.h>

#include <ostream>
#include <iostream>
#include <string>

namespace synapse {
    
  class println {
    std::ostream* os;
    std::string end;
    public:
    println(std::ostream* os = &std::cout, std::string end = "\n") : os(os), end(end) {}
    template<typename T>
    void operator()(const T& value) { *os << value << end; }
  };

  template<typename Container>
  class adiff {
    using T = typename Container::value_type;
    // data is values then gradients
    Container _data;
    // derivatives for each value
    int _nvalues, _nders;
  public:
    using value_type = T;
    adiff(const Container& c, int nders = 0) : _nvalues(c.size()) _nders(nders), _data((nders+1)*_nvalues)) {
      thrust::copy(c.cbegin(), c.cend(), _data.begin());
    }
    auto cbegin() { return _data.cbegin(); }
    auto cend() { return _data.cend(); }
    auto clmul_iterator { 
      std::vector<int> indices()
       
    }
    auto crmul_iterator {
      
    }
  };
};