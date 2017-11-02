#include <string>
#include <sstream>

#ifndef SYNAPSE_LOGGING
#define SYNAPSE_LOGGING

namespace synapse {
    
  template<class Sequence>
  auto comma_sep_list(const Sequence& seq) {
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
  }
  
  {
  protected:
    virtual std::string prefix() { return "{"; }
    virtual std::string delim() { return ", "; }
    virtual std::string suffix() { return "}"; }
  public:
    using Container::Container;
    friend auto& operator<<(std::ostream& os, printable& p) {
      
    }
  };
    
};

#endif // SYNAPSE_LOGGING